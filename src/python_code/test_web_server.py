# -*- coding: utf-8 -*-

#TESTING WEB SERVER, NOT INCLUDED IN FUNCTIONAL CODE

#Flask app, jsonify jsonifies a response, request checks requests details
from flask import Flask, jsonify, request
import serial


app = Flask(__name__)
#ser = serial.Serial('/dev/cu.usbmodem1421',9600
print("Serial opened")
print("Soy el andres virtual")

@app.route('/')
def hello_world():
    if request.method == 'GET':
        return jsonify(response="Hola mundo")
    else:
        return jsonify(error="Only get method allowed")

@app.route('/rm', methods=['POST', 'GET'])
def removeCard():
    if request.method == 'GET':
        return jsonify(response="Ok")
    elif request.method == 'POST':
        #print(request.form['idSala'])
        #print(request.form['idTarjeta'])
        return jsonify(response="Ok")
    else:
        return jsonify(error="Only get method allowed")

@app.route('/delete', methods=['POST', 'GET'])
def deleteCard():
    if request.method == 'GET':
        return jsonify(response="Ok")
    elif request.method == 'POST':
        #print(request.form['idSala'])
        #print(request.form['idTarjeta'])
        return jsonify(response="Ok")
    else:
        return jsonify(error="Only get method allowed")

@app.route('/card', methods=['POST', 'GET'])
def validateCard():
    if request.method == 'POST':
        val = request.form['idCard']
        if val == "1a:b9:e2:2b":
            res = "True"
        else:
            res = "False"
        print(val)
        print(res)
        return jsonify(response=res)
    else:
        return jsonify(error="Only get method allowed")


if __name__ == '__main__':
    app.run(host="127.0.0.1", port=8081, debug=True, threaded=True)

