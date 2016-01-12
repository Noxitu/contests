#!/usr/bin/python

from utils import iline

def test():
    n, p = iline()
    B = iline()
    
    yield
   
    a = 0
    sum = 0
    answer = 0
    for b in xrange(n):
        sum += B[b]
        while p < sum:
            sum -= B[a]
            a += 1
        answer += b-a+1
    print answer
        
