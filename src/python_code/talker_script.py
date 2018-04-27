# -*- coding: utf-8 -*-
#Francisco Julián Huerta y Munive  A01323513
#Jorge Alberto Beauregard Braco    A01328439
#Tabatha Tabeli Acosta Pastrana    A01550280
#Begoña Montes Gómez               A01329896

#Fecha: 27 de Abril, 2018.

#Descripcion: Codigo del script para el arduino talker que espera lo que reporta el arduino y actúa de acuerdo a ello para realizar acciones comunicándose con el back end


#serial communication for arduino
import serial 
#delay utility
import time
#used for sending http requests with data read from the arduino
import requests
#used for parsing json response
import json

#define serial for macos or linux
ser = serial.Serial('/dev/cu.usbmodem1411',9600)
#ser = serial.Serial('/dev/ttyACM1',9600)

#define urls 
otherArduinoServer = "http://localhost:8080/"
#backEndServer = "http://localhost:8081/"
backEndServer = "http://47.254.65.201:8081/"

#stores the last rfid card scaned 
lastRFIDScanned= ""
print("Service waiting...")
print("Im the rfid/ldr arduino")

#skip flag 
skip = 0
while 1:
    #waits for the arduino to send something
    while ser.in_waiting == 0:
        pass
    time.sleep(0.1)
    string = ser.readline().rstrip()
    string = string.encode('utf8')
    print(string)
    #if the string contains card: we determine if the arduino wants us to delete, remove, beep or insert a card
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
    #if not, we determine the arduino wants us to send the rfid card
    else:
        print("Read some card")
        lastRFIDScanned = string
        url = backEndServer + "card"
        data = {
            'idRoom': 1,
            'idCard': string
        }

    #we make a request
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


