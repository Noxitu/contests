#!/usr/bin/python

from utils import iline

def test():
    n, = iline()
    data = [ iline() for i in xrange(n) ]
    
    yield
    
    answer = 0
    for x1, y1 in data:
        stars = {}
        for x2, y2 in data:
            dist2 = (x1-x2)**2 + (y1-y2)**2
            stars[dist2] = stars.get(dist2, 0)+1
            
        assert stars[0] == 1
        for v in stars.values():
            answer += v*(v-1)/2
    
    print answer
        
