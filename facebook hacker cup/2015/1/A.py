#!/usr/bin/python

from utils import line, iline

prev_prime = [ i for i in xrange(10000001) ]
answer = [ 0 for i in xrange(10000001) ]

for p in xrange(2,10000):
	if prev_prime[p] == p:
		w = 2*p
		while w < len(prev_prime):
			if prev_prime[w] == w:
				prev_prime[w] = p
			w += p

for p in xrange(2,10000001):
	q = p / prev_prime[p]
	answer[p] = answer[q] + (0 if prev_prime[q] == prev_prime[p] else 1)		


def test():
	a, b, k = iline()
	
	def solve():
		print sum( 1 for x in answer[a:b+1] if x == k )
		
	return solve
	
if __name__ == '__main__':
	T, = iline()
	for i in xrange(1, T+1):
		print 'Case #%d:' % i,
		test()()
