# -*- coding: utf-8 -*-
import requests
import json
import sys
reload(sys)  
sys.setdefaultencoding('utf8')


string = "A01323513"
url = "https://753678ac.ngrok.io/"
response = requests.get(url)
data = json.loads(response.text)
print(response.text)
print(data)
print(data['response'])


