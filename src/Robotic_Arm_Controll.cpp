#include <mutex>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
//#include "camfun.h"
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


//#define WINSOCK_DEPRECATED_NO_WARNINGS 0
#pragma comment(lib, "ws2_32.lib") 
#pragma warning(disable:4996)

mutex Mutex;

//using namespace cv;
using namespace std;



//#define fun 2;
//char key = ' ';

//Mat src2, src3;
//
//
//
//bool compX(Point x0, Point x1)
//{
//	return x0.x < x1.x;
//}
//
//bool compY(Point y0, Point y1)
//{
//	return y0.y < y1.y;
//}

enum {
	TCP,
	TCP_PI,
	RTU
};



//void findcuppoint(vector<Point> &pts, Mat src)
//{
//	Mat sizechange;
//	Mat hsv;
//	resize(src, sizechange, Size(640, 480));
//	cvtColor(sizechange, hsv, CV_BGR2HSV);
//	Mat change;
//	inRange(hsv, Scalar(0, 0, 180), Scalar(255, 255, 255), change);
//	Mat dst3;
//	morphologyEx(change, dst3, MORPH_CLOSE, Mat());
//
//	vector<vector<Point>> contours;
//	vector<Vec4i> hireachy;
//	findContours(dst3, contours, hireachy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
//
//	Point circle_center, other_circle;
//	Mat drawing;
//	Mat a = drawing.clone(), b = drawing.clone();
//	drawing = Mat::zeros(dst3.size(), CV_8UC3);
//	double area;
//	Rect rect;
//	int x = 0, y = 0;
//	//Point ori, oriL;
//	for (auto i = 0; i < contours.size(); i++)
//	{
//		area = contourArea(contours[i]);
//		rect = boundingRect(contours[i]);
//		float ratio = float(rect.width) / float(rect.height);
//		printf("圖面積:%f\n", area);
//		//drawContours(a, contours, 0, Scalar(0, 0, 225), -1, 8, hireachy);
//		//drawContours(b, contours, 1, Scalar(0, 0, 225), -1, 8, hireachy);
//		x = rect.x + rect.width / 2;
//		y = rect.y + rect.height / 2;
//
//		if (area >= 20000)
//		{
//			circle_center = Point(x, y);
//			cout << "圓心:" << "(" << circle_center.x << " " << "," << circle_center.y << ")" << endl;
//			pts.push_back(Point(x, y));
//		}
//		else
//		{
//			other_circle = Point(x, y);
//		}
//	}
//
//}
//
//void findballpoint(vector<Point>&pts, Mat src)
//{
//	Mat hsv1;
//	Mat sizechange;
//
//	resize(src, sizechange, Size(640, 480));
//	cvtColor(sizechange, hsv1, CV_BGR2HSV);
//	Mat change1;
//	inRange(hsv1, Scalar(0, 0, 180), Scalar(255, 255, 255), change1);
//	Mat dst1;
//	morphologyEx(change1, dst1, MORPH_CLOSE, Mat());
//
//	vector<vector<Point>> contours;
//	vector<Vec4i> hireachy;
//	findContours(dst1, contours, hireachy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
//
//	Point circle_center, other_circle;
//	Mat drawing;
//	Mat a = drawing.clone(), b = drawing.clone();
//	drawing = Mat::zeros(dst1.size(), CV_8UC3);
//
//	double area;
//	Rect rect;
//	int x = 0, y = 0;
//	//Point ori, oriL;
//	for (auto i = 0; i < contours.size(); i++)
//	{
//		area = contourArea(contours[i]);
//		rect = boundingRect(contours[i]);
//		float ratio = float(rect.width) / float(rect.height);
//		printf("圖面積:%f\n", area);
//		//drawContours(a, contours, 0, Scalar(0, 0, 225), -1, 8, hireachy);
//		//drawContours(b, contours, 1, Scalar(0, 0, 225), -1, 8, hireachy);
//		x = rect.x + rect.width / 2;
//		y = rect.y + rect.height / 2;
//
//		if (area >= 10000 && area <= 20000)
//		{
//			circle_center = Point(x, y);
//			cout << "圓心:" << "(" << circle_center.x << " " << "," << circle_center.y << ")" << endl;
//			pts.push_back(Point(x, y));
//		}
//		else
//		{
//			other_circle = Point(x, y);
//		}
//	}
//}
//
//void cameraon(Mat &src)
//{
//	CamFun capbasecam;
//	capbasecam.StreamUrl = "0";
//	capbasecam.CamType = 0;
//	capbasecam.CameraArea = Rect(0, 0, 4384, 3288);
//	capbasecam.Init();
//	while (true) 
//	{
//		Mat Frame;
//		capbasecam.SingleFrame();
//		Frame = capbasecam.GetFrame();
//		Mat SmallFrame;
//		resize(Frame, src, Size(640, 480));
//		imshow("camera", src);
//		imwrite("cam.jpg", src);
//	}
//	
//
//}

//void modbusconnect()
//{
//	ctx = modbus_new_rtu("COM8", 9600, 'N', 8, 1);
//	if (ctx == NULL)
//	{
//		fprintf(stderr, "modbus doesn't control");
//		modbus_close(ctx);
//	}
//	else {
//		cout << "modbus connected!" << endl;
//	}
//
//	modbus_set_debug(ctx, 1);
//	modbus_set_slave(ctx, 1);
//	modbus_connect(ctx);
//	uint16_t reg[64];
//
//	int num = modbus_read_registers(ctx, 0, 5, reg);
//	if (num != 5)
//	{
//		fprintf(stderr, "failed to read:", modbus_strerror(errno));
//	}
//	modbus_write_register(ctx, 0, 400);
//}

int main()
{
	//socket client----------------------------------
	string ipAddress = "127.0.0.1";			// IP Address of the server
	//string ipAddress = "192.168.1.91";			// IP Address of the server
	int port = 4000;
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		cerr << "Can't start Winsock, Err #" << wsResult << endl;
	}
	// Create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		cerr << "Can't create socket, Err #" << WSAGetLastError() << endl;
		WSACleanup();
	}
	// Fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);
	// Connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		cerr << "Can't connect to server, Err #" << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
	}
	//-----------------------------------------------------------------------

	modbus_t *ctx;
	modbus_mapping_t *mb_mapping;

	//open robot
	RTN_ERR ret = 0;
	I32_T   devIndex = 0;
	I32_T   retDevID = 0;
	I32_T   retGroupCount = 0;
	I32_T   retGroupAxisCount = 0;
	I32_T   GroupIndex = 0;
	F64_T   paravalue = 0;
	int tg = 0;
	int movev = 20;

	fstream fp;

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

	Pos_T HomePos = { 0,90,0,0,0,0 };

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

	Pos_T standbypos = {-69.44,53.662,-41.503,0,-12.159,-69.444 };
	//world 55 300 200 0 90 180
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
	//open modbus

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
	
	//------------------------------socket----------------------
	char buf[256];
	string Output;



	
	//open camera
	
	/*CamFun capbasecam;
	capbasecam.StreamUrl = "0";
	capbasecam.CamType = 0;
	capbasecam.CameraArea = Rect(0, 0, 4384, 3288);
	capbasecam.Init();*/
	//Mat SmallFrame;
	//thread t1{ cameraon, ref(SmallFrame) };
	//t1.detach();

	//int ch;
	//VideoWriter writer;
	//Mat Frame;
	//	int i = 0;
	bool isstoped = true;
	while (isstoped)
	{
		Output = "11";
		int sendResult = send(sock, Output.c_str(), Output.size() + 1, 0);
		if (sendResult != SOCKET_ERROR)
		{
			std::cout << "Send> " << Output << endl;
			// Wait for response
			ZeroMemory(buf, 256);
			int bytesReceived = recv(sock, buf, 256, 0);
			std::cout << "recv: " << bytesReceived << endl;
			if (bytesReceived > 0)
			{
				string recive = string(buf, 0, bytesReceived);
				int loc;
				string n1, n2;
				vector<int> vect;
				for (int i = 0; i < recive.size(); i++)
				{
					if (recive[i] == ',')
					{
						loc = i;
					}
				}

				if (loc != 0)
				{


					n1.append(recive, 0, loc);
					n2.append(recive, loc + 1, recive.size() - loc - 1);
					int pos_x, pos_y;
					pos_x = stoi(n1);
					pos_y = stoi(n2);

					//Point pts = Point(pos_x, pos_y);
					float coordinateversion_x, coordinateversion_y, coordinaterobot_x, coordinaterobot_y;


					ret = NMC_GroupSetParamI32(retDevID, GroupIndex, 0x30, 0, 0);
					if (ret != 0) {
						cout << "ERROR! NMC_GroupSetParamI32:" << NMC_GetErrorDescription(ret, NULL, 0) << endl;
						CloseDevice(retDevID, sleepTime, retGroupCount);
					}
					ret = NMC_GroupSetParamF64(retDevID, GroupIndex, 0x32, 0, 40);
					//coordinateversion_x = pts.x;
					//coordinateversion_y = pts.y;
					coordinateversion_x = 229;
					coordinateversion_y = 313;
					//----------------------------------------------------------------------------------
					coordinaterobot_x = coordinateversion_x * 1.113 + 59.743;//robotcoordinate
					coordinaterobot_y = coordinateversion_y * -1.099 - 407.178;
					//---------------------------------------------------------------------------------
					coordinaterobot_x = coordinateversion_x * 0.337 + 59.743;//robotcoordinate
					coordinaterobot_y = coordinateversion_y * -0.338 - 407.178;

					Sleep(2000);
					Pos_T point = { coordinaterobot_x, coordinaterobot_y ,240,0,0,-180 };
					ret = NMC_GroupLine(retDevID, GroupIndex, 63, &point, NULL);
					if (ret != 0) {
						cout << "ERROR! NMC_Groupline:" << NMC_GetErrorDescription(ret, NULL, 0) << endl;

						break;
					}
					ret = WaitCmdReached(retDevID, GroupIndex, retGroupAxisCount);
					if (ret != 0)
					{
						CloseDevice(retDevID, sleepTime, retGroupCount);
						break;
					}
					Sleep(1000);

					point = { coordinaterobot_x, coordinaterobot_y ,213.304,0,0,-180 };
					NMC_GroupLine(retDevID, GroupIndex, 63, &point, NULL);
					ret = WaitCmdReached(retDevID, GroupIndex, retGroupAxisCount);
					modbus_write_register(ctx, 0, 1000);
					Sleep(500);
					modbus_write_register(ctx, 0, 400);
					Sleep(500);

					int current = 1000;
					modbus_write_register(ctx, 0, current);
					Sleep(2000);
					point = { coordinaterobot_x, coordinaterobot_y ,240,0,0,-180 };
					NMC_GroupLine(retDevID, GroupIndex, 63, &point, NULL);
					ret = WaitCmdReached(retDevID, GroupIndex, retGroupAxisCount);

					point = { 400,0,240,0,0,-180 };
					//jog 0 48.167 -26.678 0 -20.054 0
					NMC_GroupLine(retDevID, GroupIndex, 63, &point, NULL);
					ret = WaitCmdReached(retDevID, GroupIndex, retGroupAxisCount);
					Sleep(1000);

					point = { 400,0,100,0,0,-180 };
					NMC_GroupLine(retDevID, GroupIndex, 63, &point, NULL);
					ret = WaitCmdReached(retDevID, GroupIndex, retGroupAxisCount);
					Sleep(500);
					modbus_write_register(ctx, 0, 400);
					Sleep(2000);

					point = { 400,0,240,0,0,-180 };
					NMC_GroupLine(retDevID, GroupIndex, 63, &point, NULL);
					ret = WaitCmdReached(retDevID, GroupIndex, retGroupAxisCount);
					Sleep(1000);

					ret = NMC_GroupPtpAcsAll(retDevID, GroupIndex, 63, &standbypos);
					ret = WaitCmdReached(retDevID, GroupIndex, retGroupAxisCount);
				}
				else
				{
					closesocket(sock);
					WSACleanup();
					isstoped = false;
				}
			}
			else
			{
				std::cout << "Not get the receive ! " << endl;
			}
		}
		else
		{
			cerr << "Lost connect to server, Err #" << WSAGetLastError() << endl;
		}
		std::cout << "stop? y/n" << endl;
		string ans;
		cin >> ans;
		if (ans == "y")
		{
			Output = "22";
			int sendResult = send(sock, Output.c_str(), Output.size() + 1, 0);
			isstoped = false;
		}




		//float coordinateversion_x, coordinateversion_y, coordinaterobot_x, coordinaterobot_y;
		//vector<Point> pstcup;

		//vector<Point> pstball;
		/*capbasecam.SingleFrame();
		Frame = capbasecam.GetFrame();
		Mat SmallFrame;
		resize(Frame, SmallFrame, Size(640, 480));
		imshow("camera", SmallFrame);*/
		//		imwrite("cam.jpg", SmallFrame);

		//if (GetAsyncKeyState('B'))
		//{
		//	/*imwrite("cam.jpg", SmallFrame);*/
		//	src2 = imread("cam.jpg", CV_LOAD_IMAGE_UNCHANGED);
		//	findballpoint(pstball, src2);
		//	
		//	ret = NMC_GroupSetParamI32(retDevID, GroupIndex, 0x30, 0, 0);
		//	if (ret != 0) {
		//		cout << "ERROR! NMC_GroupSetParamI32:" << NMC_GetErrorDescription(ret, NULL, 0) << endl;
		//		CloseDevice(retDevID, sleepTime, retGroupCount);
		//	}
		//	ret = NMC_GroupSetParamF64(retDevID, GroupIndex, 0x32, 0, 40);
		//	coordinateversion_x = pstball[0].x;
		//	coordinateversion_y = pstball[0].y;
		//	coordinaterobot_x = coordinateversion_x * 0.41875 + 101;//robotcoordinate
		//	coordinaterobot_y = coordinateversion_y * 0.41667 + 359;
		//	Sleep(2000);
		//	Pos_T point = { coordinaterobot_x, coordinaterobot_y ,200,0,90,180 };
		//	ret = NMC_GroupLine(retDevID, GroupIndex, 63, &point, NULL);
		//	if (ret != 0) {
		//		cout << "ERROR! NMC_Groupline:" << NMC_GetErrorDescription(ret, NULL, 0) << endl;

		//		break;
		//	}
		//	ret = WaitCmdReached(retDevID, GroupIndex, retGroupAxisCount);
		//	if (ret != 0)
		//	{
		//		CloseDevice(retDevID, sleepTime, retGroupCount);
		//		break;
		//	}
		//	Sleep(1000);

		//	point = { coordinaterobot_x, coordinaterobot_y ,130,0,90,180 };
		//	NMC_GroupLine(retDevID, GroupIndex, 63, &point, NULL);
		//	ret = WaitCmdReached(retDevID, GroupIndex, retGroupAxisCount);
		//	modbus_write_register(ctx, 0, 1000);
		//	Sleep(500);
		//	modbus_write_register(ctx, 0, 400);
		//	Sleep(500);

		//	int current = 1000;
		//	modbus_write_register(ctx, 0, current);
		//	Sleep(2000);
		//	point = { coordinaterobot_x, coordinaterobot_y ,200,0,90,180 };
		//	NMC_GroupLine(retDevID, GroupIndex, 63, &point, NULL);
		//	ret = WaitCmdReached(retDevID, GroupIndex, retGroupAxisCount);

		//	point = { 400,0,200,0,90,180 };
		//	//jog 0 48.167 -26.678 0 -20.054 0
		//	NMC_GroupLine(retDevID, GroupIndex, 63, &point, NULL);
		//	ret = WaitCmdReached(retDevID, GroupIndex, retGroupAxisCount);
		//	Sleep(1000);

		//	point = { 400,0,100,0,90,180 };
		//	NMC_GroupLine(retDevID, GroupIndex, 63, &point, NULL);
		//	ret = WaitCmdReached(retDevID, GroupIndex, retGroupAxisCount);
		//	Sleep(500);
		//	modbus_write_register(ctx, 0, 400);
		//	Sleep(2000);

		//	point = { 400,0,200,0,90,180 };
		//	NMC_GroupLine(retDevID, GroupIndex, 63, &point, NULL);
		//	ret = WaitCmdReached(retDevID, GroupIndex, retGroupAxisCount);
		//	Sleep(1000);

		//	ret = NMC_GroupPtpAcsAll(retDevID, GroupIndex, 63, &standbypos);
		//	ret = WaitCmdReached(retDevID, GroupIndex, retGroupAxisCount);

		//}
		//if (GetAsyncKeyState('C'))
		//{
		//	
		//	src2 = imread("cam.jpg", CV_LOAD_IMAGE_UNCHANGED);
		//	//src3 = imread("D:\\cupandball\\4.jpg", CV_LOAD_IMAGE_UNCHANGED);
		//	findcuppoint(pstcup, src2);
		//	ret = NMC_GroupSetParamI32(retDevID, GroupIndex, 0x30, 0, 0);
		//	if (ret != 0) {
		//		cout << "ERROR! NMC_GroupSetParamI32:" << NMC_GetErrorDescription(ret, NULL, 0) << endl;
		//		CloseDevice(retDevID, sleepTime, retGroupCount);
		//	}
		//	ret = NMC_GroupSetParamF64(retDevID, GroupIndex, 0x32, 0, 40);

		//	coordinateversion_x = pstcup[0].x;
		//	coordinateversion_y = pstcup[0].y;
		//	coordinaterobot_x = coordinateversion_x * 0.41875 + 101;
		//	coordinaterobot_y = coordinateversion_y * 0.41667 + 358;
		//	Pos_T point = { coordinaterobot_x, coordinaterobot_y ,200,0,90,180 };
		//	Sleep(2000);
		//	ret = NMC_GroupLine(retDevID, GroupIndex, 63, &point, NULL);
		//	if (ret != 0) {
		//		cout << "ERROR! NMC_Groupline:" << NMC_GetErrorDescription(ret, NULL, 0) << endl;
		//		
		//		break;
		//	}
		//	ret = WaitCmdReached(retDevID, GroupIndex, retGroupAxisCount);
		//	if (ret != 0)
		//	{
		//		CloseDevice(retDevID, sleepTime, retGroupCount);
		//		break;
		//	}
		//	Sleep(1000);
		//	
		//	point = { coordinaterobot_x, coordinaterobot_y ,174,0,90,180 };
		//	NMC_GroupLine(retDevID, GroupIndex, 63, &point, NULL);
		//	ret = WaitCmdReached(retDevID, GroupIndex, retGroupAxisCount);
		//	modbus_write_register(ctx, 0, 1000);
		//	Sleep(500);
		//	modbus_write_register(ctx, 0, 400);
		//	Sleep(500);



		//	int current = 735;
		//	modbus_write_register(ctx, 0, current);
		//	Sleep(2000);
		//	point = { coordinaterobot_x, coordinaterobot_y ,200,0,90,180 };
		//	NMC_GroupLine(retDevID, GroupIndex, 63, &point, NULL);
		//	ret = WaitCmdReached(retDevID, GroupIndex, retGroupAxisCount);

		//	point = { 400,0,200,0,90,180 };
		//	//jog 0 48.167 -26.678 0 -20.054 0
		//	NMC_GroupLine(retDevID, GroupIndex, 63, &point, NULL);
		//	ret = WaitCmdReached(retDevID, GroupIndex, retGroupAxisCount);
		//	Sleep(1000);

		//	point = { 400,0,140,0,90,180 };
		//	NMC_GroupLine(retDevID, GroupIndex, 63, &point, NULL);
		//	ret = WaitCmdReached(retDevID, GroupIndex, retGroupAxisCount);
		//	Sleep(500);
		//	modbus_write_register(ctx, 0, 400);
		//	Sleep(2000);
		//	point = { 400,0,200,0,90,180 };
		//	NMC_GroupLine(retDevID, GroupIndex, 63, &point, NULL);
		//	ret = WaitCmdReached(retDevID, GroupIndex, retGroupAxisCount);
		//	Sleep(1000);
		//	
		//	ret = NMC_GroupPtpAcsAll(retDevID, GroupIndex, 63, &standbypos);
		//	ret = WaitCmdReached(retDevID, GroupIndex, retGroupAxisCount);

		//}
		if (GetAsyncKeyState(VK_UP)) {
			closesocket(sock);
			WSACleanup();
			cout << "End Keyin Event..." << endl;
			break;
		}
		/*switch (key)
		{

		case 'b':
		src2 = imread("D:\\cupandball\\4.jpg", CV_LOAD_IMAGE_UNCHANGED);
		findballpoint(pstcup, src2);
		coordinate_x = pstcup[0].x;
		coordinate_y = pstcup[0].y;

		break;
		case 'c':
		src3 = imread("D:\\cupandball\\4.jpg", CV_LOAD_IMAGE_UNCHANGED);
		findcuppoint(pstball, src3);
		break;
		case 'z':
		cout << "End Keyin Event..." << endl;
		break;
		}*/



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
	//testclass.join();

	return 0;

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

//void Set15DO(IO15 status, I32_T retDevID, U32_T sleepTime, I32_T retGroupCount, I32_T *do15value) {
//
//	RTN_ERR ret = 0;
//	U32_T   firstIoSet = 0;
//	I32_T   valueByte0 = 0;
//	U32_T   sizeByteDIO = 1;
//
//
//	ret = NMC_ReadOutputMemory(retDevID, 1, 1, &valueByte0);
//	if (ret != 0)
//	{
//		printf("ERROR! NMC_ReadOutputMemory: (%d)%s.\n", ret, NMC_GetErrorDescription(ret, NULL, 0));
//		CloseDevice(retDevID, sleepTime, retGroupCount);
//	}
//
//	//DO_On or DO_Off
//	if (status == IO_Off) {
//		valueByte0 = valueByte0 & ~(1 << 7);
//		Mutex.lock();
//		*do15value = (valueByte0 >> 7) & 1;//do15value=0
//		Mutex.unlock();
//	}
//	if (status == IO_On) {
//		valueByte0 = valueByte0 | (1 << 7);
//		Mutex.lock();
//		*do15value = (valueByte0 >> 7) | 1;////do15value=1
//		Mutex.unlock();
//	}
//	ret = NMC_WriteOutputMemory(retDevID, firstIoSet, sizeByteDIO, &valueByte0);
//	if (ret != 0)
//	{
//		printf("ERROR! NMC_WriteOutputMemory: (%d)%s.\n", ret, NMC_GetErrorDescription(ret, NULL, 0));
//		CloseDevice(retDevID, sleepTime, retGroupCount);
//	}
//
//	cout << "do15value = " << *do15value << endl;
//
//	Sleep(sleepTime);
//}

void detectface(string path, vector<vector<float> >& LIST) {
	float  val;
	string line;

	ifstream myfile(path);

	//vector<vector<float> > LIST;

	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			vector<float> v;

			istringstream IS(line);

			while (IS >> val)
			{
				v.push_back(val);
			}

			LIST.push_back(v);
		}

		myfile.close();
	}

	for (int i = 0; i < LIST.size(); i++)
	{
		for (int j = 0; j < LIST[i].size(); j++)
		{
			cout << LIST[i][j] << " ";
		}
		cout << endl;
	}
}

void TrackMove(I32_T retDevID, I32_T GroupIndex, U32_T sleepTime, I32_T retGroupCount, I32_T retGroupAxisCount, float *c_speed, float *dis, I32_T *do15value) {
	//軌跡移動
	RTN_ERR ret = 0;
	vector<vector<float> > LIST;
	detectface("C:/Users/Robot/Desktop/point_test.txt", LIST);

	int d_facenum = LIST[0][0];



	if (d_facenum == 2) {
		int point_num = d_facenum * 2;
		int path_num = point_num - 1;
		float path_dis = 800 / path_num;
		float path_time = path_dis / *c_speed;


		//relative move 
		float x = LIST[2][0] - LIST[1][0];
		float y = LIST[2][1] - LIST[1][1];
		float z = LIST[2][2] - LIST[1][2];

		Pos_T point_1 = { LIST[1][0],400,LIST[1][2],LIST[1][3],LIST[1][4],LIST[1][5] };
		Pos_T dis1 = { 0,-path_dis,0,0,0,0 };
		Pos_T dis2 = { x,-(path_dis - y),z,0,0,0 };
		float dis3 = sqrt(pow(dis2.pos[0], 2) + pow(dis2.pos[1], 2) + pow(dis2.pos[2], 2));
		float dis4 = sqrt(pow(point_1.pos[0], 2) + pow(point_1.pos[1], 2) + pow(point_1.pos[2], 2));
		float speed1 = dis4 / (200 / *c_speed);
		float speed2 = dis3 / path_time;

		ret = NMC_GroupSetParamF64(retDevID, GroupIndex, 0x32, 0, speed1);
		for (int j = 0; j < 6; j++) {
			ret = NMC_GroupAxSetParamF64(retDevID, GroupIndex, j, 0x32, 0, speed1);
		}

		ret = NMC_GroupPtpCartAll(retDevID, GroupIndex, 63, &point_1);//p1
		if (ret != 0) {
			cout << "ERROR! NMC_GroupPtpCartAll:" << NMC_GetErrorDescription(ret, NULL, 0) << endl;
			CloseDevice(retDevID, sleepTime, retGroupCount);
		}
		ret = WaitCmdReached(retDevID, GroupIndex, retGroupAxisCount);
		if (ret != 0)
		{
			CloseDevice(retDevID, sleepTime, retGroupCount);
		}

		//move by abs(0) or relative(1)
		ret = NMC_GroupSetParamI32(retDevID, GroupIndex, 0x30, 0, 1);

		ret = NMC_GroupSetParamF64(retDevID, GroupIndex, 0x32, 0, *c_speed);
		for (int j = 0; j < 6; j++) {
			ret = NMC_GroupAxSetParamF64(retDevID, GroupIndex, j, 0x32, 0, *c_speed);
		}

		ret = NMC_GroupLine(retDevID, GroupIndex, 63, &dis1, NULL);//p2
		if (ret != 0) {
			cout << "ERROR! NMC_GroupLine:" << NMC_GetErrorDescription(ret, NULL, 0) << endl;
			CloseDevice(retDevID, sleepTime, retGroupCount);
		}
		ret = WaitCmdReached(retDevID, GroupIndex, retGroupAxisCount);
		if (ret != 0)
		{
			CloseDevice(retDevID, sleepTime, retGroupCount);
		}

		ret = NMC_GroupSetParamF64(retDevID, GroupIndex, 0x32, 0, speed2);
		for (int j = 0; j < 6; j++) {
			ret = NMC_GroupAxSetParamF64(retDevID, GroupIndex, j, 0x32, 0, speed2);
		}

		ret = NMC_GroupLine(retDevID, GroupIndex, 63, &dis2, NULL);//p3
		if (ret != 0) {
			cout << "ERROR! NMC_GroupLine:" << NMC_GetErrorDescription(ret, NULL, 0) << endl;
			CloseDevice(retDevID, sleepTime, retGroupCount);
		}
		ret = WaitCmdReached(retDevID, GroupIndex, retGroupAxisCount);
		if (ret != 0)
		{
			CloseDevice(retDevID, sleepTime, retGroupCount);
		}

		ret = NMC_GroupSetParamF64(retDevID, GroupIndex, 0x32, 0, *c_speed);
		for (int j = 0; j < 6; j++) {
			ret = NMC_GroupAxSetParamF64(retDevID, GroupIndex, j, 0x32, 0, *c_speed);
		}

		ret = NMC_GroupLine(retDevID, GroupIndex, 63, &dis1, NULL);//p4
		if (ret != 0) {
			cout << "ERROR! NMC_GroupLine:" << NMC_GetErrorDescription(ret, NULL, 0) << endl;
			CloseDevice(retDevID, sleepTime, retGroupCount);
		}
		ret = WaitCmdReached(retDevID, GroupIndex, retGroupAxisCount);
		if (ret != 0)
		{
			CloseDevice(retDevID, sleepTime, retGroupCount);
		}

		//move by abs(0) or relative(1)
		ret = NMC_GroupSetParamI32(retDevID, GroupIndex, 0x30, 0, 0);

		//ret = NMC_GroupSetParamI32(retDevID, GroupIndex, 0x36, 0, 0);

		Pos_T HomePos = { 0,90,0,0,0,0 };
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
		//Set15DO(IO_Off, retDevID, sleepTime, retGroupCount, do15value);
	}



}

void adaptspeed(float *c_speed, I32_T retDevID, U32_T sleepTime, I32_T retGroupCount) {
	//適應速度
	RTN_ERR ret;
	I32_T   GroupIndex = 0;

	ret = NMC_GroupSetParamF64(retDevID, GroupIndex, 0x32, 0, *c_speed);//this parameter is for line move
																		//for (int j = 0; j < 6; j++) { //this parameter is for P2P
																		//	ret = NMC_GroupAxSetParamF64(retDevID, GroupIndex, j, 0x32, 0, robotspeed);
																		//}
	cout << "set cart_speed to " << *c_speed << endl;
}

