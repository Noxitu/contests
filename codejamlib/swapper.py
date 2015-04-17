import sys

def genericSwapper(obj):
    class theSwapper:
        def __init__(self, new_value):
            self.new_value = new_value
            
        def __enter__(self):
            exec "self.old_value = %s" % obj in globals(), locals()
            exec "%s = self.new_value" % obj in globals(), locals()
            
        def __exit__(self, type, value, traceback):
            exec "%s = self.old_value" % obj in globals(), locals()
            
    return theSwapper
            
stdinSwapper = genericSwapper('sys.stdin')
stdoutSwapper = genericSwapper('sys.stdout')