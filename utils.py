#!/usr/bin/python

import sys

_line = lambda : sys.stdin.readline()

def line(*args):
	L = _line().strip().split()
	L = zip( L, list(args) + [str]*(len(L)-len(args)) )
	return [ type(data) for data, type in L ]	
	
def iline(): return map( int, _line().strip().split() )
def fline(): return map( float, _line().strip().split() )

def setLineInput(input_file):
	global _line
	_line = lambda : input_file.readline()