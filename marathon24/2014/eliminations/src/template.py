#!/usr/bin/python

import sys
from utils import *
	
	
def load_dataset():	
	T, = iline()
	
	for i in xrange(T):
		yield iline()
		
		
def solve(T):
	return 42
	
def save_answer(answer):
	print answer
	
if __name__ == '__main__':
	datasets = load_dataset()
	
	answers = map( solve, datasets )
	
	map( save_answer, answers )
	