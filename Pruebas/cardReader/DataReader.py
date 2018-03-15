# -*- coding: utf-8 -*-
#serial communication for arduino
import serial 
#delay utility
import time
#used for sending http requests with data read from the arduino
import requests
#used for parsing json response
import json


ser = serial.Serial('/dev/cu.usbmodem1421',9600)
#ser = serial.Serial('/dev/cu.usbmodem1411',9600)

#ser = serial.Serial('/dev/ttyACM0',9600)
 



import sys
reload(sys)  
sys.setdefaultencoding('utf8')


while 1:
    while ser.in_waiting == 0:
        pass
    string = ser.readline().rstrip()
    url = "http://localhost:8080/?id=" + string
    response = requests.get(url)
    data = json.loads(response.text)
    print(data)
    valid = data['response']
    print("Arduno wrote: \"" + string + "\"")
    if valid == "true":
        print("true!")
        ser.write("t")
    else:
        print("false!")
        ser.write("f")

