#!/usr/bin/python

from utils import iline
import sys

def test():
    N, J = iline()
    
    yield
    
    print
    i = 0
    while J:
      seq = '1' + (('0'*N)+bin(i)[2:])[-(N-2):] + '1'
      divisors = []
      for base in xrange(2,11):
        val = int(seq, base)
        p = 2
        while p*p <= 10000000:
          if val % p == 0:
            divisors.append(p)
            val = None
            break
          p += 1
            
        if val is not None:
           seq = None
           break
      if seq is not None:
        print >>sys.stderr, 'Found one,', J, 'missing'
        print seq, ' '.join(map(str, divisors))
        J -= 1
        
      i += 1
        
