#!flask/bin/python

import json
import MySQLdb
import pygal

from datetime import datetime
from pygal.style import LightGreenStyle
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

@app.route('/feedme/')
def feed():
     db = MySQLdb.connect(host='localhost',
                          db='api',
                          user='root',
                          passwd='H3ll0nurse!#')
     cursor = db.cursor()

     sql = "SELECT moisture FROM sense_data ORDER BY ID DESC LIMIT 1"
     cursor.execute(sql)
     print(cursor._executed)
     result = cursor.fetchall()
     for i in result:
        feed = (str(i[0]))
     db.commit()
     return feed

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
     coffee_sql = "SELECT moisture,timestamp FROM sense_data WHERE plant = 'coffee' AND DATE(timestamp) = DATE(CURDATE()) ORDER BY ID DESC"
     palm_sql = " SELECT moisture,timestamp FROM sense_data WHERE plant = 'palm' AND DATE(timestamp) = DATE(CURDATE()) ORDER BY ID DESC"
     g_time = "SELECT TIME(timestamp) FROM sense_data WHERE plant = 'coffee' AND DATE(timestamp) = DATE(CURDATE()) ORDER BY ID DESC"

     cursor.execute(g_time)
     row = cursor.fetchall()
     db.commit()
     formatted_x = ""
     g_time_x = ([str(x[0]) for x in row])

     cursor.execute(coffee_sql)
     row = cursor.fetchall()
     db.commit()
     coffee_data_points = ([int(x[0]) for x in row])

     cursor.execute(palm_sql)
     row = cursor.fetchall()
     db.commit()
     palm_data_points = ([int(x[0]) for x in row])

     try:
        graph = pygal.Line(fill=False, show_only_major_dots=True, interpolate='cubic', x_label_rotation=20, x_labels_major_every=10, show_minor_x_labels=False, show_x_labels=True, style=LightGreenStyle)
	graph.title = 'plant moisture data'
        graph.x_labels  = reversed(g_time_x[0::25])
        graph.add('coffee_moisture', list(reversed(coffee_data_points[0::25])))
        graph.add('palm_moisture', list(reversed(palm_data_points[0::25])))
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
