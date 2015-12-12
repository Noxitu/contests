#!/usr/bin/python

import sys, random
from utils import *

	
def load_dataset():	
	C, Q = iline()
	mx, my = iline()
	D = [ iline() for i in xrange(C) ]
	D = [ (id,x-mx,y-my,b,e,d,s) for id,x,y,b,e,d,s in D ]
	T0 = 2*sum( abs(x)+abs(y) for id,x,y,b,e,d,s in D )
	
	return [ (Q, D, C, T0) ]

def solve((Q, D, C, T0)):
	T = 0
	left = set( d for d in D )
	answer = []
	
	trucks = []
	
	while left:
		can_arrive = lambda (vx,vy,t,q,path),(id,x,y,b,e,d,s) : t+abs(x-vx)+abs(y-vy) <= e
		can_supply = lambda (vx,vy,t,q,path),(id,x,y,b,e,d,s) : q >= d
		arrive_at = lambda (vx,vy,t,q,path),(id,x,y,b,e,d,s) : max(t+abs(x-vx)+abs(y-vy), b)
		
		try:
			arrive, truck, client = min( (arrive_at(truck, client), truck, client) for truck in trucks for client in left if can_arrive(truck, client) and can_supply(truck, client) )
		except ValueError:
			need = (sum( d for id,x,y,b,e,d,s in left )+Q-1)//Q
			print 'adding', need, 'trucks'
			for i in xrange(need):
				path = []
				truck = [0,0,0,Q,path]
				trucks.append(truck)
				answer.append(path)
			continue
		
		vx,vy,t,q,path = truck
		id,x,y,b,e,d,s = client
		
		left.remove(client)
		T += abs(x-vx) + abs(y-vy)
		truck[3] -= d
		truck[2] = arrive+s
		path.append(id)
		
	for vx,vy,t,q,path in trucks[1:]:
		T += abs(vx) + abs(vy)
			
	yield T, answer, C, T0
	
def get_score((T, answer, C, T0)):
	return ( 1000*T0//T + 1000*C/len(answer) )
			
def save_answer((T, answer, C, T0)):
	print len(answer), T
	for L in answer:
		for id in L:
			print id,
		print
	

					
if __name__ == '__main__':
	datasets = load_dataset()
	
	answers = map( solve, datasets )
	
	map( save_answer, answers )