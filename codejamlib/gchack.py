
import gc
from functools import wraps

class gchack:
    def __enter__(self):
        gc.disable()
        
    def __exit__(self, type, value, traceback):
        gc.enable()
                
gchack = gchack()

