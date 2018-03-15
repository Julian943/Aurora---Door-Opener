# -*- coding: utf-8 -*-
#Flask app, jsonify jsonifies a response, request checks requests details
from flask import Flask, jsonify, request
import serial


app = Flask(__name__)
#ser = serial.Serial('/dev/cu.usbmodem1421',9600
print("Serial opened")

@app.route('/')
def hello_world():
    if request.method == 'GET':
        return jsonify(response="Hola mundo")
    else:
        return jsonify(error="Only get method allowed")

@app.route('/rm')
def removeCard():
    if request.method == 'GET':
        return jsonify(response="Ok")
    else:
        return jsonify(error="Only get method allowed")


@app.route('/in')
def insertCard():
    if request.method == 'GET':
        return jsonify(response="Ok")
    else:
        return jsonify(error="Only get method allowed")


@app.route('/card')
def validateCard():
    if request.method == 'GET':
        val = request.args.get('id')
        if val == "1a:b9:e2:2b":
            res = "true"
        else:
            res = "false"
        print(val)
        print(res)
        return jsonify(response=res)
    else:
        return jsonify(error="Only get method allowed")




if __name__ == '__main__':
    app.run(host="127.0.0.1", port=8081, debug=True, threaded=True)

