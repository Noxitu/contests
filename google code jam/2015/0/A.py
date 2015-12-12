#!/usr/bin/python

from utils import line
from itertools import *

def test():
    _, row = line()
    
    def solve():
        print max( v-i for i, v in enumerate( chain( *( repeat(i, n) for i, n in enumerate( imap( int, row ) ) ) ) ) )
    
    return solve

