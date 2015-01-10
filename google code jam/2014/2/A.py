#!/usr/bin/python

from sys import stdin

def line():
	return map( int, stdin.readline().strip().split() )

def sline():
	return stdin.readline().strip().split()

T, = line()

for T in xrange(1,T+1):
	N, X = line()
	S = line()
	S.sort()

	A = 0
	while len(S) > 0:
		if S[0] + S[-1] <= X:
			S = S[1:-1]
		else:
			S = S[:-1]
		A += 1

	print 'Case #%d: %d' % (T, A)
