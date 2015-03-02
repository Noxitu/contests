#!/usr/bin/python
# -*- coding: utf-8 -*-

import imp
import os, errno
import re
import multiprocessing
import sys, time

from itertools import izip, repeat
from solve_utils import *	

if __name__ == '__main__':
	args = parse_args()
	
	task = load_task_module(args.task_id)
	
	datasets = find_datasets(args.task_id)
			
	pool = multiprocessing.Pool(args.processes)
	
	print >>sys.stderr, 'Loading %d datasets...' % len(datasets)
	
	datasets = pool.map( load_dataset, izip( datasets, repeat(task.load_dataset) ) )
	pool.close()
	
	datas = flattenDatasets(datasets)
		
	print >>sys.stderr, 'Solving %d tests...' % len(datas)
	
	lock = multiprocessing.Semaphore(args.processes)
	queue = multiprocessing.Queue()
	
	state_content = [ '%s (%d) Score: %d' % (data.dataset.name, data.no, 0) for data in datas ] 
	with open(args.state.format(id=args.task_id), 'w') as f:
		print >>f, '\n'.join(state_content)
	
	def timer_func():
		while True:
			time.sleep(15)
			queue.put( 'timer()' )
			
	timer = multiprocessing.Process(target=timer_func)
	timer.daemon = True
	timer.start() 
	
	def run(data):
		best_score = None
		try:
			lock.acquire()
			for answer in task.solve(data.data):
				if answer is not None:
					score = task.get_score(answer)
					if score > best_score:
						best_score = score
						queue.put( (data.i, score, answer) )
					
				lock.release()
				lock.acquire()
		finally:
			lock.release()
			queue.put( 'finished' )
			
	def solve(data):
		p = multiprocessing.Process(target=run, args=(data,))
		p.daemon = True
		p.start()
		
	map( solve, datas )
	
	N = len(datas)
	changed = set()
	
	while N or changed:
		ret = queue.get()
		
		if ret == 'finished':
			N -= 1
			print >>sys.stderr, 'One process finished it`s work, %d left...' % N
			if N == 0:
				queue.put('timer()')
			continue 
				
		if ret == 'timer()':
			for dataset in changed:
				save_dataset(( dataset, task.save_answer ))
			changed &= set()
			continue
			
		i, score, answer = ret
		data = datas[i]
		data.answer = answer
		changed.add( data.dataset )
		
		print >>sys.stderr, 'Found new answer for dataset %s.in, test %d with score: %d' % (data.dataset.name, data.no, score)
		
		state_content[i] = '%s (%d) Score: %d' % (data.dataset.name, data.no, score)
		with open(args.state.format(id=args.task_id), 'w') as f:
			print >>f, '\n'.join(state_content)
			
	print >>sys.stderr, 'All done. Exiting...'
	