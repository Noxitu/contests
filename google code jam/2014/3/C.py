#!/usr/bin/python

from sys import stdin, stderr

def line():
	return map( int, stdin.readline().strip().split() )

def sline():
	return stdin.readline().strip().split()

def myline():
	l = sline()
	return [ l[0], int(l[1]) ]

class Person:
	def __init__(self):
		self.e = []
		self.l = []

class Crime(Exception):
	pass

T, = line()

for T in xrange(1,T+1):
	
	N, = line()
	actions = [ myline() for i in xrange(N) ]
	try:
		M = max(actions, key = lambda (a,i) : i)[1] + 1

		P = [ Person() for i in xrange(M) ]

		for t, (a,i) in enumerate(actions):
			if a == 'E':
				P[i].e.append( t )
			else:
				P[i].l.append( t )

		print actions

		needE = set()
		needL = set()

		for t, (a,i) in enumerate(actions):

			if a == 'E':
				P[i].e.pop(0)
				if (t,i) in needL:
					raise Crime()
			else:
				if (t,i) in needE:
					raise Crime()
				P[i].l.pop(0)

			if i == 0:
				if a == 'E':
					if len(needE) > 0:
						tx, j = min(needE)
						needE.remove( (tx, j) )
						if t > tx:
							raise Crime()

						actions[t][1] = j
				else:
					if len(needL) > 0:
						tx, j = min(needL)
						needL.remove( (tx, j) )
						if t > tx:
							raise Crime()

						actions[t][1] = j

			if i != 0:
				if a == 'E':
					need = len(P[i].e) > 0 and ( len(P[i].l) == 0 or P[i].e[0] < P[i].l[0] )

					if need:
						needL.add( ( P[i].e[0], i ) )

						
				else:
					need = len(P[i].l) > 0 and ( len(P[i].e) == 0 or P[i].l[0] < P[i].e[0] )

					if need:
						needE.add( ( P[i].l[0], i ) )
	

		P = [ [] for i in xrange(M) ]

		Entries = []
		Leaves = []

		for t, (a,i) in enumerate(actions):
			P[i].append( (a, t) )

		for p in P[1:]:
			if len(p)>0:
				if p[0][0] == 'L':
					Leaves.append(p[0][1])
				if p[-1][0] == 'E':
					Entries.append(p[-1][1])

		PL = [ t for a,t in P[0] if a == 'L' ]
		PE = [ t for a,t in P[0] if a == 'E' ]

		Entries = Entries + PE
		Entries.sort()
		Leaves.sort()

		print Entries, Leaves, PL

		people = 0
		for t in Entries[::-1]:
			if len(Leaves) > 0 and Leaves[-1] > t and (actions[t][1] == 0 or actions[Leaves[-1]][1] == 0):
				Leaves.pop()
			elif len(PL) > 0 and PL[-1] > t:
				PL.pop()
			else:
				people += 1

		print 'Case #%d: %d' % (T, people)
	except Crime:
		print 'Case #%d: CRIME TIME' % T

#	print >>stderr, 'Case #%d' % T

