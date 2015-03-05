#!/usr/bin/python

import sys, random
from utils import *

	
def load_dataset():	
	C, Q = iline()
	mx, my = iline()
	D = [ iline() for i in xrange(C) ]
	D = [ (id,x-mx,y-my,b,e,d,s) for id,x,y,b,e,d,s in D ]
	T0 = sum( max(abs(x)+abs(y), b)+s+abs(x)+abs(y) for id,x,y,b,e,d,s in D )
	
	return [ (Q, D, C, T0) ]

def solve((Q, D, C, T0)):
	T = 0
	left = set( d for d in D )
	answer = []
	
	while left:
		answer.append([])
		vx = vy = t = 0
		q = Q
		try:
			while left:
				arrival = lambda (id,x,y,b,e,d,s) : max( t + abs(x-vx) + abs(y-vy), b )
				id,x,y,b,e,d,s = client = min( (d for d in left if arrival(d) <= d[4] and d[5] <= q), key = arrival )
				
				left.remove(client)
				
				T += abs(x-vx) + abs(y-vy)
				t = arrival(client) + s
				
				vx, vy = x, y
				#answer[-1].append((id, t-s))
				answer[-1].append(id)
				
				q -= d
		except ValueError:
			pass
		T += abs(vx) + abs(vy)
			
	return T, answer
			
def save_answer((T, answer)):
	print len(answer), T
	for L in answer:
		for id in L:
			print id,
		print
	
			
		
"""	
def score(input, output):
	with open(input) as fi, stdinSwapper(fi):
		
		
	T0 = sum( max(abs(x)+abs(y), b)+s+abs(x)+abs(y) for id,x,y,b,e,d,s in D.values() )
	
	with open(output) as fo, stdinSwapper(fo):
		K, T1 = iline()
		visited = set()
		T = 0
		
		for i in xrange(K):
			t = vx = vy = q = 0
			for id in iline():
				id, x, y, b, e, d, s = D[id]
				
				t = max( t + abs(x-vx) + abs(y-vy), b ) + s
				T += abs(x-vx) + abs(y-vy)
				vx, vy = x, y
				
				if id in visited:
					raise Exception('Revisiting client')
					
				visited.add(id)
				
				if t > e+s:
					raise Exception('Truck is too late')
					
				q += d
				
			T += abs(vx) + abs(vy)
				
			if q > Q:
				raise Exception('Truck is too small')
			
		if len(visited) != C:
			raise Exception('Not all clients are satisfied')
				
		if T != T1:
			raise Exception('Invalid score')
		
	return 
					"""
					
if __name__ == '__main__':
	datasets = load_dataset()
	
	answers = map( solve, datasets )
	
	map( save_answer, answers )