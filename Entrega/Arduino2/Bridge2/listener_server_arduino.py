# -*- coding: utf-8 -*-
#Flask app, jsonify jsonifies a response, request checks requests details
from flask import Flask, jsonify, request
import serial

app = Flask(__name__)
ser = serial.Serial('/dev/cu.usbmodem1421',9600)
print("Serial opened")

@app.route('/')
def hello_world():
    if request.method == 'GET':
        return jsonify(response="Hola mundo")
    else:
        return jsonify(error="Only get method allowed")

@app.route('/red')
def red():
    if request.method == 'GET':
        ser.write("r")
        return jsonify(response="red")
    else:
        return jsonify(error="Only get method allowed in /red")

@app.route('/green')
def green():
    if request.method == 'GET':
        ser.write("g")
        return jsonify(response="green")
    else:
        return jsonify(error="Only get method allowed in /green")

@app.route('/yellow')
def yellow():
    if request.method == 'GET':
        ser.write("y")
        return jsonify(response="yellow")
    else:
        return jsonify(error="Only get method allowed in /yellow")

@app.route('/threeNoises')
def threeNoises():
    if request.method == 'GET':
        ser.write("t")
        return jsonify(response="threeNoises")
    else:
        return jsonify(error="Only get method allowed in /threeNoises")

@app.route('/annoy')
def annoy():
    if request.method == 'GET':
        ser.write("a")
        return jsonify(response="annoy")
    else:
        return jsonify(error="Only get method allowed in /annoy")

@app.route('/stop')
def stop():
    if request.method == 'GET':
        ser.write("s")
        return jsonify(response="stop")
    else:
        return jsonify(error="Only get method allowed in /stop")

@app.route('/passed')
def passed():
    if request.method == 'GET':
        ser.write("p")
        return jsonify(response="passed")
    else:
        return jsonify(error="Only get method allowed in /passed")

@app.route('/noPass')
def noPass():
    if request.method == 'GET':
        ser.write("n")
        return jsonify(response="noPass")
    else:
        return jsonify(error="Only get method allowed in /noPass")

if __name__ == '__main__':
    app.run(host="127.0.0.1", port=8080, debug=True, threaded=True)
