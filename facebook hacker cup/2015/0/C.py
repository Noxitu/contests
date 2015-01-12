#!/usr/bin/python

from sys import stdin
from collections import deque
from utils import line, iline

def next_dir(dx,dy):
	return (-dy,dx)
	
DIRS = {'>' : (1,0), 'v': (0,1), '<': (-1,0), '^':(0,-1) }

def test():
	H, W = iline()
	M = [['#']*(W+2)]+[ list('#'+line()[0]+'#') for j in xrange(H) ]+[['#']*(W+2)]
	
	X = []
	for y, L in enumerate(M):
		for x, v in enumerate(L):
			if v == 'S':
				S = x,y
			elif v == 'G':
				G = x,y
			elif v in '><^v':
				dx, dy = DIRS[v]
				X.append([x,y,(dx,dy)])
	
	M[S[1]][S[0]] = '.'
	M[G[1]][G[0]] = '.'
	
	MM = []
	for j in xrange(4):
		mm = []
		MM.append(mm)
		for L in M:
			mm.append(list(L))
		
		for xx in X:
			x,y,(dx, dy) = xx
			x += dx
			y += dy
			
			while M[y][x] == '.':
				mm[y][x] = '#'
				x += dx
				y += dy
			
			xx[2] = next_dir(dx,dy)
	
		
	curr_layer = [S]
	next_layer = []
	dist = 0
	MM[0][S[1]][S[0]] = '*'
	answer = None
	
	while curr_layer:
		dist += 1
		for bx,by in curr_layer:
			if (bx,by) == G:
				next_layer = []
				answer = dist-1
				break
		
			for dx, dy in DIRS.values():
				x = bx+dx
				y = by+dy
				if MM[dist%4][y][x] == '.':
					next_layer.append( (x,y) )
					MM[dist%4][y][x] = '*'
					
		curr_layer = next_layer
		next_layer = []
					

	try:
		print '%d' %answer
	except:
		print 'impossible'
		
		
