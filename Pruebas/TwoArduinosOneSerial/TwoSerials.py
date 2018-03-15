# -*- coding: utf-8 -*-
import serial 
import time
import requests
import json


esplora = serial.Serial('/dev/cu.usbmodem1421',9600)
uno = serial.Serial('/dev/cu.usbmodem1411',9600)

#ser = serial.Serial('/dev/ttyACM0',9600)

import sys
reload(sys)  
sys.setdefaultencoding('utf8')

while 1:
    string = raw_input()
    if string == "1":
        uno.write("t")
    if string == "2":
        esplora.write("t")
    if string == "3":
        uno.write("t")
        esplora.write("t")
    if string == "4":
        uno.write("f")
        esplora.write("f")
