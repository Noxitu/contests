
from codejam import *

from fractions import gcd
 
def problem():
	a,b = map( int, sys.stdin.readline().strip().split('/') )

	c = gcd(a,b)
	a /= c
	b /= c

	if int(bin(b)[3:]) != 0:
		print 'impossible'
	else:
		far = len(bin(b)[2:])
		near = far - len(bin(a)[2:])
		print near

codejam(problem)

        
        
        

