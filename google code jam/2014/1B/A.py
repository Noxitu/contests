
from codejam import *

def simplify(X):
    Y = []
    Z = ''
    for x in X:
        if x == Z[-1:]:
            Y[-1] += 1
        else:
            Z += x
            Y.append(1)
            
    return Z, Y
    
def problemA():
    N, = readiline()
    X = [ readline()[0] for x in xrange(N) ]
    X = map( simplify, X )
    X, Y = zip(*X)
    
    if len(set(X)) != 1:
        print "Fegla Won"
    else:
        sol = 0
        Y = zip(*Y)
        for Z in Y:
            Z = list(Z)
            Z.sort()
            mid = Z[N/2]
            for y in Z:
                sol += abs(y-mid)
                
        print "%d" % sol

codejam(problemA)
        
        
        

