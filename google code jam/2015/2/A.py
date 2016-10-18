#!/usr/bin/python

from utils import iline

DIRS = {'<' : (-1,0), '>':(1,0), 'v':(0,1), '^':(0,-1)}

def test():
    R, C = iline()
    world = [ raw_input() for y in xrange(R) ]
    
    yield
    
    n = 0
    for y in xrange(R):
        for x in xrange(C):
            if world[y][x] != '.':
                
                dx, dy = DIRS[world[y][x]]
                def path_ok(dx,dy):
                    _x = x+dx
                    _y = y+dy
                    if _y < 0 or _x < 0 or _y >= R or _x >= C: return False
                    while world[_y][_x] == '.':
                        _x += dx
                        _y += dy
                        if _y < 0 or _x < 0 or _y >= R or _x >= C: return False
                        
                    return True
                        
                if not path_ok(dx,dy):
                    n += 1
                    if not any( path_ok(dx, dy) for dx, dy in DIRS.values() ):
                        print 'IMPOSSIBLE'
                        return
                        
                
    print n
        
