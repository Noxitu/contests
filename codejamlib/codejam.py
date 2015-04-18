#!/usr/bin/python

import argparse, imp, sys, os, time, re

import multiprocessing
from cStringIO import StringIO
from shutil import copyfile
from tempfile import TemporaryFile

import bcolors
from swapper import stdinSwapper, stdoutSwapper
from exporter import export

    
def simple_solve(input_file, output_file, debug=False, delayCase=False, **kwargs):
    solve_start = time.time()
    
    with stdoutSwapper(output_file), stdinSwapper(input_file):
        T = input()
        
        if debug:
            print >>sys.stderr, '\tFound $B%d$$ cases.'.replace('$B', bcolors.OKBLUE).replace('$$', bcolors.ENDC) % (T)
        
        for id in xrange(1, T+1):
            if not delayCase:
                print 'Case #%d:' % id,
                
            solve = task.test()
            
            if hasattr(solve, '__call__'):
                if delayCase:
                    print 'Case #%d:' % id,
                    
                solve()
                
                if debug:
                    print >>sys.stderr, '\tCase $B#%d$$ solved [$B%dms$$]. $B%d$$ cases left.'.replace('$B', bcolors.OKBLUE).replace('$$', bcolors.ENDC) % (id, 1000*(time.time()-solve_start), T-id)
    if debug:                
        print >>sys.stderr, '\tOutput saved. [$B%dms$$].'.replace('$B', bcolors.OKBLUE).replace('$$', bcolors.ENDC) % (1000*(time.time()-solve_start))
                
                
def complex_solve(input_file, output_file, debug=False, processes=4, **kwargs):
    solve_start = time.time()    
    solvers = []
    
    with stdinSwapper(input_file):
        T = input()
            
        for i in xrange(1, T+1):
            with stdoutSwapper(output_file):
                solve = task.test()
                if hasattr(solve, '__call__'):
                    solvers.append(( i, solve ))
                    
    if len(solvers) != T:
        raise Exception('cant solve paraller')
        
    if debug:
        print >>sys.stderr, '\tInput loaded [$B%dms$$]. Found $B%d$$ cases.'.replace('$B', bcolors.OKBLUE).replace('$$', bcolors.ENDC) % (1000*(time.time()-solve_start), T)
        
    current_test = multiprocessing.Value('i', 0)
    queue_out = multiprocessing.Queue()
    output_data = [ None for t in solvers ]
    
    
    for p in xrange(processes):
        pid = os.fork()
        
        if pid == 0:
            while True:
                with current_test.get_lock():
                    i = current_test.value
                    current_test.value = i + 1
                    
                if i >= T:
                    sys.exit(0)
                    
                id, solve = solvers[i]
                output_data = StringIO()
                with stdoutSwapper(output_data):
                    print 'Case #%d:' % id,
                    solve()
                    
                queue_out.put(( i, output_data.getvalue(), p ))
                
    for j in xrange(T):
        i, data, p = queue_out.get()
        id = solvers[i][0]
        output_data[i] = data
        if debug:
            print >>sys.stderr, '\tCase $B#%d$$ solved by $B@%d$$ [$B%dms$$]. $B%d$$ cases left.'.replace('$B', bcolors.OKBLUE).replace('$$', bcolors.ENDC) % (id, p+1, 1000*(time.time()-solve_start), T-j-1)
        
    for data in output_data:
        output_file.write(data)
      
    if debug:
        print >>sys.stderr, '\tOutput saved. [$B%dms$$].'.replace('$B', bcolors.OKBLUE).replace('$$', bcolors.ENDC) % (1000*(time.time()-solve_start))
            
def findInputFiles(taskid):
    files = os.listdir('.')
    
    examples = []
    problems = []
    problem = None
    
    for f in files:
        if f.endswith('.in'):
            if f.startswith('%s-example' % taskid):
                examples.append( (f, f[:-3]+'.out') )
            elif f.startswith(taskid):
                problems.append( (f, f[:-3]+'.out') )
                
    if problems:
        problem = max(problems, key=lambda (I,O) : os.path.getctime(I))
                
    return examples, problem
    
def file_contents(f):
    return [ l.split() for l in f.read().split('\n') ]
    
def are_same(one, two):
    return one == two
    
def color_diff(own, expected):
    for y, L in enumerate(own):
        for x, W in enumerate(L):
            if y >= len(expected) or x >= len(expected[y]):
                own[y][x] = bcolors.FAIL + W + bcolors.ENDC
            elif W != expected[y][x]:
                own[y][x] = bcolors.FAIL + W + bcolors.OKGREEN + ' (' + expected[y][x] + ')' + bcolors.ENDC

if __name__ == '__main__':

    parser = argparse.ArgumentParser(description='Framework for solving Google Code Jam and Facebook Hacker Cup.')
    parser.add_argument('-p', '--processes', help='use multiple processes while solving', type=int, default=1)
    parser.add_argument('-I', '--init', help='initize task source', action='store_true')
    parser.add_argument('-s', '--std', help='use stdin as input and stdout as output', action='store_true')
    parser.add_argument('-o', '--output', help='use stdout as output', action='store_true')
    parser.add_argument('-f', '--file', help='solve test with given name')
    parser.add_argument('-d', '--debug', help='display debug information (eg. solved cases)', action='store_true')
    parser.add_argument('-q', '--quiet', help='hide all stderr text', action='store_true')
    parser.add_argument('task', help='task id')

    args = parser.parse_args()
    
    if args.quiet:
        sys.stderr = open(os.devnull, 'w')
    
    source_path = '%s.py' % args.task
    
    if args.init:
        if os.path.exists(source_path):
            raise Exception('Source file already exisits!')
            
        copyfile(os.path.join( os.path.dirname(__file__), 'task.py.tpl' ), source_path )
        sys.exit(0)
        
    
    print >>sys.stderr, 'Loading solution module...'
    task = imp.load_source( 'task', source_path )
    solve = complex_solve if args.processes > 1 else simple_solve
    
    examples, problem = findInputFiles(args.task)
    
    for I, O in examples:
        print >>sys.stderr, 'Solving example "%s"...' % I
        with open(I) as input_file, TemporaryFile() as output_file:
            simple_solve(input_file, output_file)
            
            output_file.seek(0)
            try:
                with open(O) as expected_file:
                    output_content = file_contents(output_file)
                    expected_content = file_contents(expected_file)
                    
                    if not are_same(output_content, expected_content):
                        print >>sys.stderr, bcolors.FAIL, 'Example "%s" has incorrect output!' % I, bcolors.ENDC
                        color_diff(output_content, expected_content)
                        print >>sys.stderr, '\n'.join( ' '.join( l ) for l in output_content )
            except IOError:
                print >>sys.stderr, bcolors.WARNING, 'Couldnt verify example "%s" output!' % I, bcolors.ENDC
                
    if args.file:
        in_path = None
        for path in (args.file, args.file+'.in', args.file.rsplit('.')[0]+'.in'):
            if os.path.exists(path):
                in_path = path
                break
            
        if in_path is None:
            raise Exception('Couldnt find input file!')
                
        if in_path.endswith('.in'):
            out_path = in_path[:-3] + '.out'
        else:
            out_path = in_path + '.out'
        problem = in_path, out_path
                
    if problem or args.std: 
        print >>sys.stderr, 'Solving %s...' % ('STDIN' if args.std else '"%s"' % problem[0])
        with sys.stdin if args.std else open(problem[0]) as input_file:
            with sys.stdout if args.std or args.output else open(problem[1], 'w') as output_file:
                solve( input_file, output_file, debug=args.debug, processes=args.processes, delayCase=args.std or args.output )
            

    export( '%s-source.py' % args.task, source_path )
  
