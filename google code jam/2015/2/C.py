#!/usr/bin/python

# Small test cases only

from utils import iline, line

def test():
    N, = iline()
    sentences = [ set(line()) for i in xrange(N) ]
    
    yield
    
    source, dist = '^$'
    edges = {}
    connections = {}
    
    sentences[0] += source
    sentences[1] += dist
    
    for sent in sentences:
        for w1 in sent:
            for w2 in sent:
                if w1 != w2:
                    if w1 not in connections:
                        connections[w1] = set()
                    connections[w1].add(w2)
    
    Q = ['^']
    visited = {'^'}
    for q in Q:
        
        
   
    
    
        
    print answer
        
