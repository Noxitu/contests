#!/usr/bin/python

from utils import iline

def test():
    n, = iline()
    high = raw_input()
    low = raw_input()
    
    yield
    
    high = list('X' + high + 'X')
    low = list('X' + low + 'X')
    n += 2
    
    def put(a,b,i):
        a[i] = 'G'
        if b[i] == '.':
            b[i] = '*'
        j = i-1
        while a[j] in '.*':
            a[j] = '*'
            j -= 1
        j = i+1
        while a[j] in '.*':
            a[j] = '*'
            j += 1
    
    def fill_one(a, b):
        for i in xrange(1, n-1):
            if a[i-1:i+2] == ['X', '.', 'X']:
                if b[i] == 'X':
                    put(a,b,i)
                else:
                    put(b,a,i)
                    
    def fill_rest(a, b):
        for i in xrange(1, n-1):
            if a[i] == '.':
                put(a,b,i)
    
    fill_one(high,low)
    fill_one(low,high)
    fill_rest(high,low)
    fill_rest(low,high)
    
    print high.count('G')+low.count('G')
        
