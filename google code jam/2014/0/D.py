#!/usr/bin/python

from sys import stdin

def readiline():
	return map( int, stdin.readline().strip().split() )

def readfline():
	return map( float, stdin.readline().strip().split() )

T, = readiline()

for i in xrange(1,T+1):
	n, = readiline()
	naomi = readfline()
	ken = readfline()

	naomi.sort()
	ken.sort()

	deceitfulWarScore = warScore = 0
	kenI = 0
	for naomiI in xrange(n):
		if naomi[naomiI] > ken[kenI]:
			deceitfulWarScore += 1
			kenI += 1

	kenI = n-1
	for naomiI in xrange(n-1,-1,-1):
		if naomi[naomiI] > ken[kenI]:
			warScore += 1
		else:
			kenI -= 1
			
	print 'Case #%d: %d %d' % (i, deceitfulWarScore, warScore)
