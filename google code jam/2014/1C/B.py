from codejam import *

silnia = [1]
MOD = 1000000007

for i in xrange(1,101):
	silnia.append( silnia[-1]*i % MOD )

def problem():
	N, = readiline()
	S = readline()
	R = []
	
	before = set()
	after = set()

	for s in S:
		lol = False
		for a,b in zip(s[0]+s, s+s[-1]):
			if a != b:
				lol = True
				if a in before:
					print 0
					return
				else:
					before.add(a)

				if b in after:
					print 0
					return
				else:
					after.add(b)
		if lol and s[0] == s[-1]:
			print 0
			return

		R.append( (s[0], s[-1]) )

	simple = dict()
	canSkip = set()
	for a,b in R:
		if a != b:
			canSkip.add(a)
			canSkip.add(b)
		if a == b:
			simple[a] = simple.get(a,0)+1

	
	R = [ (a,b) for a,b in R if a != b ]

	def CutOne():
		for a,b in R:
			for c,d in R:
				if b == c:
					R.remove((a,b))
					R.remove((c,d))
					R.append((a,d))
					return True
		return False

	try:
		while CutOne():
			pass
	except:
		print 0
		return
					
	for a in simple:
		if a not in canSkip:
			R.append((a,))

	ret = 1
	for v in simple.values():
		ret = ret * silnia[v] % MOD

	ret = ret * silnia[len(R)] % MOD
	

	print ret
	

codejam(problem)
        
