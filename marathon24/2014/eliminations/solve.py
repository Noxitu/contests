#!/usr/bin/python
# -*- coding: utf-8 -*-

import imp
import os, errno
import re
import multiprocessing
import sys

from itertools import izip

try:
	os.mkdir('out')
except OSError as e:
	if e.errno != errno.EEXIST:
		raise e

class Data:
	def __init__(self, (no, data)):
		self.i = None
		self.no = no
		self.data = data
		self.answer = None

class Dataset:
	def __init__(self, id, path, name):
		self.id = id
		self.path = path
		self.name = name
		

def find_datasets(task_id):
	tasks = os.listdir('datasets')
	datasets_path = filter( lambda dir : dir[:len(task_id)].lower() == task_id, tasks )
	
	if len(datasets_path) != 1:
		raise Exception, 'Cant determine datasets path'
	
	datasets_path = datasets_path[0]
	
	datasets_files = os.listdir('datasets/%s' % datasets_path)
	
	datasets = []
	for dataset_file in datasets_files:
		M = re.search( '([0-9]+).in', dataset_file )
		if M:
			id = int(M.group(1))
			path = 'datasets/%s/%s' % (datasets_path,dataset_file)
			name = dataset_file[:-3] # without .in
			datasets.append( Dataset( id, path, name ) )
			
	datasets.sort(key=lambda d:d.id)
	
	return datasets
	
def load_dataset( dataset ):
	with open(dataset.path, 'rt') as file:
		try: 
			sys.stdin = file
		
			data = task.load_dataset()
			dataset.data = map( Data, enumerate(data) )
			
			for d in dataset.data:
				d.dataset = dataset
		
		finally:
			sys.stdin = sys.__stdin__
		
		return dataset

def solve_data( data ):
	return task.solve( data.data )
	
def prepare_state_file_binary(datas):
	data = [ [ '_' for d in D.data ] for D in datasets ]
	data = [ ' '.join(d) for d in data ]
	data = '\n'.join(data)
	with open(args.state.format(id=args.task_id), 'w') as f:
		print >>f, data
		
state_file_lock = multiprocessing.Lock()
def solve_data_binary( data ):
	answer = task.solve( data.data )
	with state_file_lock:
		with open(args.state.format(id=args.task_id), 'r+') as f:
			f.seek(data.i*2)
			f.write('#')
			f.close()
			
	return answer 
		
def save_dataset( dataset ):
	with open('out/%s.out' % dataset.name, 'wt') as file:
		try: 
			sys.stdout = file
			
			for d in dataset.data:
				task.save_answer(d.answer)
		
		finally:
			sys.stdout = sys.__stdout__
	

if __name__ == '__main__':
	import argparse
	parser = argparse.ArgumentParser(description='Solve some tasks.', formatter_class=argparse.ArgumentDefaultsHelpFormatter)
	parser.add_argument('task_id', help='the task id')
	parser.add_argument('-p', '--processes', type=int, help='number of executed processes', default=multiprocessing.cpu_count() )
	parser.add_argument('-c', '--config', help='config file', default='{id}.conf' )
	parser.add_argument('-s', '--state', help='state file', default='{id}.state' )
	parser.add_argument('-b', '--binary', help='binary task', default=False, action='store_true' )
	
	args = parser.parse_args()
	
	# Load task solution .py
	sys.path.append('./src/')
	task = imp.load_source('task', './src/%s.py' % args.task_id)
	
	
	datasets = find_datasets(args.task_id)
			
	pool = multiprocessing.Pool(args.processes)
	
	datasets = pool.map( load_dataset, datasets )
	
	# Flat data array
	datas = [ data
				for dataset in datasets
					for data in dataset.data ]
					
	
	for i,d in enumerate(datas):
		d.i = i
		
	print >>sys.stderr, 'Solving %d tests in %d datasets...' % (len(datas), len(datasets))

	
	if args.binary:
		prepare_state_file_binary( datasets )
		solve_data = solve_data_binary
			
		answers = pool.map( solve_data, datas )
			
		print >>sys.stderr, 'Solved. Saving...'
		for d, a in izip(datas, answers):
			d.answer = a
			
		
		pool.map( save_dataset, datasets )
								
		pool.close()
		print >>sys.stderr, 'Saved.'
	else:
		pool.close()
		state_content = [ '%s (%d) Score: %d' % (data.dataset.name, data.no, 0) for data in datas ] 
		
		with open(args.state.format(id=args.task_id), 'w') as f:
			print >>f, '\n'.join(state_content)
		
		
		Q = multiprocessing.Queue()
		S = multiprocessing.Semaphore(args.processes)
		N = 0
		
		def solve_data(data):
			with S:
				answer = task.solve( data.data )
			try:
				answer.next
			except:
				answer = [answer].__iter__()
				
			try:
				while True:
					with S:
						solution = answer.next()
						if solution != None:
							Q.put( (data.i, data.no, solution) )
						
			except StopIteration:
				pass
				
			Q.put( None )
			
		for data in datas:							
			p = multiprocessing.Process(target=solve_data, args=(data,))
			p.daemon = True
			p.start()
			N += 1
			
		while N > 0:
			ret = Q.get()
			if ret == None:
				N -= 1
				print >>sys.stderr, 'One process finished it`s work, %d left...' % N
				continue 
				
			i, no, solution = ret
			data = datas[i]
			data.answer = solution
			
			try:
				score = task.get_score(solution)
				print >>sys.stderr, 'Found new answer for dataset %s.in, test %d with score: %d' % (data.dataset.name, no, score)
				
				state_content[i] = '%s (%d) Score: %d' % (data.dataset.name, data.no, score)
				with open(args.state.format(id=args.task_id), 'w') as f:
					print >>f, '\n'.join(state_content)
					
			except:
				print >>sys.stderr, 'Found new answer for dataset %s.in, test %d' % (data.dataset.name, no)
			
			
			
			with S:
				save_dataset( datas[i].dataset )
			
			
	
