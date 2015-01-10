#!/usr/bin/python

from sys import stdin

def line():
	return map( int, stdin.readline().strip().split() )

T, = line()

for i in xrange(1,T+1):
	row, = line()
	
	possible = [ line() for j in xrange(4) ][row-1]
	
	row, = line()
	
	possible2 = [ line() for j in xrange(4) ][row-1]

	possible = [ x for x in possible if x in possible2 ]

	case = 'Case #%d:' % i

	if len(possible) == 0:
		print case, 'Volunteer cheated!'

	elif len(possible) > 1:
		print case, 'Bad magician!'

	else:
		print case, possible[0]
