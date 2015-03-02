#!/usr/bin/python

import sys, random
from utils import *

names = []
shapes = {}
joins = {}

with open('dev.tiles') as f, stdinSwapper(f):
	N, = iline()
	for i in xrange(N):
		H, W = iline()
		name, = line()
		matrix = [ line()[0] for i in xrange(H) ]
		lists = {'.':[], 'o':[], '#':[]}
		shape = [(0,0)]
		for y, L in enumerate(matrix):
			for x, c in enumerate(L):
				lists[c].append((x, y))
		
		assert len(lists['o']) == 1
		(dx, dy), = lists['o']
		
		for x,y in lists['#']:
			shape.append((x-dx, y-dy))
			
		names.append(name)
		shapes[ (name, 0) ] = shape
		for r in xrange(1,4):
			shapes[ (name, r) ] = [ (-y,x) for x,y in shapes[ (name,r-1) ] ]
			
"""def can_join(A,B,dx,dy):
	ok = False
	for x1, y1 in A:
		for x2, y2 in B:
			if (x1,y1) == (x2+dx,y2+dx):
				return False
			if abs(x1-dx-x2)+abs(y1-dy-y2) == 1:
				ok = True
	return ok
	
for id1, A in shapes.items():
	_j = joins[ id1 ] = set()
	for id2, B in shapes.items():
		for dx in xrange(-11, 12):
			for dy in xrange(-11, 12):
				if can_join(A,B,dx,dy):
					_j.add((id2, (dx, dy)))"""
					
				
def load_dataset():	
	T, = iline()
	
	for i in xrange(T):
		H,W = iline()
		yield H,W,[ list(line()[0]) for i in xrange(H) ]
		
		
def solve((H,W,D)):
	def tiles((id, (dx, dy))):
		for x, y in shapes[id]:
			yield x+dx, y+dy
			
	def walls(tile):
		for dx, dy in [ (1,0), (0,1), (-1,0), (0,-1) ]:
			for x, y in tiles(tile):
				yield x+dx, y+dy
			
	def count(tile, c):
		n = 0
		for x, y in tiles(tile):
			_c = D[y][x] if x >= 0 and x < W and y >= 0 and y < H else '#'
			if _c == c or _c in c:
				n += 1
		return n
		
	def countWalls(tile):
		n = 0
		for x,y in walls(tile):
			_c = D[y][x] if x >= 0 and x < W and y >= 0 and y < H else '#'
			if _c == '@':
				n += 1
		return n
		
	def apply(tile, c):
		for x, y in tiles(tile):
			assert D[y][x] in ['.','X']
			D[y][x] = c
			
	
	score = 0
	answer = []
	possible = ( (id, (x,y)) for id in shapes for x in xrange(W) for y in xrange(H) )
	possible = [ (tile, count(tile, 'X'), 0) for tile in possible if count(tile, '#') == 0 ]
	
	
	try:
		while True:
			garden = house = None
			
			if not possible:
				break
				
			garden = max( possible, key=lambda (tile, trees, walls) : trees )
				
			gard_tile, gard_trees, gard_walls = garden
			gard_id, (gard_x, gard_y) = gard_tile
			garden_set = set(tiles(gard_tile))
			garden_walls = set(walls(gard_tile))
			
			def isHouseOK(tile):
				house_set = set(tiles(tile))
				return not garden_set & house_set and garden_walls & house_set
			
			try:
				house = min(( 
					(tile, trees, walls) 
						for tile, trees, walls 
						in possible 
						if isHouseOK(tile) )
					, key = lambda (tile, trees, walls) : trees+walls )
			except:
				possible.remove( garden )
				continue
			
			house_tile, house_trees, house_walls = house
			house_id, (house_x, house_y) = house_tile

			value = 3 + 2*gard_trees - 2*house_trees - 2*house_walls
			if value < 0:
				possible.remove( garden )
				continue
				
			answer.append( [house_id+(house_x, house_y), gard_id+(gard_x, gard_y)] )
			apply( gard_tile, '*' )
			apply( house_tile, '@' )
			score += value
			
			possible = [ (tile, trees, countWalls(tile)) for tile,trees,_ in possible if count(tile, ('*','@')) == 0 ]
			
			yield score, answer
			
	except ValueError:
		pass
	
	yield score, answer
	
def save_answer((s,A)):
	print len(A), s
	for L in A:
		for a,b,c,d in L:
			print a,b,c+1,d+1,
		print
	
def get_score((s,A)):
	return s
	
	
if __name__ == '__main__':
	datasets = load_dataset()
	
	answers = map( solve, datasets )
	
	map( save_answer, answers )
	