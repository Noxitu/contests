
from random import randint

N = 1000

def good():
	A = range(N)
	for k in xrange(N):
		p = randint(k,N-1)
		A[k], A[p] = A[p], A[k]
	return A

def bad():
	A = range(N)
	for k in xrange(N):
		p = randint(0,N-1)
		A[k], A[p] = A[p], A[k]
	return A

X = {}
for i in xrange(120):
	z = randint(0,1)
	A = [good,bad][z]()
	rate = 0
	for i, a in enumerate(A[:-1]):
		if a <= i:
			a += i
		rate += (a-i) ** 0.25

	rate /= 1e4
	y = rate > 0.396
	rate = (z==0) == y
		

	X[rate] = X.get(rate,0)+1

X = X.items()
X.sort()

print min(X), max(X)

#{0: 375, 1: 343, 2: 196, 3: 68, 4: 16, 5: 2}
#{0: 427, 1: 332, 2: 169, 3: 51, 4: 17, 5: 3, 6: 1}

#{0: 3688, 1: 3704, 2: 1812, 3: 607, 4: 157, 5: 28, 6: 2, 7: 2}
#{0: 4308, 1: 3363, 2: 1536, 3: 570, 4: 160, 5: 53, 6: 7, 7: 2, 8: 1}

