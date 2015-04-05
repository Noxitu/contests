# -*- coding: utf-8

import socket
from itertools import chain, repeat
from functools import wraps

import py24.log as log
from .config import config
from .failed import Failed
from .utils import izip


def Action(callback):
    """Decorated methods send their name and non-keyword arguments."""
    @wraps(callback)
    def call(self, *args, **kwargs):
        sep = kwargs.get('sep', ' ')
        send = [callback.func_name] + map(str, args)
        self.send( sep.join(send))
        self.checkstatus()
        return callback(self, *args, **kwargs)
    return call

class Client:
    """Auto-connects to server specified in config. Allows for easy sending and receiving of data."""
    def send(self, what):
        """Sends line of text."""
        log.net_out(what)
        return self._socket.sendall(what +'\n')

    def getline(self):
        """Receives line of text."""
        while self._buffer.find('\n') == -1:
            self._buffer +=  self._socket.recv(2048)
        out, trash, self._buffer = self._buffer.partition('\n')
        log.net_in(out)
        return out
    
    def line(self, *args):
        """Receives and splits line of text. Converts i-th word using i-th argument."""
        return [ type(value) for value, type in izip( self.getline.split(), chain( args, repeat(str) ) ) ]
    
    def iline(self):
        """Receives line of integers."""
        return map(int, self.getline().split() )
        
    def fline(self):
        """Receives line of floats."""
        return map(float, self.getline().split() )

    def checkstatus(self):
        """Receives OK or throws exception if FAILED"""
        ret = self.getline().split(' ')
        if ret[0] == self.FAILED_RESPONSE:
            raise Failed(int(ret[1]))( ' '.join(ret[1:]))

        if ret[0] != 'OK':
            raise AssertionError('Invalid response: expected OK or %s; got: %s' % (self.FAILED_RESPONSE, str(ret)) )

    @Action
    def WAIT(self):
        line = self.getline()
        line = line.split()
        if line[0] != 'WAITING':
            raise AssertionError('Expected WAITING command')

        if self.SERVER_MODE == 'd24':
            self.while_waiting(float(line[1]))
            
        self.checkstatus()

    def __init__(self):
        self.SERVER_MODE = config['mode']
        self.FAILED_RESPONSE = {'m24': 'ERROR', 'd24': 'FAILED'}[self.SERVER_MODE]
        PASSWORD_CMD = {'m24': 'PASSWORD', 'd24': 'PASS'}[self.SERVER_MODE]
        
        self._socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self._socket.connect( (config['hostname'], config['port']) )
        self._socket.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
        
        log.net_conn()
        self._buffer = ''

        if self.getline() != 'LOGIN':
            raise AssertionError('Server did not ask for LOGIN')
        self.send(config['login'])
        if self.getline() != PASSWORD_CMD:
            raise AssertionError('Server did not ask for %s' % PASSWORD_CMD)
        self.send(config['password'])
        self.checkstatus()

    def __getattr__(self, name):
        if name.startswith('_'):
            raise AttributeError('Attribute %s not found' % name)
        
        def defaultMethod(*args, **kwargs):
            sep = kwargs.get('sep', ' ')
            args = map(str, args)
            self.send( sep.join([name] + args))
            self.checkstatus()
        return defaultMethod

    def while_waiting(self):
        pass
        
