
from codejam import *
    

def problem():
	N, M = readiline()
	V = [ readline()[0] for i in xrange(N) ]
	G = dict( (i,[]) for i in V)

	for i in xrange(M):
		A, B = readiline()
		G[V[A-1]].append(V[B-1])
		G[V[B-1]].append(V[A-1])
		

	V = set(V)
	answer = []
	queue = set()

	def force(v):
		

	def solve(v):
		V.remove(v)
		answer.append(v)
		queue = queue.union(set(G[v])).intersect(V)

		need = []
		for u in G[v]:
			
		
		
	solve(min(V))

	print answer, V

codejam(problem)
        
        
        

