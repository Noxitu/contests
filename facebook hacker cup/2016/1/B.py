#!/usr/bin/python

from utils import iline
from heap import heap

def test():
    L, N, M, D = iline()
    W = iline()

    yield
    
    washing = heap((w, w) for w in W)
    dryiers = heap()
    
    t = 0
    for i in xrange(L):
        t, w = washing.pop()
        washing.push((t+w, w))
        
        if M > 0:
            t += D
            M -= 1
        else:
            t = max(dryiers.pop(), t) + D
        dryiers.push(t)
    
    print t
        
