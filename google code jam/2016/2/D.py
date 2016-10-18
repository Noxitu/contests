#!/usr/bin/python

from utils import iline, line
from itertools import *
from sys import stderr

def test():
    N, = iline()
    X = [ line()[0] for i in xrange(N) ]
    XX = [ int(val) for row in X for val in row ]
    
    yield
    
   
    best = N*N
    for group in product(range(N), repeat=N):
        Y = [ [0]*N for i in xrange(N) ]
        for ai, aj in enumerate(group):
            for bi, bj in enumerate(group):
                if aj == bj:
                    Y[ai][bi] = 1
                    
        YY = [ int(val) for row in Y for val in row ]
        if any(x and not y for x, y in zip(XX, YY)):
            continue
            
        best = min(best, sum(YY) - sum(XX))
    print best
    