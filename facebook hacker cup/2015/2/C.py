#!/usr/bin/python

from utils import line, iline
from sys import stderr

class TrieElement:
	def __init__(d, *args):
		d.size, d.tree, d.height, d.name = args
		
def test():
	N, K = iline()
	words = [ line()[0]+'$' for i in xrange(N) ]
	
	def solve():
		DICT = TrieElement(0, {}, 0, '')
		
		for w in words:
			d = DICT
			
			d.size += 1
			for c in w:
				if c not in d.tree:
					name = d.name + c
					d.tree[c] = TrieElement(0, {}, d.height+(1 if c != '$' else 0), name)
				
				d = d.tree[c]
					
				d.size += 1
				
		solutions = [ (d.height,d) for d in DICT.tree.values() ]
		
		while len(solutions) < K:
			val, parent = min( solutions, key = lambda (v, d) : (d.size == 1, d.height, -len(d.tree)) )
			solutions.remove( (val, parent) )
			
			if len(parent.tree) == 1:
				solutions.extend([ (val, d) for d in parent.tree.values() ])
			else:
				solutions.extend([ (d.height, d) for d in parent.tree.values() ])
			
		#print >>stderr, [ d.name for v, d in solutions[:K] ]
		print sum( v for v, d in solutions[:K] )
		
		
	return solve
	
if __name__ == '__main__':
	T, = iline()
	for i in xrange(1, T+1):
		print 'Case #%d:' % i,
		test()()
