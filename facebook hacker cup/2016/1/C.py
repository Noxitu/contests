#!/usr/bin/python

from utils import iline

def test():
    N, A, B = iline()
    C = iline()
    
    yield
    
    Q = 2.0*(B-A)
    yacht = sum(C)
    
    def solve(A, B):
        total = 0
        for c in C:
            if B <= c:
                break
                
            if A <= c:
                total += (c+A)*(c-A)
                A = c
            
            A -= c
            B -= c
                
        total += (B+A)*(B-A)
        return total
        
    A_ok = A/yacht
    B_ok = B/yacht
    total = 0
    
    if A_ok == B_ok:
        total += solve(A%yacht, B%yacht)
    else:
        total += solve(A%yacht, yacht)
        total += solve(0, yacht) * (B_ok-A_ok-1)
        total += solve(0, B%yacht)
    
    print '{:.12f}'.format(total/Q)
        
