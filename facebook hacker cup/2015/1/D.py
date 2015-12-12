#!/usr/bin/python

from utils import line, iline

def test():
	N, = iline()
	P = [0] + iline()
	
	def solve():
		C = [ [] for i in P ]
		RET = [ None for i in P ]
		
		for i, p in enumerate(P):
			if p != 0:
				C[p].append(i)
	
		sorted = [1]
		for p in sorted:
			for c in C[p]:
				sorted.append(c)
				
	
		for p in sorted[::-1]:
			K = max( [0] + [ RET[c][0] for c in C[p] ] )
			
			possible = []
			for k in xrange(1, K+3):
				v = k + sum( v1 if kc != k else v2 for kc, v1, v2 in (RET[c] for c in C[p]) )
				possible.append((k, v)) 
				
			possible.sort(key=lambda (k,v) : v)
			RET[p] = (possible[0][0], possible[0][1], possible[1][1])
			
		print RET[1][1]#, max( k for k,a,b in RET[1:] )
		
	return solve
	
if __name__ == '__main__':
	T, = iline()
	for i in xrange(1, T+1):
		print 'Case #%d:' % i,
		test()()
