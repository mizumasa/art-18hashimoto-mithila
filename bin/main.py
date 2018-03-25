#!/usr/local/bin/python
# -*- coding: utf-8 -*-
#内容
#auther:mizuochi
#from 11/11/

import sys
import os
import numpy as np
import json
import random
import time
import pprint
import OSC
import time, random
import threading

OSC_PORT_OF2PY = 7110
OSC_PORT_PY2OF = 7111
SEND_ADDRESS = '127.0.0.1', OSC_PORT_PY2OF
RECV_ADDRESS = '127.0.0.1', OSC_PORT_OF2PY

FILEBASE = "/Users/sapugc/programming/of_v0.9.8_osx/apps/Art2018/MithilaPainting/bin/data/"

from imgurpython import ImgurClient
import qrcode
#import pyperclip
client_id = 'd2b14e2d0051120'
client_secret = 'c354854c2797da7b7814013360bd730ff805e3af'
client = ImgurClient(client_id, client_secret)


def uploadImgur(filename):
	filepath = os.path.join(FILEBASE,filename)
	if os.path.exists(filepath):
		print "[PY] exec",filename
		res = client.upload_from_path(filepath)
		print "[PY] imgur upload finihsed",filename
		#print "[PY] fileUrl",res
		if "link" in res.keys():
			img = qrcode.make(res["link"])
			qrpath = filepath.replace("/capture/","/qr/")
			img.save(qrpath)
			qrfile = qrpath.replace(FILEBASE,"")
		else:
			qrfile = "none"
	else:
		print "[PY] no such file",filepath
		qrfile = "none"
	return qrfile


class PyModule:
	def __init__(self):
		self.errorCount = 0
		self.flag = True
		self.setup()
		return
	def setup(self):
		print "[PY] client init"
		self.c = OSC.OSCClient()
		self.c.connect(SEND_ADDRESS) 
		msg = OSC.OSCMessage() 
		msg.setAddress("/status")
		msg.append("python OSC started")
		self.c.send(msg)
		print "[PY] server init"
		self.s = OSC.OSCServer(RECV_ADDRESS)
		self.s.addDefaultHandlers()
		#self.s.addMsgHandler("/bapabar/test", printing_handler)
		self.s.addMsgHandler("/image/saved", self.imageSaved)
		self.s.addMsgHandler("/test", self.printMsg)
		self.s.addMsgHandler("/kill", self.kill)
		self.st = threading.Thread( target = self.s.serve_forever )
		self.st.start()

		print "[PY] server py2py test"
		self.c2 = OSC.OSCClient()
		self.c2.connect(RECV_ADDRESS) 
		msg = OSC.OSCMessage() 
		msg.setAddress("/test")
		msg.append(1)
		self.c2.send(msg)

		if 0:#finish in 4 sec
			time.sleep(4)
			print "[PY] server py2py test"
			self.c2 = OSC.OSCClient()
			self.c2.connect(RECV_ADDRESS) 
			msg = OSC.OSCMessage() 
			msg.setAddress("/kill")
			msg.append(1)
			self.c2.send(msg)
		return

	def imageSaved(self, addr, tags, stuff, source):
		print "---"
		print "received new osc msg from %s" % OSC.getUrlStr(source)
		print "with addr : %s" % addr
		print "typetags %s" % tags
		print "data %s" % stuff
		filename = stuff[0]
		msg = OSC.OSCMessage()
		msg.setAddress("/image/uploaded")
		qrname = uploadImgur(filename)
		msg.append(qrname)
		self.c.send(msg)
		return

	def printMsg(self, addr, tags, stuff, source):
		self.flag = True
		print "---"
		print "received new osc msg from %s" % OSC.getUrlStr(source)
		print "with addr : %s" % addr
		print "typetags %s" % tags
		print "data %s" % stuff
		
	def kill(self, addr, tags, stuff, source):
		self.flag = False
		print "---"
		print "received new osc msg from %s" % OSC.getUrlStr(source)
		print "with addr : %s" % addr
		print "typetags %s" % tags
		print "data %s" % stuff

	def start(self):
		print "[PY] PyModule Start"
		count = 0
		while 1:
			try:
				time.sleep(2)
				msg = OSC.OSCMessage() 
				msg.setAddress("/status")
				count += 1
				msg.append("python awake "+str(count))
				self.c.send(msg)

				if self.flag == False:
					break
			except:
				self.errorCount += 1
				print "error (need to setup oF server)", self.errorCount
				if self.errorCount > 5:
					break
		print "[PY] PyModule Stop"
		try:
			msg = OSC.OSCMessage() 
			msg.setAddress("/status")
			msg.append("[PY] python OSC goodbye")
			self.c.send(msg)
		except:
			print "[PY] goodbye error"

		self.s.close()
		self.st.join()
		return


def printing_handler(addr, tags, stuff, source):
	print "---"
	print "received new osc msg from %s" % OSC.getUrlStr(source)
	print "with addr : %s" % addr
	print "typetags %s" % tags
	print "data %s" % stuff


def main():
	a = PyModule()
	a.start()
	pass

if __name__=='__main__':
	argvs=sys.argv
	print argvs
	main()
