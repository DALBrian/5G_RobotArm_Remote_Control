import pyrebase
import cv2, time
import os
import socket


#firebase initialization
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
realtime = pyrebase.initialize_app(db_config)
db = realtime.database()
#firebase storage儲存照片空間的宣告
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

stor = pyrebase.initialize_app(stor_config)
storage = stor.storage()
upload_path = ''

if __name__ =='__main__':
#Industrial PC initialization
    print('wait for connection')
    HOST = "192.168.225.39"
    PORT = 8000
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind((HOST,PORT))
    server.listen(1)
    connection, address = server.accept()
    print(connection, address)
    
cap = cv2.VideoCapture(0)
#cap = cv2.VideoCapture(1, cv2.CAP_DSHOW) 
cap.set(3,1920)
cap.set(4,1080)
cap.set(cv2.CAP_PROP_EXPOSURE, -5)
N = 1

while True:
    recv = connection.recv(1024)[0:10]
    recv = recv.decode("ascii")
    #print(recv[0])
    ret, frame = cap.read()
    if recv[0] =='s':
        
        #taking shot
        t1 = time.time() # start taking pics
        print('start taking')
        #time.sleep(0.5)
        #ret, frame = cap.read()
              #save image   
        filename = str(N) + '.jpeg'
        cv2.imwrite("C:/Users/dal/Desktop/5gdemo/"+filename, frame)           
        
        if ret == 1:
            t2 = time.time()  #taking pics finish
            t3 = t2 - t1  #time of taking pics 
            #print('take fin')         

            #start uploading
            t4 = time.time() # start uploading
            path_cloud = os.path.join(upload_path, filename)
            path_local = os.path.join('C:\\','Users','dal','Desktop','5gdemo',filename)
            storage.child(path_cloud).put(path_local)
            t5 = time.time() #upload finish
            t6 = t5 - t4 # time of uploading
            
            #read info from database
            time.sleep(1.7)
            t7 = time.time() #get info
            #使用get只讀取一項目錄中的內容
            shape = db.child("0804test").child(str(N)).child("Shape").get() 
            #後面需加上.val()才會顯示str
            shape = str(shape.val())
            cx = db.child("0804test").child(str(N)).child("cx").get()
            cx = str(cx.val())
            cy = db.child("0804test").child(str(N)).child("cy").get()
            cy = str(cy.val())
            reliability = db.child("0804test").child(str(N)).child("reliability").get()
            reliability = str(reliability.val())
            arrival = db.child("0804test").child(str(N)).child("arrival").get()
            arrival = str(arrival.val())
            t8 = time.time() #get info finish
            t9 = t8 - t7
            information = cx + ',' + cy + ',' + reliability + ',' + arrival
            #print(information)
            ta = time.time()
            connection.send(bytes(information, encoding = "ascii"))
            tb = time.time()
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
        #print("cap break")
        time.sleep(2)
    
    else:
        print('recive info ',recv,' is unknown')
        
cap.release        
cv2.destroyAllWindows()
