# -*- coding: utf-8 -*-
#Flask app, jsonify jsonifies a response, request checks requests details
from flask import Flask, jsonify, request

import sys
reload(sys)  
sys.setdefaultencoding('utf8')

app = Flask(__name__)
@app.route('/')
def hello_world():
    if request.method == 'GET':
        val = request.args.get('id')
        if val == "A01323513":
            res = "true"
        else:
            res = "false"
        print(res)
        return jsonify(response=res)
    else:
        return jsonify(error="Only get method allowed")
   

if __name__ == '__main__':
    app.run(host="localhost", port=8080, debug=True, threaded=True)

