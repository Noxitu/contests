#!/usr/bin/python

from utils import line, iline

def swaped(v,i,j):
	return v[:i]+v[j]+v[i+1:j]+v[i]+v[j+1:]

def test():
	row, = line()
	
	def solve():
		possible = [ row ]
		for a in xrange(len(row)):
			for b in xrange(a+1,len(row)):
				possible.append(swaped(row, a, b))
				
		possible = filter( lambda v : v[0] != '0' or v == '0', possible )

		print '%s %s' % (min(possible), max(possible) )
	
	return solve
	
if __name__ == '__main__':
	T, = iline()
	for i in xrange(1, T+1):
		print 'Case #%d:' % i,
		test()
