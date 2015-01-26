#!/usr/bin/python

from utils import line, iline
from collections import deque

def test():
	n, = iline()
	P = iline()
	
	def solve():
	
		def solve_part( Q, l ):
			Q = deque(Q)
			r = l
			while Q:
				if Q[0] == l-1:
					l = Q.popleft()
				elif Q[0] == r+1:
					r = Q.popleft()
				elif Q[-1] == l-1:
					l = Q.pop()
				elif Q[-1] == r+1:
					r = Q.pop()
				else:
					return False
			return True
			
		print 'yes' if solve_part(P[1:], P[0]) or solve_part(P[:-1], P[-1]) else 'no'
		
	return solve
	
if __name__ == '__main__':
	T, = iline()
	for i in xrange(1, T+1):
		print 'Case #%d:' % i,
		test()()
