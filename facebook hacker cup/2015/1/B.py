#!/usr/bin/python

from utils import line, iline

def test():
	n, = iline()
	words = [ line()[0] for i in xrange(n) ]
	
	def solve():
		DICT = {}
		
		answer = 0
		for w in words:
			d = DICT
			
			need_add = True
			for c in w:
				if need_add:
					answer += 1
					
				if c not in d:
					need_add = False
					d[c] = {}
					
				d = d[c]
		
		print answer
		
	return solve
	
if __name__ == '__main__':
	T, = iline()
	for i in xrange(1, T+1):
		print 'Case #%d:' % i,
		test()()
