# -*- coding: utf-8 -*-
#serial communication for arduino
import serial 
#delay utility
import time
#used for sending http requests with data read from the arduino
import requests
#used for parsing json response
import json


ser = serial.Serial('/dev/cu.usbmodem1411',9600)

otherArduinoServer = "http://localhost:8080/"
#backEndServer = "http://localhost:8081/"
backEndServer = "http://47.254.65.201:8081/"
#ser = serial.Serial('/dev/ttyACM0',9600)
 
print("Service waiting...")
print("Soy el del rfid/lcd")

while 1:
    while ser.in_waiting == 0:
        pass
    time.sleep(0.1)
    string = ser.readline().rstrip()
    print(string)
    if "Card" in string:
        if "delete" in string:
            requests.get(otherArduinoServer + "stop")
            url =  backEndServer + "rm"
            data = {
                'idRoom': 1,
            }
        else:
            if  "beeps" in string:
                requests.get(otherArduinoServer + "beep")
            elif "removed" in string:
                requests.get(otherArduinoServer + "stopAndResetTimer")
            elif "inserted" in string:
                requests.get(otherArduinoServer + "stop")
            pass
    else:
        print("Read some shit")
        url = backEndServer + "card"
        data = {
            'idRoom': 1,
            'idCard': string
        }

    print("Making response")
    response = requests.post(url,data)
    print("Hola")
    responseData = json.loads(response.text)

    print("Response back")
    if "Card" not in string:
        result = responseData['response']
        if result == 'true':
            requests.get(otherArduinoServer + "setTimer")
            ser.write("t")
            print("True!")
        else:
            requests.get(otherArduinoServer + "noPass")
            ser.write("f")
            print("False!")


