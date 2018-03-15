# -*- coding: utf-8 -*-
#Flask app, jsonify jsonifies a response, request checks requests details
from flask import Flask, jsonify, request
import serial

import sys
reload(sys)  
sys.setdefaultencoding('utf8')

app = Flask(__name__)
ser = serial.Serial('/dev/cu.usbmodem1421',9600)
print("Serial opened")

@app.route('/')
def hello_world():
    if request.method == 'GET':
        return jsonify(response="Hola mundo")
    else:
        return jsonify(error="Only get method allowed")

@app.route('/threeNoises')
def hello_world():
    if request.method == 'GET':
        ser.write("n")
        return jsonify(response="Ok")
    else:
        return jsonify(error="Only get method allowed")

@app.route('/annoy')
def hello_world():
    if request.method == 'GET':
        ser.write("a")
        return jsonify(response="Ok")
    else:
        return jsonify(error="Only get method allowed")

@app.route('/stop')
def hello_world():
    if request.method == 'GET':
        ser.write("s")
        return jsonify(response="Ok")
    else:
        return jsonify(error="Only get method allowed")
   



if __name__ == '__main__':
    app.run(host="127.0.0.1", port=8081, debug=True, threaded=True)

