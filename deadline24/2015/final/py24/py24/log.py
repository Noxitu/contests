# -*- coding: utf-8

import traceback
from time import strftime
from functools import wraps
from itertools import chain

def init(data_path):
    """Open log files, internal use"""
    global _log_file, _net_file, _status_file
    _log_file = open('%s/log.log' % data_path, 'a')
    _net_file = open('%s/net.log' % data_path, 'a')
    _status_file = open('%s/status.log' % data_path, 'w')

def net_in(msg):
    """Logs incoming message, internal use."""
    for line in msg.split('\n'):
        _net_file.write(" ->\t%s\n" % line)
        _net_file.flush()

def net_out(msg):
    """Logs outgoing message, internal use."""
    for line in msg.split('\n'):
        _net_file.write("<- \t%s\n" % line)
        _net_file.flush()

def net_conn():
    """Logs start of new connection, internal use."""
    _net_file.write("\n-X-\n\n")
    _net_file.flush()
    
def log(msg):
    """Logs message."""
    format = strftime("[%H:%M:%S]\t%%s\n")
    for line in str(msg).split('\n'):
        _log_file.write(format % line)
    _log_file.flush()
        
def log_exc():
    """Logs exception."""
    log( traceback.format_exc() )
    
    
def call(func):
    """Logs calls to decorated function."""
    try:
        msg = 'Calling %s.%s with arguments: %%s %%s' % (func.im_class.__name, func.__name__)
    except:
        msg = 'Calling %s with arguments: %%s %%s' % func.__name__
        
    @wraps(func)
    def call(*args, **kwargs):
        log( msg % (args, kwargs))
        return func(*args, **kwargs)
    return call
    
def calls(class_obj):
    """Logs calls to all methods of decorated class."""
    for key, value in vars(class_obj).items():
        if callable(value):
            setattr(class_obj, key, call(value))
    return class_obj
    
def status(msg):
    """Overrides current status with message."""
    _status_file.truncate(0)
    _status_file.write(msg)
    _status_file.write('\n')
    _status_file.flush()


def status_format(format, *args, **kwargs):
    """Overrides current status with formatted message.
        Sample of allowed syntax: {object.field['key']}
        `object` used in example must be present in either keyword arguments or in any dict passed as positional argument.
        You may call it as follows: status_format( "...", locals(), globals(), ... ). Note that nonlocal objects aren't in neither of these.
        Array keys in format are parsed using eval().
        To use { symbol use {{ in format string.
    """
    def format_it():
        name = None
        for c in format:
            if c == '{':
                if name is None:
                    sep = '^'
                    name = ''
                else:
                    yield '{'
                    name = None
           
            elif name is None:
                yield c
                
            else:
                if c in '}.[]':
                    if sep == '^':
                        found = False
                        for o in chain([kwargs], args):
                            if name in o:
                                found, obj = True, o[name]
                                break
                        if not found:
                            obj, sep = '<object not found[root]>', '$'
                            
                    if sep == '.':
                        try:
                            obj = getattr( obj, name )
                        except:
                            obj, sep = '<object not found[dot]>', '$'
                    if sep == '[':
                        try:
                            obj = obj[eval(name)]
                        except:
                            obj, sep = '<object not found[index]>', '$'
                    if sep == ']':
                        pass
                        
                    if sep != '$':
                        sep = c
                        
                    if c == '}':
                        yield str(obj)
                        name = None 
                    else:
                        name = ''
                else:
                    name += c
                
    status(''.join( format_it() ) )
                