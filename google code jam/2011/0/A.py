#!/usr/bin/python

from utils import line, iline


def test():
    data = line()
    positions = [1, 1]
    time = [0,0]
    
    for c, p in zip( data[1::2], data[2::2] ):
        p = int(p)
        r = 1 if c == 'O' else 0
        
        time[r] = max( time[r]+abs(positions[r]-p), max(time) ) + 1
        positions[r] = p
    
    print max(time)
    
if __name__ == '__main__':
    T, = iline()
    for i in xrange(1, T+1):
        print 'Case #%d:' % i,
        test()
