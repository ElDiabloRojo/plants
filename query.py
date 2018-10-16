#!flask/bin/python

import json
import MySQLdb

from datetime import datetime
from flask import Flask
from flask import request
from flask import jsonify

app = Flask(__name__)

@app.route('/')
def index():
        return "hello"

@app.route('/temp', methods=['POST'])
def temp():
     temp_q = request.get_json()
     temp_str = json.dumps(temp_q) 
     temp_d = json.loads(temp_str)

     for k in temp_d:
        print (k + ": " + temp_d[k]) 

     db = MySQLdb.connect(host='localhost',
                          db='api',
                          user='root',
                          passwd='H3ll0nurse!#')    
     cursor = db.cursor() 

     sql = "INSERT INTO sense_data (plant, moisture, temp) VALUES (%s, %s, %s);"
     data = (temp_d['plant'], temp_d['moisture'], temp_d['temp'])
     cursor.execute(sql, data)
     print(cursor._executed)
     db.commit()
     return 'success' 

@app.before_request
def log_request_info():
     app.logger.debug('Headers: %s', request.headers)
     app.logger.debug('Body: %s', request.get_data())


if __name__ == '__main__':

    app.run(host='0.0.0.0', port=80, debug=True)
