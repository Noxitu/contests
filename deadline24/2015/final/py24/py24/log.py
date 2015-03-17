# -*- coding: utf-8

import traceback
from time import strftime

def init(data_path):
    global _log_file, _net_file
    _log_file = open('%s/log.log' % data_path, 'a')
    _net_file = open('%s/net.log' % data_path, 'a')

def net_in(msg):
    for line in msg.split('\n'):
        _net_file.write(" ->\t%s\n" % line)
        _net_file.flush()

def net_out(msg):
    for line in msg.split('\n'):
        _net_file.write("<- \t%s\n" % line)
        _net_file.flush()

def net_conn():
    _net_file.write("\n-X-\n\n")
    _net_file.flush()
    
def log(msg):
    format = strftime("[%H:%M:%S]\t%%s\n")
    for line in str(msg).split('\n'):
        _log_file.write(format % line)
    _log_file.flush()
        
def log_exc():
    log( traceback.format_exc() )