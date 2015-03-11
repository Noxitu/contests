# -*- coding: utf-8

import log

_failedexceptions = {}

def Failed(c):
	#log.log('Exception: %d' % c)
	try:
		return _failedexceptions[c]
	except KeyError:
		class FailedException(Exception):
			pass
		f = _failedexceptions[c] = FailedException
		return f
		
