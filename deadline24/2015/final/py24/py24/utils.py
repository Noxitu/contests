# -*- coding: utf-8

import threading
from functools import wraps, partial

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
