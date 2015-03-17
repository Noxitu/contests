#!/usr/bin/python
# -*- coding: utf-8 -*-

import imp
import os, errno
import re
import multiprocessing
import sys

from itertools import izip

class stdinSwapper:
	def __init__(self, new_stdin):
		self.new_stdin = new_stdin
		
	def __enter__(self):
		self.old_stdin, sys.stdin = sys.stdin, self.new_stdin
		
	def __exit__(self, type, value, traceback):
		sys.stdin = self.old_stdin
		
class stdoutSwapper:
	def __init__(self, new_stdout):
		self.new_stdout = new_stdout
		
	def __enter__(self):
		self.old_stdout, sys.stdout = sys.stdout, self.new_stdout
		
	def __exit__(self, type, value, traceback):
		sys.stdout = self.old_stdout

class Data:
	def __init__(self, (no, data)):
		self.i = self.answer = None
		self.no = no
		self.data = data

class Dataset:
	def __init__(self, id, path, name):
		self.id = id
		self.path = path
		self.name = name
		
def flattenDatasets(datasets):
	datas = [ data for dataset in datasets for data in dataset.data ]			
	
	for i,d in enumerate(datas):
		d.i = i
		
	return datas
		

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
	
def load_dataset((dataset, loader)):
	with open(dataset.path, 'rt') as file, stdinSwapper(file):
		data = loader()
		dataset.data = map( Data, enumerate(data) )
		
		for d in dataset.data:
			d.dataset = dataset
		
		return dataset
	
def save_dataset((dataset, saver)):
	if all( d.answer != None for d in dataset.data ):
		with open('out/%s.ans' % dataset.name, 'wt') as file, stdoutSwapper(file):
			for d in dataset.data:
				saver(d.answer)
	
def parse_args():
	try:
		os.mkdir('out')
	except OSError as e:
		if e.errno != errno.EEXIST:
			raise e
			
	import argparse
	parser = argparse.ArgumentParser(description='Solve some tasks.', formatter_class=argparse.ArgumentDefaultsHelpFormatter)
	parser.add_argument('task_id', help='the task id')
	parser.add_argument('-p', '--processes', type=int, help='number of executed processes', default=multiprocessing.cpu_count() )
	#parser.add_argument('-c', '--config', help='config file', default='{id}.conf' )
	parser.add_argument('-s', '--state', help='state file', default='{id}.state' )
	
	return parser.parse_args()
	
def load_task_module(task_id):
	print >>sys.stderr, 'Loading solution module...'
	
	sys.path.append('./src/')
	task = imp.load_source('task', './src/%s.py' % task_id)
	
	return task

