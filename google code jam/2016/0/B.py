#!/usr/bin/python

from utils import iline, line

def test():
    s, = line()
    
    yield
    
    n = 0
    for a, b in zip(s, s[1:]+'+'):
      if a != b:
        n += 1
    print n
        
