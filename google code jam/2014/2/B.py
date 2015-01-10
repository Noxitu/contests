#!/usr/bin/python

from sys import stdin

def line():
	return map( int, stdin.readline().strip().split() )

def sline():
	return stdin.readline().strip().split()

T, = line()

for T in xrange(1,T+1):
	N, = line()
	S = line()

	A = 0
	while len(S) > 0:
		i, _ = min(enumerate(S), key = lambda (i,v) : v)
		A += min(i, len(S)-i-1)
		S = S[:i]+S[i+1:]

	print 'Case #%d: %d' % (T, A)
