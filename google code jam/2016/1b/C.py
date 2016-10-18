#!/usr/bin/python

from utils import iline, line
from collections import defaultdict
import sys

def test():
    n, = iline()
    topics = { tuple(line()) for i in xrange(n) }
    
    yield
    
    edges = defaultdict(set)
    
    for a, b in topics:
        edges[1].add((1,a))
        edges[(1,a)].add((2,b))
        edges[(2,b)].add(2)
        
    
    
    def find():
        prev = {1: None}
        Q = [1]
        for q in Q:
            if q == 2:
                X = []
                while q != 1:
                    if q != 2:
                        X.append(q)
                    q = prev[q]
                return X[::-1]
                    
                
            for e in edges[q]:
                if e not in prev:
                    prev[e] = q
                    Q.append(e)
            
    path = find()
    print path
    for a, b in zip(path[:-1], path[1:]):
        edges[a].remove(b)
        edges[b].add(a)
        
        
    
    
        
        