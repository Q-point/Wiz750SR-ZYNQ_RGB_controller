# Copyright dhq 2018
# This project shows how to send TCP data to a server via a Python flask app.

from __future__ import division
from flask import Flask, request, make_response, redirect, render_template
import json
import time
from time import sleep  # pull in the sleep function from time module  
import socket

clientsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)


app = Flask(__name__)

pause_time = 0.02           # you can change this to slow down/speed up  

rpval = 0
gpval = 0
bpval = 0

host = '192.168.11.2'
port  = 5000


@app.route("/")
def hello():
	print ("Welcome to Python Flask!")
	return render_template('index.html')
 
@app.route("/RGB" ,methods=['POST'])
def RGB():
	try:
		if request.method == 'POST':
		# slider = request.json['name']
			rval = request.json['rval']
			gval = request.json['gval']
			bval = request.json['bval']
			# print "name: %s" % slider
			print ("R: %d " % rval)
			print ("G: %d " % gval)
			print ("B: %d " % bval)
			rpval = int((rval/255)*100)
			gpval = int((gval/255)*100)
			bpval = int((bval/255)*100)
			print ("calr: %d " % rpval)
			print ("calg: %d " % gpval)
			print ("calb: %d " % bpval)
			#try: 
			clientsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
			clientsocket.connect((host, port))
			#clientsocket.send(b'---------------------\r\n')
			clientsocket.send(b'%d' % rpval)
			#clientsocket.send(b'\r\n')
			clientsocket.send(b'%2x' % gpval)
			#clientsocket.send(b'\r\n')
			clientsocket.send(b'%2x' % bpval)
			#clientsocket.send(b'\r\n')
			
	except:
		# recreate the socket and reconnect
		clientsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		clientsocket.connect((host, port))
		clientsocket.send(b'Error')
		#connection.close()
		
	return json.dumps({'Status':'OK','red':rval,'green':gval,'blue':bval})


if __name__ == "__main__":
	app.run(host='0.0.0.0',debug=True, use_reloader=False)
