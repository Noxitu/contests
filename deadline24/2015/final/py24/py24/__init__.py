# -*- coding: utf-8

import os, sys, importlib
import log, config, utils

def _run_once( module ):
    try:
        log.log( 'Connecting...' )
        module.run()
    except:
        log.log_exc()
        
def _run_loop( module ):
    while True:
        run_once( module )
        
def initize():
    data_path = config.config['data-path'].replace(':id', config.config['id'])
        
    try: 
        os.makedirs(data_path)
    except OSError:
        if not os.path.isdir(data_path):
            raise
            
    log.init(data_path)

def run():
    sys.path.append(os.getcwd()+'/src')
    name = config.config['run']
    log.log('Running module: %s' % name )
    module = importlib.import_module( name )
    
    call = _run_loop if config.config['loop'] else _run_once
    if sys.flags.interactive:
        call = utils.Thread(daemon=True)(call)
        
    call( module )