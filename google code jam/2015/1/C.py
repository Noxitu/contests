#!/usr/bin/python

from utils import iline
from math import atan2

def test():
    N, = iline()
    P = [ iline() for i in xrange(N) ]
    
    def solve():
        print
        for px, py in P:
            Q = [ (qx-px, qy-py) for qx, qy in P if px != qx or py != qy ]
            N = len(Q)
            Q.sort( key = lambda (x,y) : atan2(y,x) )
            Q.extend([ q for q in Q ])
            a = 0    
            ans = N
            for b in xrange(N, len(Q)):
                a = max( a, b-N )
                while a < b and Q[b][0]*Q[a][1] - Q[b][1]*Q[a][0] >= 0:
                    a += 1
                ans = min( ans, b-a )
            
            print ans
        
    return solve
