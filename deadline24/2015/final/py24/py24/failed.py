# -*- coding: utf-8

_failedexceptions = {}

def Failed(id):
    """Returns exception class assigned to id."""
    try:
        return _failedexceptions[id]
    except KeyError:
        class FailedException(Exception):
            pass
        f = _failedexceptions[id] = FailedException
        return f
        
