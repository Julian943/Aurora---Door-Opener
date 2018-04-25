# -*- coding: utf-8 -*-
#serial communication for arduino
import serial 
#delay utility
import time
#used for sending http requests with data read from the arduino
import requests
#used for parsing json response
import json


#ser = serial.Serial('/dev/ttyACM1',9600)
ser = serial.Serial('/dev/cu.usbmodem1411',9600)

otherArduinoServer = "http://localhost:8080/"
#backEndServer = "http://localhost:8081/"
backEndServer = "http://47.254.65.201:8081/"

lastRFIDScanned= ""
print("Service waiting...")
print("Soy el del rfid/lcd")
skip = 0
while 1:
    while ser.in_waiting == 0:
        pass
    time.sleep(0.1)
    string = ser.readline().rstrip()
    string = string.encode('utf8')

    print(string)
    if "Card" in string:
        if "delete" in string:
            requests.get(otherArduinoServer + "stop")
            url =  backEndServer + "delete"
            data = {
                'idRoom': 1,
            }
        elif "removed" in string:
            requests.get(otherArduinoServer + "stop")
            url =  backEndServer + "rm"
            data = {
                'idRoom': 1,
                'idCard': lastRFIDScanned
            }
        else:
            skip = 1
            if  "beeps" in string:
                requests.get(otherArduinoServer + "beep")
            elif "inserted" in string:
                requests.get(otherArduinoServer + "stop")
    else:
        print("Read some card")
        lastRFIDScanned = string
        url = backEndServer + "card"
        data = {
            'idRoom': 1,
            'idCard': string
        }

    if skip == 0:
        try:
            print("Making response")
            response = requests.post(url,data)
            print("Responded!")
            responseData = json.loads(response.text)
        except Exception:
            print("Something went wrong with the connection or request")
            pass
    else:
        skip = 0
    

    print("Response back")
    if "Card" not in string:
        result = str(responseData['response'])
        print(result)
        if result == "True":
            requests.get(otherArduinoServer + "setTimer?time=1080")
            ser.write("t")
            print("True!")
        else:
            requests.get(otherArduinoServer + "noPass")
            ser.write("f")
            print("False!")


