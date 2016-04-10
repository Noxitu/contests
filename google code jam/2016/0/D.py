#!/usr/bin/python

from utils import iline

def test():
    K, C, S = iline()
    
    yield
    
    if C*S < K:
        print 'IMPOSSIBLE'
    else:
        for i in xrange(0, K, C):
            n = 0
            for j in xrange(C):
                n *= K
                if i+j < K:
                    n += i+j
            print n+1,
        print
        
