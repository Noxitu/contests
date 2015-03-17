# -*- coding: utf-8

_failedexceptions = {}

def Failed(c):
	try:
		return _failedexceptions[c]
	except KeyError:
		class FailedException(Exception):
			pass
		f = _failedexceptions[c] = FailedException
		return f
		
