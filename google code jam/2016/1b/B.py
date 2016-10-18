#!/usr/bin/python

from utils import iline, line

def solve(A, B):
    X = ''
    for a, b in zip(A, B):
        if a != '?':
            X += a
        elif b != '?':
            X += b
        else:
            X += '0'
            
    return X
    
def fill(X, c):
    Y = ''
    for x in X:
        if x == '?':
            Y += c
        else:
            Y += x
    return Y
    
def brutforce(X_, Y_):
    def all_results():
        def write(A, n):
            return ('0'*n+str(A))[-n:]
        def check(A, A_):
            return all(a == a_ or a_ == '?' for a, a_ in zip(A, A_))

        V = '0'*len(X_)
        for x in xrange(int('1'+V)):
            x = write(x, len(V))
            if check(x, X_):
                for y in xrange(int('1'+V)):
                    y = write(y, len(V))
                    if check(y, Y_):
                        yield x, y
                        
    return min(all_results(), key=lambda (c, j) : (abs(int(c)-int(j)), c, j))
    
    
def fix(P, X_, ok, change):
    if ok:
        return P
        
    X_ = X_[:len(P)]
    X = list(P)
    Pmiss = ''.join( p for p, x in zip(P, X_) if x == '?' )
    if not Pmiss:
        return P
    Ppos = [ i for i, x in enumerate(X_) if x == '?' ]
    
    Pmiss = str(int(Pmiss)+change)
    
    if len(Pmiss) > len(Ppos) or Pmiss[0] == '-':
        return P
        
    Pmiss = ('0'*len(Ppos)+Pmiss)[-len(Ppos):]
        
    for fPos, tPos in enumerate(Ppos):
        X[tPos] = Pmiss[fPos]
        
    return ''.join(X)
    

def test():
    C_, J_ = line()
    
    yield
        
    C = ''
    J = ''
    X = ''
    
    pos = None
    for i, (c, j) in enumerate(zip(C_, J_)):
        if '?' not in (c,j) and c != j:
            pos = i
            break
            
    if pos is None:
        X = solve(C_, J_)
        print X, X
    else:
        answers = []
        
        prefix = solve(C_[:pos], J_[:pos])
        
        # C smaller
        answers += [(prefix+fill(C_[pos:], '9'), fix(prefix, J_, C_[pos] < J_[pos], 1)+fill(J_[pos:], '0'))]
        answers += [(fix(prefix, C_, C_[pos] < J_[pos], -1)+fill(C_[pos:], '9'), prefix+fill(J_[pos:], '0'))]
        
        # J smaller
        answers += [(fix(prefix, C_, C_[pos] > J_[pos], 1)+fill(C_[pos:], '0'), prefix+fill(J_[pos:], '9'))]
        answers += [(prefix+fill(C_[pos:], '0'), fix(prefix, J_, C_[pos] > J_[pos], -1)+fill(J_[pos:], '9'))]
        
        C, J = min(answers, key=lambda (c, j) : (abs(int(c)-int(j)), c, j))
        print C, J
        
        #print answers