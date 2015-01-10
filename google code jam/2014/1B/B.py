from codejam import *

def solve(A, B, K):
    M = 1
    sol = 0
    
    while M <= K:
        M *= 2
    
    while M > 0:
        if M <= K:
            
            sol += min(A,M)*B + A*min(B,M) - min(A,M)*min(B,M)
            A -= M
            B -= M
            K -= M
        M /= 2
        
    return sol
    

def problem():
	A, B, K = readiline()
	print

codejam(problem)
        
