# -*- coding: utf-8
"""Framework for Deadline24 and Marathon24 finals.

Usage:
  py24 [options] ID

Arguments:
  ID                     It should contain both task and server id. [Examples: ctf-2, ice-1-testing]

Options:
  -h --help              Show this screen.
  --config-path=<path>   Set config file path. [Default: ./config.txt]
  --data-path=<path>     Set data directory path. [Default: ./data/:id/]
  --loop                 Reconnect after errors. [Default: False]
  --hostname=<hostname>  Hostname of the server.
  --port=<port>          Hostname of the server.
  --login=<login>        Login used after connecting.   
  --password=<password>  Login used after connection.
  --run=<module>         Module that will be run. Must contain run() function.
  --mode=(d24|m24)       Server compability mode. [Default: d24]
  
All options may be set in config file. Options passed by command line take precedence.
You MAY pass custom options.
Option arguments MUST be passed using = sign.
Short options CAN NOT have arguments.
"""

# Nie umiem w docopt-cie zrobić custom parametrów

import sys
import py24
import py24.config

command_line = {}
for arg in sys.argv[1:]:
    key, value = 'id', arg
        
    if arg.startswith('--'):
        if '=' in arg:
            key, value = arg[2:].split('=', maxsplits=1)
        else:
            key, value = arg[2:], True
            
    elif arg.startswith('-'):
        key = arg[1:]
        value = True
    
    if key in { 'h', 'help' } or key in command_line:
        print(__doc__)
        sys.exit(0)
        
    command_line[key] = value
    
if 'id' not in command_line:
    print(__doc__)
    sys.exit(0)
    
py24.config.load( command_line.get( 'config-path', './config.txt'), command_line['id'] )
for key, value in command_line.items():
    py24.config.apply( key, value )
    
py24.initize()
py24.run()