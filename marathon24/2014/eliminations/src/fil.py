#!/usr/bin/python

import sys
from utils import *
	
def load_dataset():	
	T, = iline()
	
	for i in xrange(T):
		k, S = line()
		yield int(k), S
		
		
def solve((k,S)):
	arr = []
	n = 0
	for c in S+'$':
		if c == 'D':
			n += 1
		else:
			if n > 0:
				arr.append(n)
				n = 0
				
	if len(arr) == 0:
		return 0
		
	if k == 0 or len(arr) == 1:
		return max(arr)
	
	begin = end = 0
	if S[0] == 'D':
		begin = arr.pop(0)
	if S[-1] == 'D':
		end = arr.pop()
		
	if k == 1:
		return max(begin+end, max(arr))
	
	answer = 0
	arr.sort()
	while k > 3 and len(arr) > 0:
		answer += arr.pop()
		k -= 2
		
	#print >>sys.stderr, begin, arr, end, answer	
		
	ending = begin + end
	if k >= 3 and len(arr) > 0:
		ending = max(ending, max(begin, end) + sum(arr[-2:]))
		
	if k >= 2 and len(arr) > 0:
		ending = max(ending, begin + end + arr[-1], sum(arr[-2:]) )
		
	if k >= 1 and len(arr) > 0:
		ending = max(ending, max(begin, end) + arr[-1])
		
	answer += ending
	return answer
	
				
	
def save_answer(answer):
	print answer
	
if __name__ == '__main__':
	datasets = load_dataset()
	
	answers = map( solve, datasets )
	
	map( save_answer, answers )
	