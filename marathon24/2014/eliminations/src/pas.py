#!/usr/bin/python

UP = 1
DOWN = 2
LEFT = -1
RIGHT = 1

import sys
from utils import *
	
def load_dataset():	
	yield iline()
		
def solve((k,m,n)):
	T = []
	done = 0
	now = min(m-done, k-1)
	
	# dociagnij wszystko do zwrotnic
	for i in xrange(k):
		T.append( [DOWN,UP,RIGHT,m,LEFT,n] )
		
	# bialy wciaga max k-1, czarny calosc
	T.append( [DOWN,UP,RIGHT,now,LEFT,n] )
	
	# czarny przeciaga przez zwrotnice
	for i in xrange(n+2):
		T.append( (UP,UP,0,0,LEFT,n) )	

	# tyle bialych jest na prawo
	done += now
	while done < m:
		now = min(m-done, k)
		
		T.append( (DOWN, DOWN, 0, 0, RIGHT, m-done) )
		
		for i in xrange(now+n):
			T.append( (DOWN, DOWN, RIGHT,m+n+1, RIGHT, m-done) )
			
		for i in xrange(k-now+1):
			T.append( (DOWN, DOWN, RIGHT,done+n+1+now, RIGHT, now) )
		
		T.append( (DOWN, DOWN, RIGHT, done+n+1, RIGHT, 0) )
		
		for i in xrange(n+1):
			T.append( (UP, UP, LEFT, 0, LEFT, n+done+1 ) )
			
		T.append( (UP, UP, 0, 0, LEFT, n ) )
		T.append( (UP, DOWN, LEFT, 0, LEFT, n ) )
		
		for i in xrange(k):
			T.append( (UP, DOWN, 0, 0, LEFT, n ) )
		
		done += now
		
	for i in xrange(now+1):
		T.append((DOWN, DOWN, RIGHT, m, 0, 0))
		
	return T
	
def save_answer(T):
	for a,b,c,d,e,f in T:
		print a,b,c,d,e,f
	print 0
	
if __name__ == '__main__':
	datasets = load_dataset()
	
	answers = map( solve, datasets )
	
	map( save_answer, answers )
	