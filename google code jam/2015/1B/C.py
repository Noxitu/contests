#!/usr/bin/python

from utils import iline
from heap import heap

def test():
    n, = iline()
    data = [ iline() for i in xrange(n) ]
    yield
    
    data = [ (d,m+x) for d,h,m in data for x in xrange(h) ]
    h = heap( ((360-d)*m, 0, d, m) for d,m in data )
    n = len(data)
    
    def calc():
        result = n
        yield result
        
        for i in xrange(2*n):
            when, count, d, m = h.pop()
            if count == 0:
                result -= 1
            else:
                result += 1
            h.push(( when+m*360, count-1, d, m ))
            yield result
    
    print min(calc())
        
