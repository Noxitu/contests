#!/usr/bin/python
# -*- coding: utf-8 -*-

import imp
import multiprocessing
import sys

from itertools import izip, repeat
from solve_utils import *
	
def prepare_state_file_binary(datas):
	data = [ [ '_' for d in D.data ] for D in datasets ]
	data = [ ' '.join(d) for d in data ]
	data = '\n'.join(data)
	with open(args.state.format(id=args.task_id), 'w') as f:
		print >>f, data
		
state_file_lock = multiprocessing.Lock()
def solve( data ):
	answer = task.solve( data.data )
	with state_file_lock:
		with open(args.state.format(id=args.task_id), 'r+') as f:
			f.seek(data.i*2)
			f.write('#')
			f.close()
			
	return answer 	
	

if __name__ == '__main__':
	args = parse_args()
	
	task = load_task_module(args.task_id)
	
	datasets = find_datasets(args.task_id)
			
	pool = multiprocessing.Pool(args.processes)
	
	print >>sys.stderr, 'Loading %d datasets...' % len(datasets)
	
	datasets = pool.map( load_dataset, izip( datasets, repeat(task.load_dataset) ) )
	
	datas = flattenDatasets(datasets)
		
	print >>sys.stderr, 'Solving %d tests...' % len(datas)
	
	prepare_state_file_binary( datasets )
	answers = pool.map( solve, datas )
		
	print >>sys.stderr, 'Solved. Saving...'
	for d, a in izip(datas, answers):
		d.answer = a	
	
	pool.map( save_dataset, izip( datasets, repeat(task.save_answer) ) )
							
	pool.close()
	print >>sys.stderr, 'Saved.'
