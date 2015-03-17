#!/usr/bin/python

import sys

def getline():
	return sys.stdin.readline()

def line(*args):
	L = getline().strip().split()
	L = zip( L, list(args) + [str]*(len(L)-len(args)) )
	return [ type(data) for data, type in L ]	
	
def iline(): return map( int, getline().strip().split() )
def fline(): return map( float, getline().strip().split() )

class stdinSwapper:
	def __init__(self, new_stdin):
		self.new_stdin = new_stdin
		
	def __enter__(self):
		self.old_stdin, sys.stdin = sys.stdin, self.new_stdin
		
	def __exit__(self, type, value, traceback):
		sys.stdin = self.old_stdin