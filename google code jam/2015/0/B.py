#!/usr/bin/python

from utils import iline

def test():
    iline()
    P = iline()
    
    def solve():
        print min( i + sum( (p-1)/i for p in P ) for i in xrange(1, max(P)+1) )
        
    return solve
    
if __name__ == '__main__':
    T, = iline()
    for i in xrange(1, T+1):
        print 'Case #%d:' % i,
        test()()
