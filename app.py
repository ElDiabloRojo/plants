#!flask/bin/python
from flask import Flask
from flask import request

app = Flask(__name__)

@app.route('/')
def index():
    return "Hello, World!"

@app.route('/jinput', methods = ['POST'])
def jinputHandler():
    req_data = request.get_json()
    #data1 = req_data['data1']
    #data2 = req_data['data2']

    return '''
            data1: {}
            data2: {}'''.format(data1

if __name__ == '__main__':

    app.run(host='0.0.0.0', port=5000, debug=True)
