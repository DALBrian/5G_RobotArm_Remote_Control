import pyrebase
import cv2, time
import os
import socket
'''
@brief: This program is used to take photos and upload to firebase storage.
@author: Small Brian
@date: 20210825
@version: V1.6; for 0826 demonstration usage.
@description: Run this porgram on Raspberry Pi 3B+ to take photo; also run image processing program on Colab to get the position of the object.
@dependencies: pyrebase, cv2, socket
'''
# Firebase configuration.
## Remove confidential information before uploading to github
db_config = {
   "apiKey": "XXX",
   "authDomain": "g-service-at-asia.firebaseapp.com",
   "databaseURL": "XXX",
   "serviceAccount": "C:/Users/dal/Desktop/5gdemo/newAccount.json",
   "projectId": "g-service-at-asia",
   "storageBucket": "g-service-at-asia.appspot.com",
   "messagingSenderId": "XXX",
   "appId": "1:XXX:web:XXX",
   "measurementId": "G-XXX"
 
}
stor_config = {
   "apiKey": "XXX",
   "authDomain": "g-service-at-asia.firebaseapp.com",
   "databaseURL": "XXX",
   "serviceAccount": "C:/Users/dal/Desktop/5gdemo/newAccount.json",
   "projectId": "g-service-at-asia",
   "storageBucket": "g-service-at-asia.appspot.com",
   "messagingSenderId": "XXX",
   "appId": "1:XXX:web:XXX",
   "measurementId": "G-XXX"
}
# Firebase storage initialization
realtime = pyrebase.initialize_app(db_config)
db = realtime.database()
stor = pyrebase.initialize_app(stor_config)
storage = stor.storage()
upload_path = '' # set upload path to root directory

if __name__ =='__main__':
# Rasp Pi initialization
    print('Waiting for connection')
    HOST = "192.168.225.39" # Robot arm IP
    PORT = 8000 # Robot arm port
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind((HOST,PORT))
    server.listen(1)
    connection, address = server.accept()
    print(connection, address)
    
# Camera initialization
cap = cv2.VideoCapture(0)
cap.set(3,1920)
cap.set(4,1080)
cap.set(cv2.CAP_PROP_EXPOSURE, -5)
N = 1
# Main loop
while True:
    recv = connection.recv(1024)[0:10]
    recv = recv.decode("ascii")
    ret, frame = cap.read()
    # Take photo when robot arm send 's'
    if recv[0] =='s':
        # Taking shot
        t1 = time.time() 
        print('Start taking image.')
        #ret, frame = cap.read() # For debug
        # Save image   # For debug
        filename = str(N) + '.jpeg'
        cv2.imwrite("/home/5gdemo/"+filename, frame)           
        
        if ret == 1:
            t2 = time.time()  # Taking pics finish
            t3 = t2 - t1  # Time of taking pics 
            print('Image taken.')         

            # Start uploading
            t4 = time.time() 
            path_cloud = os.path.join(upload_path, filename)
            path_local = os.path.join('home', '5gdemo', filename)
            storage.child(path_cloud).put(path_local)
            t5 = time.time() #upload finish
            t6 = t5 - t4 # time of uploading
            
            #read info from database
            time.sleep(1.7) # wait for image processing
            t7 = time.time()

            # Use "get()" to get the data from database
            shape = db.child("0804test").child(str(N)).child("Shape").get() 
            # Use "val()" to get the value of the data
            shape = str(shape.val())
            cx = db.child("0804test").child(str(N)).child("cx").get()
            cx = str(cx.val())
            cy = db.child("0804test").child(str(N)).child("cy").get()
            cy = str(cy.val())
            reliability = db.child("0804test").child(str(N)).child("reliability").get()
            reliability = str(reliability.val())
            arrival = db.child("0804test").child(str(N)).child("arrival").get()
            arrival = str(arrival.val())
            t8 = time.time() # Download from database finish
            t9 = t8 - t7
            information = cx + ',' + cy + ',' + reliability + ',' + arrival
            #print(information) # for debug
            ta = time.time() # TCP/IP send time
            connection.send(bytes(information, encoding = "ascii")) # Send info to robot arm
            tb = time.time() # TCP/IP send finish
            tc = tb-ta
            timeused = {
                'take photo' : t3,
                'upload photo' : t6,
                'read data' : t9,
                'sendto' : tc
                       }
            N += 1
            print('time consume ', timeused)
            
        else:
            print('unable to take photo')
    
    elif recv[0] == 'e':
        #print("cap break") # for debug
        time.sleep(2)
    
    else:
        print('recive info ',recv,' is unknown')
        