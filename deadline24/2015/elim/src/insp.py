#!/usr/bin/python

import sys
from utils import *
	
	
def load_dataset():	
	T, = iline()
	
	for i in xrange(T):
		N, = iline()
		yield [ iline() for i in xrange(N) ]
		
		
def solve(data):
	data.sort(key=lambda(a,b):b)
	freeAt = (-2000000000,)
	n = 0
	for a, b in data:
		if max(freeAt) <= a:
			freeAt = min(freeAt), b
			n += 1
		elif min(freeAt) <= a:
			freeAt = max(freeAt), b
			n += 1
			
	return n
	
def save_answer(answer):
	print answer
	
if __name__ == '__main__':
	datasets = load_dataset()
	
	answers = map( solve, datasets )
	
	map( save_answer, answers )
	