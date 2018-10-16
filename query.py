#!flask/bin/python

import json
import MySQLdb
import pygal

from datetime import datetime
from flask import Flask
from flask import request
from flask import render_template
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

@app.route('/hello/<user>')
def hello_name(user):
    return render_template('hello.html', name = user)


@app.route('/pygalexample/')
def pygalexample():
     db = MySQLdb.connect(host='localhost',
                          db='api',
                          user='root',
                          passwd='H3ll0nurse!#')
     cursor = db.cursor()
     sql = "SELECT moisture FROM sense_data WHERE plant = 'coffee' LIMIT 100"
     cursor.execute(sql)
     row = cursor.fetchall()
     db.commit()
     print(type(row))
     data_points = ([int(x[0]) for x in row])
     print(type(data_points))
     #str =  ','.join(row)
     #print (str)
     #for var in row:
     #       print (type(str(var[0])))

     try:
	graph = pygal.Line()
	graph.title = 'coffee plant moisture over time'
	graph.x_labels  = data_points
        graph.add('coffee_moisture', list(data_points))
	graph_data = graph.render_data_uri()
        return render_template("graphing.html", graph_data = graph_data)
     except Exception, e:
	return(str(e))


@app.before_request
def log_request_info():
     app.logger.debug('Headers: %s', request.headers)
     app.logger.debug('Body: %s', request.get_data())


if __name__ == '__main__':

    app.run(host='0.0.0.0', port=80, debug=True)
