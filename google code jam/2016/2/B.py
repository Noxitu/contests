#!/usr/bin/python

from utils import iline, fline
from sys import stderr

def test():
    N, K = iline()
    P_ = fline()
    
    yield
    
    P_.sort()
    answer = 0.0
    for n in xrange(K+1):
        P = P_[:n] + (P_[-K+n:] if n != K else [])
        #print >>stderr, P
        F = { 0: 1.0 }
        for p in P:
            score = lambda p, x : p*F.get(x-1, 0.0) + (1-p)*F.get(x, 0.0)
            F = { x: score(p, x) for x in xrange(N) }
            
        answer = max(answer, F[K/2])
        
    print '{:.12f}'.format(answer)