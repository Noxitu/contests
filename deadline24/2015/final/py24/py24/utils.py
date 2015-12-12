# -*- coding: utf-8

import threading
from functools import wraps, partial
from itertools import chain

# Python 2 & 3 madness
try:
    from itertools import izip
except:
    izip = zip
    
try:
    exec('def exec_(src, g, l):\n  exec src in g, l\n')
except:
    exec_ = eval('exec')
    
def Thread(func=None, daemon=True):
    """Functions with this decorator will start new thread.
        You can use @Thread or @Thread(daemon=True/False)
    """
    if func is None:
        return partial(func, daemon=daemon)
        
    @wraps(func)
    def call():
        thread = threading.Thread(target=func)
        thread.setDaemon(daemon)
        thread.start()
    return call

    
# Did someone say "Metaprogramming"?
def Fields(*args):
    """Creates constructor coping positional arguments. Uses arguments as field names."""
    def decorator(base):
        code = '\n'.join([
            'def __init__(%s):',
            '%s',
            '']) % (', '.join(chain(['self'],args)), '\n'.join('    self.%s = %s' % (a,a) for a in args) )
            
        loc = {}
        exec_(code, globals(), loc)
        base.__init__ = loc['__init__']
        
        return base
    return decorator
    