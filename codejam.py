#!/usr/bin/python

import argparse, imp, sys, os, time, re

from shutil import copyfile
from multiprocessing import Semaphore
from tempfile import TemporaryFile
import tarfile

from utils import setLineInput, iline

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

class stdoutSwapper:
	def __init__(self, new_stdout):
		self.new_stdout = new_stdout
		
	def __enter__(self):
		self.old_stdout, sys.stdout = sys.stdout, self.new_stdout
		
	def __exit__(self, type, value, traceback):
		sys.stdout = self.old_stdout
		
def simple_solve(input_file, output_file, debug=False, **kwargs):
	start_time = time.time()
	
	with stdoutSwapper(output_file):
		setLineInput(input_file)
		T, = iline()
		
		for i in xrange(1, T+1):
			print 'Case #%d:' % i,
			solve = task.test()
			if hasattr(solve, '__call__'):
				solve()
				
			if debug:
				print >>sys.stderr, '\tCase %d solved [%dms].' % (i, 1000*(time.time()-start_time))
				
				
				
def complex_solve(input_file, output_file, debug=False, processes=4, **kwargs):
	start_time = time.time()	
	solvers = []
	
	setLineInput(input_file)
	T, = iline()
		
	for i in xrange(1, T+1):
		with stdoutSwapper(output_file):
			solve = task.test()
			if hasattr(solve, '__call__'):
				solvers.append( (i, solve) )
			else:
				if debug:
					print >>sys.stderr, '\tCase %d solved [%dms].' % (i, 1000*(time.time()-start_time))
					
	if len(solvers) != T:
		raise Exception('cant solve paraller')
		
	fds = []
	semaphore = Semaphore(processes)
	for i, solve in solvers:
		pid, fd = os.forkpty()
		
		if pid == 0:
			with semaphore:
				print 'Case #%d:' % i,
				solve()
					
			sys.exit(0)
			
		fds.append(( i, fd ))
		
	for i, fd in fds:
		while True:
			data = os.read(fd, 1024)
			if len(data) == 0:
				break
			output_file.write(data)
			
		if debug:
			print >>sys.stderr, '\tCase %d solved [%dms].' % (i, 1000*(time.time()-start_time))
				
def findInputFiles(path, taskid):
	files = os.listdir(path)
	
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
		problem = max(problems, key=lambda (I,O) : os.path.getctime(path+I))
				
	return examples, problem
	
def file_diff(one, two):
	one = one.read().split()
	two = two.read().split()
	
	return one == two
			

if __name__ == '__main__':

	parser = argparse.ArgumentParser(description='Framework for solving Google Code Jam and Facebook Hacker Cup.')
	parser.add_argument('-p', '--processes', help='use multiple processes while solving', type=int, default=1)
	parser.add_argument('-I', '--init', help='initize task source', action='store_true')
	parser.add_argument('-s', '--std', help='use stdin as input and stdout as output', action='store_true')
	parser.add_argument('-o', '--output', help='use stdout as output', action='store_true')
	parser.add_argument('-d', '--debug', help='display debug information (eg. solved cases)', action='store_true')
	parser.add_argument('-r', '--round', help='round directory path. Defaults to last used stored in "codejam.round"')
	parser.add_argument('-q', '--quiet', help='hide all stderr text', action='store_true')
	parser.add_argument('task', help='task id')

	args = parser.parse_args()
	
	if args.quiet:
		sys.stderr = open(os.devnull, 'w')
	
	directory = args.round
	
	if directory is None:
		with open('codejam.round') as f:
			directory = f.read().strip()
	else:
		with open('codejam.round', 'w') as f:
			f.write(directory)
			
	if directory[-1] != '/':
		directory += '/'
	
	source_path = '%s%s.py' % (directory, args.task)
	
	if args.init:
		if os.path.exists(source_path):
			raise Exception('Source file already exisits!')
			
		copyfile('task.py.tpl', source_path )
		sys.exit(0)
	
	task = imp.load_source( 'task', source_path )
	solve = complex_solve if args.processes > 1 else simple_solve
	
	examples, problem = findInputFiles(directory, args.task)
	
	for I, O in examples:
		print >>sys.stderr, 'Solving example "%s"...' % I
		with open(directory+I) as input_file, TemporaryFile() as output_file:
			simple_solve(input_file, output_file)
			
			output_file.seek(0)
			try:
				with open(directory+O) as expected_file:
					if not file_diff(output_file, expected_file):
						print >>sys.stderr, bcolors.FAIL, 'Example "%s" has incorrect output!' % I, bcolors.ENDC
			except IOError:
				print >>sys.stderr, bcolors.WARNING, 'Couldnt verify example "%s" output!' % I, bcolors.ENDC
				
	if problem or args.std: 
		print >>sys.stderr, 'Solving %s...' % ('STDIN' if args.std else '"%s"' % problem[0])
		with sys.stdin if args.std else open(directory+problem[0]) as input_file:
			with sys.stdout if args.std or args.output else open(directory+problem[1], 'w') as output_file:
				solve( input_file, output_file, debug=args.debug, processes=args.processes, delay_case=args.std or args.output )
			

			
	tar_name = '%s-source.tar' % args.task
	print >>sys.stderr, 'Creating "%s"...' % tar_name
	
	with tarfile.open(directory+tar_name, 'w') as tar:
	
		for path,filename in [(source_path, 'solution.py'), ('utils.py', 'utils.py')]:
			def filter_tarinfo(t):
				t.name = filename
				t.mtime = t.mode = t.uid = t.gid = 0
				t.uname = t.gname = ''
				t.pax_headers = {}
				return t
				
			tar.add(path, filter=filter_tarinfo)
		
