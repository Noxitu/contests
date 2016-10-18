#!/usr/bin/python

from utils import iline

def test():
    iline()
    nums = iline()
    
    yield
    
    total = sum(nums)
    nums = [ [count, chr(ord('A')+i), i] for i, count in enumerate(nums) ]
    
    answer = ''
    while total > 0:
        count, c, i = max(nums)
        answer += c
        nums[i][0] -= 1
        total -= 1
        
        if total%2 == 0:
            answer += ' '
    print answer
        
        
