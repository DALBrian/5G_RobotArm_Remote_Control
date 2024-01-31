#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <conio.h>
#include <stdio.h>
#include "Header.h"
#include <time.h>
#include <modbus.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <Windows.h>
#include <thread>
#include <winsock.H> 
#include <WS2tcpip.h>
/*
@author: DAL
@date: 2020/9/23
@modified by: Small Brian
@date: 2021/7/20
@brief: This program is used to control the robotic arm to pick up the product and use modbus to control vacuum valve.
@details: The program was created by unknown students from DAL, and I modified it to make it to communicate with Raspberry Pi via TCP/IP.
Using the information that raspberry pi sent to control the robotic arm to pick up the product.
Comments added by Small Brian.
*/
#pragma comment(lib, "ws2_32.lib") 
#pragma warning(disable:4996)

using namespace std;
enum {
	TCP,
	TCP_PI,
	RTU
};

int main()
{
	string ipAddress = "192.168.225.39"; // IP Address of the robot arm
	int port = 8000; // Port of the robot arm
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		cerr << "Can't start Winsock, Err #" << wsResult << endl;
	}
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0); 	// Create TCP/IP connection  
	if (sock == INVALID_SOCKET)
	{
		cerr << "Can't create socket, Err #" << WSAGetLastError() << endl;
		WSACleanup(); 
	}
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);
	// Connect to Raspberry Pi
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		cerr << "Can't connect to server, Err #" << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
	}
	else // Connection is successful
	{
		modbus_t *ctx;
		modbus_mapping_t *mb_mapping;
		RTN_ERR ret = 0;
		I32_T   devIndex = 0;
		I32_T   retDevID = 0;
		I32_T   retGroupCount = 0;
		I32_T   retGroupAxisCount = 0;
		I32_T   GroupIndex = 0;
		F64_T   paravalue = 0;
		F64_T   PMaxVel = 10000;
#ifdef UNDER_WIN32_SIMULATION
		I32_T devType = NMC_DEVICE_TYPE_SIMULATOR;
		U32_T sleepTime = 1000;
#else
		I32_T devType = NMC_DEVICE_TYPE_ETHERCAT; 
		U32_T sleepTime = 2000;
#endif

		OpenDevice(devType, devIndex, retDevID, sleepTime, retGroupCount, retGroupAxisCount);
		SafyOn(retDevID, sleepTime, retGroupCount);
		ResetAlarm(retDevID, sleepTime, retGroupCount);
		SetBreak(Break_Off, retDevID, sleepTime, retGroupCount);
		GetPoseValue(retDevID, GroupIndex, retGroupCount, sleepTime);
		Pos_T HomePos = { 0,90,0,0,0,0 }; // Home point
		SetHome(HomePos, retDevID, sleepTime, retGroupCount, GroupIndex, retGroupAxisCount);

		ret = NMC_GroupSetParamI32(retDevID, GroupIndex, 0x30, 0, 0);
		if (ret != 0) {
			cout << "ERROR! NMC_GroupSetParamI32:" << NMC_GetErrorDescription(ret, NULL, 0) << endl; 
			CloseDevice(retDevID, sleepTime, retGroupCount); 
		}
		else {
			cout << "set abs success" << endl;
		}
		ret = NMC_GroupSetVelRatio(retDevID, GroupIndex, 100); 

		if (ret != 0) {
			cout << "ERROR! NMC_GroupSetVelRatio:" << NMC_GetErrorDescription(ret, NULL, 0) << endl;
			CloseDevice(retDevID, sleepTime, retGroupCount);
		}
		else {
			cout << "feed ratio : 100%" << endl;
		}
		for (int j = 0; j < 6; j++) {
			ret = NMC_GroupAxSetParamF64(retDevID, GroupIndex, j, 0x32, 0, 30);
		}

		Pos_T standbypos = { 0,75,0,0,-75,0 }; // Initial start point for image caputrue
		ret = NMC_GroupSetParamI32(retDevID, GroupIndex, 0x30, 0, 0);
		if (ret != 0) {
			cout << "ERROR! NMC_GroupSetParamI32:" << NMC_GetErrorDescription(ret, NULL, 0) << endl;
			CloseDevice(retDevID, sleepTime, retGroupCount);
		}
		ret = NMC_GroupPtpAcsAll(retDevID, GroupIndex, 63, &standbypos);
		if (ret != 0) {
			cout << "ERROR! NMC_GroupPtpAcsAll:" << NMC_GetErrorDescription(ret, NULL, 0) << endl;
			CloseDevice(retDevID, sleepTime, retGroupCount);
		}
		ret = WaitCmdReached(retDevID, GroupIndex, retGroupAxisCount);
		if (ret != 0)
		{
			CloseDevice(retDevID, sleepTime, retGroupCount);
		}
		ctx = modbus_new_rtu("COM7", 9600, 'N', 8, 1);
		if (ctx == NULL)
		{
			fprintf(stderr, "modbus doesn't control");
			return -1;
		}
		else {
			cout << "modbus connected!" << endl;
		}
		modbus_set_debug(ctx, 1);
		modbus_set_slave(ctx, 1);
		modbus_connect(ctx);
		uint16_t reg[64];
		int num = modbus_read_registers(ctx, 0, 5, reg);
		if (num != 5)
		{
			fprintf(stderr, "failed to read:", modbus_strerror(errno));
		}
		modbus_write_register(ctx, 0, 400);

		char buf[256];
		string Output;
		bool isstoped = false; // Keyboard press "y" to stop the program
		string ans;
        /*
        Below block added by Small Brian
        */
		while (!isstoped) 
		{
			// Send start command to Raspberry Pi
			Output = "start"; 
			int sendResult = send(sock, Output.c_str(), Output.size() + 1, 0);
			if (sendResult != SOCKET_ERROR) // If send command is successful
			{
				std::cout << "Send> " << Output << endl; // for debug
				// Wait for response // 
				ZeroMemory(buf, 256);
				int bytesReceived = recv(sock, buf, 256, 0);
				std::cout << "recv: " << bytesReceived << endl; // for debug
				if (bytesReceived > 0)
				{
					string recive;
					vector<int> information; // Axises information: x,y,z,agle(0,1,2,3)
					recive = string(buf, 0, bytesReceived);
					cout << recive << endl;
					information = spilt_string(recive);
					std::cout << information[0] << endl; // for debug

                    float pos_x, pos_y, pos_y1, pos_z, angle_a, timego, cy;
                    float shape, timeused;
                    string camera_cmd;
                    int is_arrival = 1;
                    pos_x = information[0]; // Coordinate of the object
                    pos_y = information[1]; 
                    shape = information[2]; // The shape of the object, didn't use
                    pos_z = 568; // In case the robot arm hit the table
                    is_arrival = information[3]; // Used to check if the robot arm is arrived at the point
                    
                    if (ret != 0) // Check robot arm status
                    {
                        cout << "ERROR! NMC_GroupSetParamI32:" << NMC_GetErrorDescription(ret, NULL, 0) << endl;
                        CloseDevice(retDevID, sleepTime, retGroupCount);
                    }

                    ret = NMC_GroupSetParamF64(retDevID, GroupIndex, 0x32, 0, 1000);
                    ret = NMC_GroupSetParamF64(retDevID, GroupIndex, 0x33, 0, 1000);

                    camera_cmd = "e"; // Stop the camera in case didn't close successfully last time
                    sendResult = send(sock, camera_cmd.c_str(), camera_cmd.size() + 1, 0); 
                    // Correction the drift of the robot arm and the camera
                    float xx = 0, yy = 0, factor = 0.42; // factor is the ratio of the distance and the image pixel; measuere by pixel->mm experimentally.
                    xx = 454.836 + (pos_x * factor); 
                    yy = pos_y * factor;
                    cout << "move to {  " << xx << ',' << yy << ',' << pos_z << ',' << angle_a << ',' << timego << '}' << endl; // for debug
                    Pos_T point = { xx, yy, pos_z,0,0,-180 }; // Move to the point to pick up the object
                    //cout << "In first point now " << endl;
                    NMC_GroupPtpCartAll(retDevID, GroupIndex, 63, &point); 
                    if (ret != 0)
                    {
                        cout << "ERROR! NMC_Groupline:" << NMC_GetErrorDescription(ret, NULL, 0) << endl;
                        break;
                    }

                    if (ret != 0)
                    {
                        CloseDevice(retDevID, sleepTime, retGroupCount);
                        break;
                    }

                    camera_cmd = "s"; // Open the camera again to check pick up status
                    sendResult = send(sock, camera_cmd.c_str(), camera_cmd.size() + 1, 0); 

                    bytesReceived = recv(sock, buf, 256, 0);
                    recive = string(buf, 0, bytesReceived); // x,y,z,agle(0,1,2,3)
                    cout << recive << endl;
                    information = spilt_string(recive);

                    pos_x = information[0];
                    pos_y = information[1];
                    is_arrival = information[3]; 
                    xx += pos_x*factor;
                    yy += pos_y*factor;

                    float run_x = 0, run_y = 0;
                    run_x = xx;
                    run_y = yy;

                    while (is_arrival == 0) //is_arrival == 0 means the robot arm doesn't pick up object succfessfully in the first time
                    {
                        cout << "Go to new point" << endl; // for debug
                        camera_cmd = "e"; 
                        sendResult = send(sock, camera_cmd.c_str(), camera_cmd.size() + 1, 0);

                        cout << "Move to {  " << run_x << ',' << run_y << ',' << pos_z << ',' << angle_a << ',' << timego << '}' << endl;
                        point = { run_x,run_y,pos_z,0,0,-180 };
                        NMC_GroupLine(retDevID, GroupIndex, 63, &point, &PMaxVel);

                        ret = NMC_GroupSetParamF64(retDevID, GroupIndex, 0x32, 0, 1000); 
                        ret = NMC_GroupSetParamF64(retDevID, GroupIndex, 0x33, 0, 1000); 
                        ret = NMC_GroupSetParamF64(retDevID, GroupIndex, 0x34, 0, 100);

                        camera_cmd = "s"; 
                        sendResult = send(sock, camera_cmd.c_str(), camera_cmd.size() + 1, 0);
                        bytesReceived = recv(sock, buf, 256, 0);
                        recive = string(buf, 0, bytesReceived); 
                        cout << recive << endl;
                        information = spilt_string(recive);
                        is_arrival = information[3]; // Refresh status

                        if (is_arrival == 0) // If still now arrival, store now position in xx and yy, next time the robot arm will move by increment.
                        {
                            pos_x = information[0]; // Position from the new image
                            pos_y = information[1];
                            xx += pos_x*factor; 
                            yy += pos_y*factor;
                            run_x = xx;
                            run_y = yy;
                        }

                    }

                    cout << "The point is correct" << endl;
                    camera_cmd = "e"; // End of this loop
                    sendResult = send(sock, camera_cmd.c_str(), camera_cmd.size() + 1, 0);
                    cout << "ready to go down" << endl;
                    thread th_modbus(thread_modbus, ctx); 
                    th_modbus.detach();
                    ret = NMC_GroupSetParamF64(retDevID, GroupIndex, 0x32, 0, 1000); 
                    ret = NMC_GroupSetParamF64(retDevID, GroupIndex, 0x33, 0, 1000); 
                    
                    point = { run_x+76 ,run_y+39,120,0,0,-180 }; 
                    NMC_GroupLine(retDevID, GroupIndex, 63, &point, &PMaxVel);
                    ret = WaitCmdReached(retDevID, GroupIndex, retGroupAxisCount);

                    ret = NMC_GroupSetParamF64(retDevID, GroupIndex, 0x32, 0, 1000);
                    ret = NMC_GroupSetParamF64(retDevID, GroupIndex, 0x33, 0, 1000);

                    point = { 109,409,300,89,0,-180 }; 
                    NMC_GroupPtpCartAll(retDevID, GroupIndex, 63, &point);
                    ret = WaitCmdReached(retDevID, GroupIndex, retGroupAxisCount);

                    ret = NMC_GroupSetParamF64(retDevID, GroupIndex, 0x32, 0, 1000); 
                    ret = NMC_GroupSetParamF64(retDevID, GroupIndex, 0x33, 0, 1000);

                    point = { 109,409,105,89,0,-180 };
                    NMC_GroupPtpCartAll(retDevID, GroupIndex, 63, &point);
                    ret = WaitCmdReached(retDevID, GroupIndex, retGroupAxisCount);
                    
                    modbus_write_register(ctx, 0, 0);

                    ret = NMC_GroupSetParamF64(retDevID, GroupIndex, 0x32, 0, 1000); 
                    ret = NMC_GroupSetParamF64(retDevID, GroupIndex, 0x33, 0, 1000);
                    
                    point = { 109,409,300,89,0,-180 };
                    NMC_GroupPtpCartAll(retDevID, GroupIndex, 63, &point);
                    ret = WaitCmdReached(retDevID, GroupIndex, retGroupAxisCount);

                    ret = NMC_GroupPtpAcsAll(retDevID, GroupIndex, 63, &standbypos);
                    ret = WaitCmdReached(retDevID, GroupIndex, retGroupAxisCount);

				}
				else
				{
					std::cout << "Not get the message from Pi ! " << endl;
				}
			}
			else
			{
				cerr << "Lost connect to server, Err #" << WSAGetLastError() << endl; 
			}
			std::cout << "stop? y/n" << endl; // Must stop by this proceduce, otherwise robot arm might fall down immediately.
			
			std::cin >> ans;
			if (ans == "y")
			{
				Output = "end";
				cout << "Send> " << Output << endl;

				ret = NMC_GroupSetParamI32(retDevID, GroupIndex, 0x36, 0, 2);
				ret = NMC_GroupSetParamI32(retDevID, GroupIndex, 0x36, 1, 3);
				ret = NMC_GroupSetParamF64(retDevID, GroupIndex, 0x36, 2, 100);
				ret = NMC_GroupSetParamF64(retDevID, GroupIndex, 0x32, 0, 300); 
				ret = NMC_GroupSetParamF64(retDevID, GroupIndex, 0x33, 0, 300); 
				ret = NMC_GroupSetParamF64(retDevID, GroupIndex, 0x34, 0, 100); 
				Pos_T point = { 454.5,0,755,89.6,-90,-90 };
				NMC_GroupLine(retDevID, GroupIndex, 63, &point, &PMaxVel);
				Sleep(10);
				isstoped = true;
			}

		}
		ret = NMC_GroupSetParamI32(retDevID, GroupIndex, 0x30, 0, 0);
		if (ret != 0) {
			cout << "ERROR! NMC_GroupSetParamI32:" << NMC_GetErrorDescription(ret, NULL, 0) << endl;
			CloseDevice(retDevID, sleepTime, retGroupCount);
		}
		ret = NMC_GroupPtpAcsAll(retDevID, GroupIndex, 63, &HomePos);
		if (ret != 0) {
			cout << "ERROR! NMC_GroupPtpAcsAll:" << NMC_GetErrorDescription(ret, NULL, 0) << endl;
			CloseDevice(retDevID, sleepTime, retGroupCount);
		}
		ret = WaitCmdReached(retDevID, GroupIndex, retGroupAxisCount);
		if (ret != 0)
		{
			CloseDevice(retDevID, sleepTime, retGroupCount);
		}
		SetBreak(Break_On, retDevID, sleepTime, retGroupCount);
		Sleep(10);
		CloseDevice(retDevID, sleepTime, retGroupCount);
		modbus_close(ctx);

	}
	return 0;
}

vector<int> spilt_string(string str)
{
	vector<int> locs;
	for (int i = 0; i < str.length(); i++)
	{
		if (str[i] == ',')
		{
			locs.push_back(i);
		}
	}
	vector<int> spilt_int;
	for (int i = 0; i < locs.size(); i++)
	{
		string temp;
		if (i == 0)
		{
			temp.append(str, 0, locs[i]);
			spilt_int.push_back(stoi(temp));
		}
		else
		{
			temp.append(str, locs[i - 1] + 1, locs[i]);
			spilt_int.push_back(stoi(temp));
		}
	}
	string temp;
	temp.append(str, locs[locs.size() - 1] + 1, str.length() - 1);
	spilt_int.push_back(stoi(temp));
	return spilt_int;
}
void thread_modbus(modbus_t * mb)
{
	Sleep(500);
	modbus_write_register(mb, 0, 2400);
}
void CloseDevice(I32_T retDevID, U32_T sleepTime, I32_T retGroupCount) {

	//=================================================	
	//              Shutdown device
	//=================================================

	RTN_ERR ret = 0;
	I32_T retState = 0;


	ret = NMC_DeviceDisableAll(retDevID);
	if (ret != 0) {
		printf("ERROR! NMC_DeviceDisableAll: (%d)%s.\n", ret, NMC_GetErrorDescription(ret, NULL, 0));
	}
	else {
		printf("\nDevice disable all succeed.\n");
	}


	//sleep
	Sleep(sleepTime);

	//check group state
	for (I32_T i = 0; i < retGroupCount; i++)
	{
		ret = NMC_GroupGetState(retDevID, 0, &retState);
		if (ret != 0)
		{
			printf("ERROR! NMC_GroupGetState(group index %d): (%d)%s.\n", i, ret, NMC_GetErrorDescription(ret, NULL, 0));
			CloseDevice(retDevID, sleepTime, retGroupCount);
		}

		if (retState != NMC_GROUP_STATE_DISABLE)
		{
			printf("ERROR! Group disable failed.(group index %d)\n", i);
			CloseDevice(retDevID, sleepTime, retGroupCount);
		}
		else
			printf("Group disable succeed.(group index %d)\n", i);
	}

	ret = NMC_DeviceShutdown(retDevID);
	if (ret != 0) {
		printf("ERROR! NMC_DeviceShutdown: (%d)%s.\n", ret, NMC_GetErrorDescription(ret, NULL, 0));
	}
	else {
		printf("\nDevice shutdown succeed.\n");
	}
	system("pause");
	exit(0);
}

void OpenDevice(I32_T &devType, I32_T &devIndex, I32_T &retDevID, I32_T sleepTime, I32_T &retGroupCount, I32_T &retGroupAxisCount) {

	//=================================================
	//              Device open up
	//=================================================

	RTN_ERR ret = 0;
	I32_T retDevState = 0;
	I32_T retSingleAxisCount = 0;

	cout << "Start to openup device..." << endl;

  	ret = NMC_DeviceOpenUp(devType, devIndex, &retDevID);
	if (ret != 0)
	{
		printf("ERROR! NMC_DeviceOpenUp: (%d)%s.\n", ret, NMC_GetErrorDescription(ret, NULL, 0));
		CloseDevice(retDevID, sleepTime, retGroupCount);
	}
	else {
		printf("\nDevice open up succeed, device ID: %d.\n", retDevID);
	}

	ret = NMC_DeviceResetStateAll(retDevID);
	if (ret != NMC_AXIS_STATE_DISABLE)
	{
		printf("ERROR! NMC_DeviceResetStateAll, device ID: %d. (err code: %d)\n", retDevID, ret);
		CloseDevice(retDevID, sleepTime, retGroupCount);
	}
	else {
		printf("Device ID %d: Device Reset State All success.\n", retDevID);
	}

	//get device state
	ret = NMC_DeviceGetState(retDevID, &retDevState);
	if (retDevState != NMC_DEVICE_STATE_OPERATION)
	{
		printf("ERROR! Device open up failed, device ID: %d.\n", retDevID);
		CloseDevice(retDevID, sleepTime, retGroupCount);
	}
	else {
		printf("Device ID %d: state is OPERATION.\n", retDevID);
	}

	//=================================================
	//              Step 2 : Get device infomation
	//=================================================
	//Get amount of single axis
	ret = NMC_DeviceGetAxisCount(retDevID, &retSingleAxisCount);
	if (ret != 0)
	{
		printf("ERROR! NMC_DeviceGetAxisCount: (%d)%s.\n", ret, NMC_GetErrorDescription(ret, NULL, 0));
		CloseDevice(retDevID, sleepTime, retGroupCount);
	}
	else
		printf("\nGet single axis count succeed, device has %d single axis.\n", retSingleAxisCount);

	//Get amount of GROUP
	ret = NMC_DeviceGetGroupCount(retDevID, &retGroupCount);
	if (ret != 0)
	{
		printf("ERROR! NMC_DeviceGetGroupCount: (%d)%s.\n", ret, NMC_GetErrorDescription(ret, NULL, 0));
		CloseDevice(retDevID, sleepTime, retGroupCount);
	}
	else
		printf("Get group count succeed, device has %d group.\n", retGroupCount);

	if (retGroupCount == 0)
	{
		printf("ERROR! The NCF has no group!");
		CloseDevice(retDevID, sleepTime, retGroupCount);
	}

	//Get amount of AXIS of each GROUP
	for (I32_T i = 0; i < retGroupCount; i++)
	{
		ret = NMC_DeviceGetGroupAxisCount(retDevID, i, &retGroupAxisCount);
		if (ret != 0)
		{
			printf("ERROR! NMC_DeviceGetGroupAxisCount: (%d)%s.\n", ret, NMC_GetErrorDescription(ret, NULL, 0));
			CloseDevice(retDevID, sleepTime, retGroupCount);
		}
		else
			printf("Get group axis count succeed, group index %d has %d axis.\n", i, retGroupAxisCount);
	}

	printf("\nReady to reset all drives in device...\n");


}

int WaitGroupStandStill(I32_T DeviceID, I32_T GroupIndex) {
	RTN_ERR     ret = 0;
	I32_T       groupStatus = 0;
	U32_T       count = 0;
	U32_T       timeOut = (20);


	do
	{
		printf("** Waiting group enabled.\n");
		ret = NMC_GroupGetStatus(DeviceID, GroupIndex, &groupStatus);
		if (ret != 0)
		{
			printf("ERROR! NMC_GroupGetStatus: (%d)%s.\n", ret, NMC_GetErrorDescription(ret, NULL, 0));
			break;
		}

		// break condition
		if (count > timeOut)
		{
			printf("** Timeout, stop waiting and continue...\n");
			return -1;
		}
		else if ((groupStatus & GROUP_STANDSTILL) != false)
		{
			printf("** The Group has been enabled and continues...\n");
			return 0;
		}


		Sleep(SLEEP_TIME);
		count++;
	} while (true);

	return 0;
}

int WaitCmdReached(I32_T DeviceID, I32_T GroupIndex, I32_T AxisCount) {
	RTN_ERR     ret = 0;
	I32_T       groupStatus = 0;
	U32_T       count = 0;
	Pos_T       groupAxisActPostion = { 0 };
	U32_T       timeOut = 10000;
	do
	{
		ret = NMC_GroupGetStatus(DeviceID, GroupIndex, &groupStatus);
		if (ret != 0)
		{
			printf("ERROR! NMC_GroupGetStatus: (%d)%s.\n", ret, NMC_GetErrorDescription(ret, NULL, 0));
			break;
		}

		//0:MCS, 2:ACS
		NMC_GroupGetActualPos(DeviceID, GroupIndex, 0, &groupAxisActPostion);
		if (ret != 0)
			printf("ERROR! NMC_GroupGetActualPosAcs: (%d)%s.\n", ret, NMC_GetErrorDescription(ret, NULL, 0));

		// print actual position
		for (int idxAxisPos = 0; idxAxisPos < AxisCount; idxAxisPos++)
		{
			printf("A%d: %.3f, ", idxAxisPos + 1, groupAxisActPostion.pos[idxAxisPos]);
		}
		printf("\n");

		// break condition
		if (count > timeOut)
		{
			printf("** Timeout, stop waiting and continue...\n");
			return -1;
		}
		else if ((groupStatus & GROUP_TARGET_REACHED) != false)
		{
			printf("** The command has been reached and continues...\n");
			return 0;
		}

		Sleep(SLEEP_TIME * 4);
		count++;
	} while (true);

	return ret;
}

void SafyOn(I32_T retDevID, U32_T sleepTime, I32_T retGroupCount) {
	//=================================================
	//    Step 3 : (for Hiwin RA605) Trigger safety rely
	//=================================================

	RTN_ERR ret = 0;
	U32_T   firstIoSet = 1;
	I32_T   dioValue = 0;
	U32_T   sizeByteDIO = 1;


	//get current DO value
	ret = NMC_ReadOutputMemory(retDevID, firstIoSet, sizeByteDIO, &dioValue);
	if (ret != 0)
	{
		printf("ERROR! NMC_ReadOutputMemory: (%d)%s.\n", ret, NMC_GetErrorDescription(ret, NULL, 0));
		CloseDevice(retDevID, sleepTime, retGroupCount);
	}


	//output DO value include bit of trigger safety rely
	dioValue = dioValue | DIO_START_RESET;
	ret = NMC_WriteOutputMemory(retDevID, firstIoSet, sizeByteDIO, &dioValue);
	if (ret != 0)
	{
		printf("ERROR! NMC_WriteOutputMemory: (%d)%s.\n", ret, NMC_GetErrorDescription(ret, NULL, 0));
		CloseDevice(retDevID, sleepTime, retGroupCount);
	}

}

void ResetAlarm(I32_T retDevID, U32_T sleepTime, I32_T retGroupCount) {
	//=================================================
	//  Step 4 : Clean alarm of drives of each group
	//=================================================

	RTN_ERR ret = 0;
	I32_T retState = 0;


	for (I32_T i = 0; i < retGroupCount; i++)
	{
		ret = NMC_GroupResetDriveAlmAll(retDevID, i);
		if (ret != 0)
		{
			printf("ERROR! NMC_GroupResetDriveAlmAll: (%d)%s.\n", ret, NMC_GetErrorDescription(ret, NULL, 0));
			CloseDevice(retDevID, sleepTime, retGroupCount);
		}
	}

	//sleep
	Sleep(sleepTime);

	//check state
	for (I32_T i = 0; i < retGroupCount; i++)
	{
		ret = NMC_GroupGetState(retDevID, i, &retState);
		if (ret != 0)
		{
			printf("ERROR! NMC_GroupGetState(group index %d): (%d)%s.\n", i, ret, NMC_GetErrorDescription(ret, NULL, 0));
			CloseDevice(retDevID, sleepTime, retGroupCount);
		}

		if (retState != NMC_GROUP_STATE_DISABLE)
		{
			printf("ERROR! Group reset failed.(group index %d)\n", i);
			CloseDevice(retDevID, sleepTime, retGroupCount);
		}
		else {
			printf("Group reset succeed.(group index %d)\n", i);
		}
	}


	//=================================================
	//       Step 5 : Enable all groups
	//=================================================
	for (I32_T i = 0; i < retGroupCount; i++)
	{
		ret = NMC_DeviceEnableAll(retDevID);
		if (ret != 0)
		{
			printf("ERROR! NMC_DeviceEnableAll: (%d)%s.\n", ret, NMC_GetErrorDescription(ret, NULL, 0));
			CloseDevice(retDevID, sleepTime, retGroupCount);
		}
		else {
			printf("\nReady to enable all single axes and groups...\n");
		}
		ret = WaitGroupStandStill(retDevID, i);
		if (ret != 0)
		{
			CloseDevice(retDevID, sleepTime, retGroupCount);
		}
	}

	//check group state
	for (I32_T i = 0; i < retGroupCount; i++)
	{
		ret = NMC_GroupGetState(retDevID, i, &retState);
		if (ret != 0)
		{
			printf("ERROR! NMC_GroupGetState(group index %d): (%d)%s.\n", i, ret, NMC_GetErrorDescription(ret, NULL, 0));
			CloseDevice(retDevID, sleepTime, retGroupCount);
		}

		if (retState != NMC_GROUP_STATE_STAND_STILL)
		{
			printf("ERROR! Group enable failed.(group index %d) (err code : %d) \n", i, retState);
			CloseDevice(retDevID, sleepTime, retGroupCount);
		}
		else {
			printf("Group enable succeed.(group index %d)\n", i);
		}
	}

	//sleep
	Sleep(sleepTime);
}

void GoHome(I32_T retDevID, U32_T sleepTime, I32_T retGroupCount, I32_T GroupIndex, I32_T retGroupAxisCount) {

	RTN_ERR ret = 0;

	I32_T groupAxesIdxMask = 0;
	groupAxesIdxMask += NMC_GROUP_AXIS_MASK_X;
	groupAxesIdxMask += NMC_GROUP_AXIS_MASK_Y;
	groupAxesIdxMask += NMC_GROUP_AXIS_MASK_Z;
	groupAxesIdxMask += NMC_GROUP_AXIS_MASK_A;
	groupAxesIdxMask += NMC_GROUP_AXIS_MASK_B;
	groupAxesIdxMask += NMC_GROUP_AXIS_MASK_C;

	Sleep(sleepTime);


	ret = NMC_GroupAxesHomeDrive(retDevID, GroupIndex, groupAxesIdxMask);
	if (ret != 0) {
		cout << "ERROR! NMC_GroupAxesHomeDrive:" << NMC_GetErrorDescription(ret, NULL, 0) << endl;
		CloseDevice(retDevID, sleepTime, retGroupCount);
	}
	else {
		cout << endl;
		cout << "Go Home Success!!!" << endl;
	}

	ret = WaitCmdReached(retDevID, GroupIndex, retGroupAxisCount);
	if (ret != 0)
	{
		printf("ERROR! Wait Command Reached: (%d)%s.\n", ret, NMC_GetErrorDescription(ret, NULL, 0));
		CloseDevice(retDevID, sleepTime, retGroupCount);
	}
}

void GetPoseValue(I32_T retDevID, I32_T GroupIndex, I32_T retGroupCount, U32_T sleepTime) {
	//=================================================
	//       Step 6 : Get position information
	//=================================================

	RTN_ERR ret = 0;
	Pos_T cmdPosPcs = { 0 };

	//Group Get Command PosPcs
	ret = NMC_GroupGetActualPosAcs(retDevID, GroupIndex, &cmdPosPcs);
	if (ret != 0)
	{
		printf("ERROR! NMC_GroupGetActualPosAcs: (%d)%s.\n", ret, NMC_GetErrorDescription(ret, NULL, 0));
		CloseDevice(retDevID, sleepTime, retGroupCount);
	}
	else
	{
		printf("\nGroup NMC_GroupGetActualPosAcs succeed.(group index %d)\n", GroupIndex);
		printf("ActualPosAcs[0] = %f \n", cmdPosPcs.pos[0]);
		printf("ActualPosAcs[1] = %f \n", cmdPosPcs.pos[1]);
		printf("ActualPosAcs[2] = %f \n", cmdPosPcs.pos[2]);
		printf("ActualPosAcs[3] = %f \n", cmdPosPcs.pos[3]);
		printf("ActualPosAcs[4] = %f \n", cmdPosPcs.pos[4]);
		printf("ActualPosAcs[5] = %f \n", cmdPosPcs.pos[5]);
	}

	//Group Get Command PosPcs
	ret = NMC_GroupGetCommandPosAcs(retDevID, GroupIndex, &cmdPosPcs);
	if (ret != 0)
	{
		printf("ERROR! NMC_GroupGetCommandPosAcs: (%d)%s.\n", ret, NMC_GetErrorDescription(ret, NULL, 0));
		CloseDevice(retDevID, sleepTime, retGroupCount);
	}
	else
	{
		printf("\nGroup NMC_GroupGetCommandPosAcs succeed.(group index %d)\n", GroupIndex);
		printf("CommandPosAcs[0] = %f \n", cmdPosPcs.pos[0]);
		printf("CommandPosAcs[1] = %f \n", cmdPosPcs.pos[1]);
		printf("CommandPosAcs[2] = %f \n", cmdPosPcs.pos[2]);
		printf("CommandPosAcs[3] = %f \n", cmdPosPcs.pos[3]);
		printf("CommandPosAcs[4] = %f \n", cmdPosPcs.pos[4]);
		printf("CommandPosAcs[5] = %f \n", cmdPosPcs.pos[5]);
	}

	//sleep
	Sleep(sleepTime);

}

void SetBreak(BreakFlag inStaute, I32_T retDevID, U32_T sleepTime, I32_T retGroupCount) {
	//=================================================
	//    Set Brake
	//=================================================

	RTN_ERR ret = 0;
	U32_T   firstIoSet = 0;
	I32_T   dioValue = 0;
	U32_T   sizeByteDIO = 1;

	//get current DO value
	ret = NMC_ReadOutputMemory(retDevID, firstIoSet, sizeByteDIO, &dioValue);
	if (ret != 0)
	{
		printf("ERROR! NMC_ReadOutputMemory: (%d)%s.\n", ret, NMC_GetErrorDescription(ret, NULL, 0));
		CloseDevice(retDevID, sleepTime, retGroupCount);
	}

	//output DO value include bit of unbrake
	if (inStaute == Break_On) {
		dioValue = dioValue & ~DIO_UNBRAKE;
	}
	else if (inStaute == Break_Off) {
		dioValue = dioValue | DIO_UNBRAKE;
	}
	ret = NMC_WriteOutputMemory(retDevID, firstIoSet, sizeByteDIO, &dioValue);
	if (ret != 0)
	{
		printf("ERROR! NMC_WriteOutputMemory: (%d)%s.\n", ret, NMC_GetErrorDescription(ret, NULL, 0));
		CloseDevice(retDevID, sleepTime, retGroupCount);
	}

	Sleep(sleepTime);
}

void SetHome(Pos_T HomePos, I32_T retDevID, U32_T sleepTime, I32_T retGroupCount, I32_T GroupIndex, I32_T retGroupAxisCount) {

	RTN_ERR ret = 0;

	I32_T groupAxesIdxMask = 0;
	groupAxesIdxMask += NMC_GROUP_AXIS_MASK_X;
	groupAxesIdxMask += NMC_GROUP_AXIS_MASK_Y;
	groupAxesIdxMask += NMC_GROUP_AXIS_MASK_Z;
	groupAxesIdxMask += NMC_GROUP_AXIS_MASK_A;
	groupAxesIdxMask += NMC_GROUP_AXIS_MASK_B;
	groupAxesIdxMask += NMC_GROUP_AXIS_MASK_C;

	Sleep(sleepTime);

	ret = NMC_GroupSetHomePos(retDevID, GroupIndex, groupAxesIdxMask, &HomePos);
	if (ret != 0) {
		cout << "ERROR! NMC_GroupSetHomePos:" << NMC_GetErrorDescription(ret, NULL, 0) << endl;
		CloseDevice(retDevID, sleepTime, retGroupCount);
	}
	else {
		cout << endl;
		cout << "Set Home!!!" << endl;
		cout << "A1:" << HomePos.pos[0] << endl;
		cout << "A2:" << HomePos.pos[1] << endl;
		cout << "A3:" << HomePos.pos[2] << endl;
		cout << "A4:" << HomePos.pos[3] << endl;
		cout << "A5:" << HomePos.pos[4] << endl;
		cout << "A6:" << HomePos.pos[5] << endl;
		cout << "set Home complete..." << endl;
	}
}