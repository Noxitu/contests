#!/usr/bin/python

from utils import iline

def test():
    B, M = iline()
    
    yield
    
    S = 2**(B-2)
    
    if S < M:
        print 'IMPOSSIBLE'
        return
        
    bonus = (M == S)
    if bonus:
        M = S-1
        
    print 'POSSIBLE'
    print ('0'*B + bin(M)[2:] + ('1' if bonus else '0'))[-B:]
    for i in xrange(2, B+1):
        print '0'*i + '1'*(B-i)
    
        
