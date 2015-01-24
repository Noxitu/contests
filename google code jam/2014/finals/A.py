#!/usr/bin/python

from utils import line, iline

def test():
	n, = iline()
	M = [ line()[0] for i in xrange(2*n) ]

	def solve():
		rows = set()

		for m in M:
			rows.add(m)
		rows = list(rows)

		def valid():
			if len(rows) != 2: return False
			if sum( 1 for c in rows[0] if c == '1' ) != n: return False
			if any( a == b for a,b in zip(*rows) ): return False
			return True

		if not valid():
			print 'IMPOSSIBLE'
			return

		a = sum( 1 for i,c in enumerate(M[0]) if int(c) == i%2 ) / 2
		b = sum( 1 for i,c in enumerate(M) if int(c[0]) == i%2 ) / 2

		print min(a,n-a)+min(b,n-b)

	return solve
	
if __name__ == '__main__':
	T, = iline()
	for i in xrange(1, T+1):
		print 'Case #%d:' % i,
		test()()
