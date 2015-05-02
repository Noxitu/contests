import sys

def genericSwapper(obj, attr):
    class theSwapper:
        def __init__(self, new_value):
            self.new_value = new_value
            
        def __enter__(self):
            self.old_value = getattr(obj, attr)
            setattr(obj, attr, self.new_value)
            
        def __exit__(self, type, value, traceback):
            setattr(obj, attr, self.old_value)
            
    return theSwapper
            
stdinSwapper = genericSwapper(sys, 'stdin')
stdoutSwapper = genericSwapper(sys, 'stdout')