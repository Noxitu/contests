# -*- coding: utf-8

import re

config = {
    'data-path': './data/:id/',
    'loop': False,
    'mode': 'd24'
}

def _try_bool(value):
    if value in { True, False }: return value
    try:
        return { 'true' : True, 'false' : False }[value.lower()]
    except:
        raise ValueError
    
def apply(key, value):
    for type in [_try_bool, int, str]:
        try:
            config[key] = type(value)
            break
        except ValueError:
            pass

_regex = re.compile( '\s*(\[([^\]]+)\])?([^:]+):(.+)' )
def load(path, id):
    tags = set(id.split('-'))
    with open(path) as f:
        for line in f.readlines():
            M = _regex.match(line)
            config_tag = set(M.group(2).split('-')) if M.group(2) else set()
            if config_tag <= tags:
                key, value = M.group(3).strip(), M.group(4).strip()
                apply( key, value )
    config['id'] = id
                

                
