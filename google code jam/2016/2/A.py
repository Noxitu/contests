#!/usr/bin/python

from utils import iline
from itertools import permutations
from sys import stderr

translate = {'P' : 'RP', 'R' : 'SR', 'S' : 'SP'} 

def test():
    N, r, p, s = iline()
    
    yield
    
    for win in 'PRS':
        x = win
        for i in xrange(N):
            x = ''.join(map(translate.get, x))
            
        if x.count('R') != r or x.count('S') != s or x.count('P') != p:
            continue
            
        answer = list(x)
        for i in range(1,N+1):
            length = 2**i
            for a in range(0, 2**N, length):
                b = a + length/2
                c = b + length/2
                if answer[a:b] > answer[b:c]:
                    tmp = answer[a:b]
                    answer[a:b] = answer[b:c]
                    answer[b:c] = tmp
        print ''.join(answer)
        return
            
    print 'IMPOSSIBLE'
