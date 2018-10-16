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
     coffee_sql = "SELECT moisture FROM sense_data WHERE plant = 'coffee' LIMIT 100"
     palm_sql = " SELECT moisture FROM sense_data WHERE plant = 'palm' LIMIT 100"
     cursor.execute(coffee_sql)
     row = cursor.fetchall()
     db.commit()
     coffee_data_points = ([int(x[0]) for x in row])

     cursor.execute(palm_sql)
     row = cursor.fetchall()
     db.commit()
     palm_data_points = ([int(x[0]) for x in row])

     try:
	graph = pygal.Line()
	graph.title = 'plant moisture data'
	graph.x_labels  = '1' 
        graph.add('coffee_moisture', list(coffee_data_points))
        graph.add('palm_moisture', list(palm_data_points))
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
