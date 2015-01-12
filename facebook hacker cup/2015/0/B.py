#!/usr/bin/python

from utils import line, iline
from itertools import combinations

def test():
	total = iline()
	N, = iline()
	meals = [ iline() for j in xrange(N) ]
	
	def solve():
		Q = ( map( sum, zip(*m) ) == total for n in xrange(0, N+1) for m in combinations(meals, n) )
		print 'yes' if any(Q) else 'no'
		
	return solve
	
if __name__ == '__main__':
	T, = iline()
	for i in xrange(1, T+1):
		print 'Case #%d:' % i,
		test()()
