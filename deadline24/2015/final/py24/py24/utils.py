# -*- coding: utf-8

import threading

def Thread(daemon=True):
    def decorator(callable):
        def call():
            thread = threading.Thread(target=callable)
            thread.setDaemon(daemon)
            thread.start()
        return call
    return decorator
