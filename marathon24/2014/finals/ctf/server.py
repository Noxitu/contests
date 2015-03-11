#!/usr/bin/python

from failed import Failed
import socket
import log, config

def Action(callback):
	def call(self, *args, **kwargs):
		sep = kwargs.get('sep', ' ')
		argss = map(str, args)
		send = [callback.func_name] + argss
		self.send( sep.join(send))
		self.checkstatus()
		return callback(self, *args, **kwargs)
	return call

class Server:
	LOGIN = 'team11'
	PASSWORD = 'egk4vaq6zc'
	HOST = '10.0.0.1'
	PORT = 7171

	def send(self, what):
		log.net_out(what)
		return self._socket.sendall(what +'\n')

	def getline(self):
		while self._buffer.find('\n') == -1:
			self._buffer +=  self._socket.recv(2048)
		out, trash, self._buffer = self._buffer.partition('\n')
		log.net_in(out)
		return out
	
	def getiline(self):
		return map(int, self.getline().split() )

	def checkstatus(self):
		ret = self.getline().split(' ')
		if ret[0] == 'ERROR':
			raise Failed(int(ret[1]))( ' '.join(ret[1:]))

		if ret[0] != 'OK':
			raise Failed(1337)('Invalid response: expected OK or FAILED; got: %s' % str(ret) )

	@Action
	def WAIT(self):
		line = self.getline()
		line = line.split()
#		t = float(line[1])
#		self.while_waiting(t)
		self.checkstatus()

	def WAITING(self):
		line = self.getline()
		line = line.split()
#		t = float(line[1])
#		self.while_waiting(t)
		self.checkstatus()

	def __init__(self):
		self._socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self._socket.connect( (self.HOST, self.PORT) )
		log.net_conn()
		self._buffer = ''

		self.getline()
		self.send(self.LOGIN)
		self.getline()
		self.send(self.PASSWORD)
		self.checkstatus()

	def __getattr__(self, name):
		def defaultMethod(*args, **kwargs):
			sep = kwargs.get('sep', ' ')
			args = map(str, args)
			self.send( sep.join([name] + args))
			self.checkstatus()
		return defaultMethod

	def while_waiting(self):
		pass


#config.path = '1'
#s = Server()
#s.ME()

