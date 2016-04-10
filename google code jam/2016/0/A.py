#!/usr/bin/python

from utils import iline
from storage import *

N = 1000000

@store('A', version=3)
def data():
    def calculate(x):
        if x == 0:
            return 'INSOMNIA'
            
        nums = set()
        for y in xrange(x, 1000000000, x):
            nums |= set(str(y))
            if len(nums) == 10:
                return y
                
        assert False
            
    return map(calculate, xrange(0, N+1))
            
def test():
    n, = iline()
    
    yield
    
    print data[n]
        
