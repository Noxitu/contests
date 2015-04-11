#!/usr/bin/python

from utils import line, iline
from itertools import repeat
from sys import stderr

Q2id = { k:v for v, k in enumerate('1ijk') }
M = """1 i j k
i -1 k -j
j -k -1 i
k j -i -1"""
M = [ L.split() for L in M.split('\n') ]

def mul(a, b):
    s = ''
    if a[0] == '-':
        s += '-'
        a = a[1:]
    if b[0] == '-':
        s += '-'
        b = b[1:]
    a = Q2id[a]
    b = Q2id[b]
    s += M[a][b]
    if s[:2] == '--':
        s = s[2:]
    return s

def test():
    L, X = iline()
    S, = line()
    
    def solve():
        Y = 12 + X%4 if X > 12 else X
              
        now = '1'
        need = 'i', 'j', 'k'
        for j in xrange(Y):
            for c in S:
                now = mul(now, c)
                if need and now == need[0]:
                    need = need[1:]
                    now = '1'
                
        if not need and now == '1':
            print 'YES'
        else:
            print 'NO'
        
    return solve
    
if __name__ == '__main__':
    T, = iline()
    for i in xrange(1, T+1):
        print 'Case #%d:' % i,
        test()
