#!/usr/bin/python

from sys import stdin

def line(): return stdin.readline().strip().split()
	
def iline(): return map( int, line() )

T, = iline()

def swaped(v,i,j):
	return v[:i]+v[j]+v[i+1:j]+v[i]+v[j+1:]

for i in xrange(1,T+1):
	row, = line()
	possible = [ row ]
	for a in xrange(len(row)):
		for b in xrange(a+1,len(row)):
			possible.append(swaped(row, a, b))
			
	possible = filter( lambda v : v[0] != '0' or v == '0', possible )

	print 'Case #%d: %s %s' % (i, min(possible), max(possible) )
