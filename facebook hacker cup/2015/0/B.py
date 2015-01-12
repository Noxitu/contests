#!/usr/bin/python

from sys import stdin

def line(): return stdin.readline().strip().split()
	
def iline(): return map( int, line() )

T, = iline()

for i in xrange(1,T+1):

	total = iline()
	N, = iline()
	meals = [ iline() for j in xrange(N) ]
	
	def ok():
		for S in xrange(2**N):
			B = ('0'*N+bin(S)[2:])[-N:]
		
			answer = [ sum( v[j] for v,b in zip(meals,B) if b == '1') for j in xrange(3) ]
			if answer == total:
				return True
				
		return False

	print 'Case #%d: %s' % (i, 'yes' if ok() else 'no')
