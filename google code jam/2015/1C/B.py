#!/usr/bin/python

from utils import iline
from operator import mul

def test():
    K, L, S = iline()
    keyboard = raw_input()
    word = raw_input()
    
    yield
    
    if any( c not in keyboard for c in word ):
        print 0.0
        return
        
    chances = { c : 1.0*keyboard.count(c)/K for c in keyboard }
    
    max_sufix = max( n for n in xrange(L) if word.startswith(word[-n:]) )
    max_bananas = 1 + (S - L) / (L-max_sufix)
    
    chance_for_word = reduce( mul, (chances[c] for c in word), 1.0 )
       
    print '%.7f' % (max_bananas - chance_for_word * (S-L+1))
        
