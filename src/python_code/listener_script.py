# -*- coding: utf-8 -*-
#Francisco Julián Huerta y Munive  A01323513
#Jorge Alberto Beauregard Braco    A01328439
#Tabatha Tabeli Acosta Pastrana    A01550280
#Begoña Montes Gómez               A01329896

#Fecha: 27 de Abril, 2018.

#Descripcion: Codigo del script para el arduino listener que monta un servidor web que escucha por rutas especificas para poder comunicarse por puerto serial al arduino para poder controlarlo

#Flask app, jsonify jsonifies a response, request checks requests details
from flask import Flask, jsonify, request
#serial for communication through usb port
import serial
#for time delays 
import time

app = Flask(__name__)
#define serial for macos or linux
ser = serial.Serial('/dev/cu.usbmodem1421',9600)
#ser = serial.Serial('/dev/ttyACM0',9600)

print("Serial opened")
print("Im the listener web server")


#In general, we define routes that can be accessed through the url of the server (we use a tunneling service to make the localhost public) and tell the arduino what to do

@app.route('/', methods=['GET', 'POST'])
def hello_world():
    if request.method == 'GET':
        return jsonify(response="Hola mundo")
    else:
        return jsonify(error="Only get method allowed")

@app.route('/post', methods=['GET', 'POST'])
def post_method():
    if request.method == 'POST':
        print(request.form['idSala'])
        return jsonify(response="Hola post")
    else:
        return jsonify(error="Only post method allowed")

@app.route('/red', methods=['GET', 'POST'])
def red():
    if request.method == 'GET':
        ser.write("r")
        return jsonify(response="red")
    else:
        return jsonify(error="Only get method allowed in /red")

@app.route('/green', methods=['GET', 'POST'])
def green():
    if request.method == 'GET':
        ser.write("g")
        return jsonify(response="green")
    else:
        return jsonify(error="Only get method allowed in /green")

@app.route('/yellow', methods=['GET', 'POST'])
def yellow():
    if request.method == 'GET':
        ser.write("y")
        return jsonify(response="yellow")
    else:
        return jsonify(error="Only get method allowed in /yellow")

@app.route('/threeNoises', methods=['GET', 'POST'])
def threeNoises():
    if request.method == 'GET':
        ser.write("t")
        return jsonify(response="threeNoises")
    else:
        return jsonify(error="Only get method allowed in /threeNoises")

@app.route('/beep', methods=['GET', 'POST'])
def beep():
    if request.method == 'GET':
        ser.write("b")
        return jsonify(response="beep")
    else:
        return jsonify(error="Only get method allowed in /passed")

@app.route('/annoy', methods=['GET', 'POST'])
def annoy():
    if request.method == 'GET':
        ser.write("a")
        return jsonify(response="annoy")
    else:
        return jsonify(error="Only get method allowed in /annoy")

@app.route('/stop', methods=['GET', 'POST'])
def stop():
    if request.method == 'GET':
        ser.write("s")
        return jsonify(response="stop")
    else:
        return jsonify(error="Only get method allowed in /stop")

@app.route('/pass', methods=['GET', 'POST'])
def passed():
    if request.method == 'GET':
        ser.write("p")
        return jsonify(response="pass")
    else:
        return jsonify(error="Only get method allowed in /passed")

@app.route('/noPass', methods=['GET', 'POST'])
def noPass():
    if request.method == 'GET':
        ser.write("n")
        return jsonify(response="noPass")
    else:
        return jsonify(error="Only get method allowed in /noPass")

@app.route('/setTimer', methods=['GET', 'POST'])
def setTimer():
    if request.method == 'GET':
        timeLeft = request.args.get("time")
        ser.write("c")
        ser.write(timeLeft.encode())
        ser.write("c")
        time.sleep(0.1)
        string = ser.readline().rstrip()
        print(string)
        return jsonify(response="setTimer")
    else:
        return jsonify(error="Only get method allowed in /setTimer")

@app.route('/stopAndResetTimer', methods=['GET', 'POST'])
def stopAndResetTimer():
    if request.method == 'GET':
        ser.write("h")
        return jsonify(response="setTimer")
    else:
        return jsonify(error="Only get method allowed in /setTimer")


if __name__ == '__main__':
    app.run(host="127.0.0.1", port=8080, debug=True, threaded=True)
