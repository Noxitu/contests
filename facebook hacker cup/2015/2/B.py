#!/usr/bin/python

from utils import line, iline, fline

def test():
	p,= fline()
	
	def solve():
		answer = 0
		P = 0
		prev = 0
		for t in xrange(1, 10000):
			prev = P ** 20
			P = P + (1-P) * p
			answer += t * (P ** 20 - prev)
		
		print '%.5f' % answer
		
	return solve
	
if __name__ == '__main__':
	T, = iline()
	for i in xrange(1, T+1):
		print 'Case #%d:' % i,
		test()()
