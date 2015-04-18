#!/usr/bin/python

from utils import iline
from itertools import izip

def test():
    iline()
    M = iline()
    
    def solve():
        diffs = [ max( 0, a-b ) for a, b in izip( M[:-1], M[1:] ) ]
        A = sum( diffs )
        rate = max( diffs )
        B = sum( min( a, rate ) for a in M[:-1] )
        print A, B
        
    return solve
