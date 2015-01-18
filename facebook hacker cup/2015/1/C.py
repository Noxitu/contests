#!/usr/bin/python

from utils import line, iline

def test():
	A, B = map( int, line()[0].split('-') )
	
	
	def solve():
	
		def calculate( allowed ):
			M = [ [ 0 for b in xrange(B+1) ] for a in xrange(A+1) ]
			M[0][0] = 1
			
			for a in xrange(A+1):
				for b in xrange(B+1):
					if allowed(a,b) and (a,b) != (0,0):
						val = 0
						
						if a > 0:
							val += M[a-1][b]
						if b > 0:
							val += M[a][b-1]
							
						M[a][b] = val % 1000000007
					
			return M[A][B]
		
		print calculate( lambda a,b : a > b ), calculate( lambda a,b : a <= b or b == B )
		
	return solve
	
if __name__ == '__main__':
	T, = iline()
	for i in xrange(1, T+1):
		print 'Case #%d:' % i,
		test()()
