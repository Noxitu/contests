#!/usr/bin/python

from sys import stdin

def line():
	return map( int, stdin.readline().strip().split() )

line()

print 'Case #1:'

R, N, M, K = line()

for i in xrange(R):
	values = line()

	def atLeast(a,b):
		c = 0
		while a % b == 0:
			a /= b
			c += 1
		return c

	X = dict( (p, max( atLeast(val, p) for val in values )) for p in [2,3,4,5] )
	Y = dict( (p, min( atLeast(val, p) for val in values )) for p in [2,3,4,5] )
	Z = dict( (p, sum( atLeast(val, p) for val in values )) for p in [2,3,4,5] )
	
	Z = [ (i, str(p)) for p,i in Z.items() ]

	x2, x3, x4, x5 = X.values()
	if x5 + x3 == 3:
		print '3'*x3+'5'*x5
	if x5 + x3 == 2:
		if x2 > 1:
			print '3'*x3+'5'*x5+'4'
		if x2 <= 1:
			print '3'*x3+'5'*x5+max(Z)[1]
	if x5 + x3 == 1:
		if x4 == 2:
			print '3'*x3+'5'*x5+'44'
		elif x2 > 3:
			print '3'*x3+'5'*x5+'42'
		else:
			print '3'*x3+'5'*x5+'22'
	if x5+x3 == 0:
		if x2 == 6:
			print '444'
		if x2 == 5:
			print '442'
		if x2 <= 4 and x2 >= 2:
			print '422'
		if x2 <= 1:
			print '235'



