
import re, sys, os.path

def non_standard_module(name):
    mod = sys.modules[name]
    if not hasattr(mod, '__file__'):
        return False
    return os.path.dirname(mod.__file__) in ('', os.path.dirname(__file__))
    
def read_module_source(name):
    path = sys.modules[name].__file__
    if path[-1] == 'c':
        path = path[:-1]
    with open(path) as mod_source:
        return mod_source.read()

def export( final_name, source_path ):
    print >>sys.stderr, 'Creating "%s"...' % final_name
    
    with open(final_name, 'w') as f:
        print >>f, 'import imp, sys'
        print >>f
        
        parsed_modules = set()
        import_finder = re.compile(r'(?m)^\s*(?:import\s+((?:[ \t]|\w|,)+))|(?:from\s+(\w+)\s+import)')
        import_spliter = re.compile(r'(\w+)(?:\s+as\s+\w+)?')
        
        def parse_module(source, name=None):
            parsed_modules.add( name )
            for m in import_finder.finditer(source):
                if m.group(1) is not None:
                    modules = import_spliter.findall( m.group(1) )
                else: # m.group(2) is not None
                    modules = [ m.group(2) ]
                for module in modules:
                    if non_standard_module(module) and module not in parsed_modules:
                        parse_module( read_module_source(module), module )
                        
            if name is not None:
                print >>sys.stderr, '\tIncluding module "%s"...' % name
                print >>f, 'sys.modules["%s"] = __mod = imp.new_module("%s")' % (name, name)
                print >>f, 'exec """%s""" in vars(__mod)' % source.replace('\\','\\\\').replace('"','\\"')
                print >>f
            else:
                print >>sys.stderr, '\tIncluding task solution...'
                f.write(source)    
        
        with open(source_path) as task_file:
            source = task_file.read()
            parse_module(source)
            
            
        print >>f, """        
        
if __name__ == '__main__':
    T = input()
    for i in xrange(1, T+1):
        print 'Case #%d:' % i,
        solver = test()
        if hasattr(solver, 'next'):
            list(solver)
        elif callable(solver):
            solver()
"""