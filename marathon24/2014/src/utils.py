import sys

def line():
	return map( lambda v : v.strip(), sys.stdin.readline().strip().split(' ') )
	
def iline():
	return map( int, line() )
	
