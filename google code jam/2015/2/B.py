#!/usr/bin/python

import sys
from utils import line, fline
from itertools import chain

def test():
    N, V, X = line(int, float, float)
    data = [ fline() for i in xrange(N) ]
    
    yield
    
    data.sort(key=lambda (r,c) : c)        
    
    def solutions(data, sgn):
        R = RC = 0
        for r,c in data:
            if sgn*(RC+r*c) > sgn*X*(R+r)+1e-10:
                if c != X:
                    k = (X*R-RC)/(r*c-X*r)
                    yield R+r*k
                    break
            R += r
            RC += r*c
            if abs(RC - X*R) < 1e-10:
                yield R
           
    try:
        solutions = chain(solutions(data[::1], 1), solutions(data[::-1], -1))
        R = max(filter(None, solutions))
        T = V / R
        print '%.12f' % T
        
    except ValueError:
        print 'IMPOSSIBLE'
    
        
        
