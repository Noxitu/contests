#!/usr/bin/python

from utils import iline

def test():
    iline()
    data = iline()
    N = len(data)
    
    yield
    
    ans = 0
    
    p = -1
    i = 0
    q = 0
    for a in data:
      if q == 0:
        p = a
        q = (q + 1) % 4
      else:
        d = a - p
        if d <= 0 or d > (4 - q) * 10:
          ans += 4 - q
          p = a
          q = 1
        elif d > 10:
          c = (d - 1) // 10
          ans += c
          p = a
          q = (q + c + 1) % 4
        else:
          p = a
          q = (q + 1) % 4
    
    ans += (4 - (ans + N) % 4) % 4
    print ans
        
