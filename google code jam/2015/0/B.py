#!/usr/bin/python

from utils import iline, heap

def test():
    iline()
    P = iline()
    
    def solve():
        P.sort()
        def solve():
            for i in xrange(1, P[-1]+1):
                yield i + sum( (p-1)/i for p in P )
                
        print min(solve())
        
    return solve
    
if __name__ == '__main__':
    T, = iline()
    for i in xrange(1, T+1):
        print 'Case #%d:' % i,
        test()
