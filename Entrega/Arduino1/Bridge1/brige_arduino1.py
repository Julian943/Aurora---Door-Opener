# -*- coding: utf-8 -*-
#serial communication for arduino
import serial 
#delay utility
import time
#used for sending http requests with data read from the arduino
import requests
#used for parsing json response
import json


#ser = serial.Serial('/dev/cu.usbmodem1421',9600)
ser = serial.Serial('/dev/cu.usbmodem1411',9600)

#ser = serial.Serial('/dev/ttyACM0',9600)
 
print("Service waiting...")

while 1:
    while ser.in_waiting == 0:
        pass
    string = ser.readline().rstrip()
    print(string)
    if "Card" in string:
        if "removed" in string:
            url =  "http://localhost:8081/rm"
        elif "inserted" in string:
            url =  "http://localhost:8081/in"
        else:
            pass
    else:
        url = "http://localhost:8081/card?id=" + string
        
    response = requests.get(url)
    data = json.loads(response.text)

    if "Card" not in string:
        result = data['response']
        if result == 'true':
            ser.write("t")
        else:
            ser.write("f")


