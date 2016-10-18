#!/usr/bin/python

from utils import iline
from fractions import gcd
from collections import defaultdict

def test():
    A, B, C, K = iline()
    
    yield
    
    K = min(C, K)
    print A*B*K
    for a in xrange(A):
        c = a
        for b in xrange(B):
            for i in xrange(K):
                print a+1, b+1, c+1
                c = (c+1)%C
    
