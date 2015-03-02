#!/usr/bin/python

import sys
from utils import *
	
def matrix_mul(A, B, M):
	m1 = len(A)
	n1 = len(A[0])
	m2 = len(B)
	n2 = len(B[0])
	assert n1 == m2
	
	C = [ [ sum( A[i][k] * B[k][j] for k in xrange(m2) )%M for j in xrange(n2) ] for i in xrange(m1) ]
	
	return C
	
def matrix_pow(A, N, M):
	n = len(A)
	C = [ [ 1 if i == j else 0 for j in xrange(n) ] for i in xrange(n) ]
	while N:
		if N & 1:
			C = matrix_mul(C, A, M)
		A = matrix_mul(A, A, M)
		N >>= 1
	
	return C
	
def load_dataset():	
	C, M = iline()
	T, = iline()
	
	for i in xrange(T):
		X, = iline()
		yield C, M, X
		
def solve((C,M,X)):
	F = [ [ 0, 1, 0 ], [ 0, 0, 1 ], [ -1, 1-C, 1+C ] ]
	G = matrix_pow( F, X, M )
	Y = matrix_mul( G, [ [0], [1], [2] ], M )
	
	return Y[0][0]
	
def save_answer(y):
	print y
	
	
if __name__ == '__main__':
	datasets = load_dataset()
	
	answers = map( solve, datasets )
	
	map( save_answer, answers )