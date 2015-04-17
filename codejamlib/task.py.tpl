#!/usr/bin/python

from utils import line, iline

def test():
	pass # read input here
	
	def solve():
		pass # calculate and print answer here
		
	return solve
	
if __name__ == '__main__':
	T, = iline()
	for i in xrange(1, T+1):
		print 'Case #%d:' % i,
		test()()
