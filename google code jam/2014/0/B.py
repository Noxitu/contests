#!/usr/bin/python

from sys import stdin

def readiline():
	return map( int, stdin.readline().strip().split() )

def readfline():
	return map( float, stdin.readline().strip().split() )

T, = readiline()

for i in xrange(1,T+1):
	C, F, X = readfline()
	
	rise = 2.0
	time = 0.0
	
	while X > C+X*rise/(rise+F):
		time += C/rise
		rise += F
	
	print 'Case #%d: %.7f' % (i, time+X/rise)
