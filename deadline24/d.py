#!/usr/bin/python

from multiprocessing import Process

from sys import stdin, argv, exit
from time import time
import random


def CalculateSolution(Tests, U, M):
	TAB = [ (0, []) for u in xrange(U+1) ]
	for T in Tests:
		for u in xrange(U,-1, -1):
			TAB[u] = (TAB[u][0], TAB[u][1] + [[]])
			for m in xrange(1, M+1):
				if u-m >= 0:
					ans = TAB[u-m][0] + T.solutions[m][0]
					if ans > TAB[u][0]:
						TAB[u] = (ans, TAB[u-m][1] + [T.solutions[m][1]])

	ans = max(TAB, key=lambda (a,s):a)
	return ans

def SaveState(Tests, testNo):
	state = open('state/help%.2d.state' % testNo, 'w' )
	for T in Tests:
		for n, s in T.solutions:
			print >>state, n, len(s), ' '.join(map(str,s))

	state.close()

def SaveAnswer( answer, removeds, testNo):
	outFile = open('out/help%.2d.ans' % testNo, 'w' )

	print >>outFile, answer
	for r in removeds:
		print >>outFile, ' '.join(map(str,r)) if len(r) > 0 else 0

	outFile.close()

def UpdateSolutionUsingRandomEdges(T, no):
	Color = range(T.V+1)
	E = {}
	for i in range(1, T.V+1):
		Color[i] = 0
		E[i] = []

	edges = [ e for e in T.edges ]
	random.shuffle(edges)

	for n,a,b in edges[no:]:
		E[a].append(b)
		E[b].append(a)

	Colors = []
	for i in range(1,T.V+1):
		if Color[i] == 0:
			n = 1
			Color[i] = i
			Q = [i]
			while len(Q) > 0:
				a, Q = Q[0], Q[1:]
				for b in E[a]:
					if Color[b] == 0:
						Color[b] = i
						Q.append(b)
						n += 1
			Colors.append(n)

	answer = 0
	for n in Colors:
		answer += n*(T.V-n)

	answer /= 2

	if T.solutions[no][0] < answer:
		removed = [ n for n,a,b in edges[:no] ]
		removed.sort()
		T.solutions[no] = (answer, removed)

def mysum(a):
	b = []
	for c in a:
		b += c
	return b

def Funkcja(T, M):
	try:
		T.EE
	except:
		T.EE = {}
		for i in range(1, T.V+1):
			T.EE[i] = []

		for n,a,b in T.edges:
			T.EE[a].append( (b, n) )
			T.EE[b].append( (a, n) )
	
	
	a = random.randint(1, T.V)

	Visited = set()
	Near = { a: [] }

	while len(Near) > 0:
		a = random.choice(Near.keys())
		Visited.add(a)

		for b in T.EE[a]:
			Near[b[0]] = Near.get(b[0], []) + [b[1]]

		removed = set(mysum(Near.values()))
		no = len(removed)

		print removed, no

		if no > M:
			continue

		Color = range(T.V+1)
		E = {}
		for i in range(1, T.V+1):
			Color[i] = 0
			E[i] = []

		for n,a,b in T.edges:
			if n not in removed:
				E[a].append(b)
				E[b].append(a)

		Colors = []
		for i in range(1,T.V+1):
			if Color[i] == 0:
				n = 1
				Color[i] = i
				Q = [i]
				while len(Q) > 0:
					a, Q = Q[0], Q[1:]
					for b in E[a]:
						if Color[b] == 0:
							Color[b] = i
							Q.append(b)
							n += 1
				Colors.append(n)

		answer = 0
		for n in Colors:
			answer += n*(T.V-n)

		answer /= 2

		if T.solutions[no][0] < answer:
			removed = list(removed)
			removed.sort()
			T.solutions[no] = (answer, removed)

	

def Licz(testNo):
	bestAnswer = 0
	try:
		outFile = open('out/help%.2d.ans' % testNo )
		bestAnswer = int(outFile.readline())
		outFile.close()
	except:
		pass

	inFile = open('t/help%.2d.in' % testNo )

	def readiline():
		return map(int, inFile.readline().strip().split() )

	P, U, M = readiline()


	class Test: pass

	Tests = range(P)

	for i in Tests:
		T = Tests[i] = Test()
		T.V, T.E = readiline()
		T.edges = [ readiline() for l in xrange(T.E) ]
		T.solutions = [ (0, []) for i in xrange(M+1) ]

	lastMerge = 0
	while True:
		for T in Tests:

			x = min(M, T.E-1)
			x = random.randint(1, x)
			UpdateSolutionUsingRandomEdges(T, x)
			#Funkcja(T, M)



		now = time()
		if now - lastMerge > 5:
			lastMerge = now

			SaveState(Tests, testNo)
			answer, removeds = CalculateSolution(Tests, U, M)
			
			if answer > bestAnswer:
				print '[%.2d:%.2d:%.2d] Found new best for %d: %d' % ( (now/3600+1)%24, now/60%60, now%60, testNo, answer)	
				bestAnswer = answer
				SaveAnswer( answer, removeds, testNo )


test = range(1,11)
if len(argv) > 1 and argv[1] == 'dbg':
	test = range(0,1)

for i in test:
	p = Process(target=Licz, args=(i,))
	p.start()






