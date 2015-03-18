# -*- coding: utf-8

import os, sys, importlib
import py24.log, py24.config, py24.utils

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
    """Initizes data path and prepares logging module"""
    data_path = config.config['data-path'].replace(':id', config.config['id'])
        
    try: 
        os.makedirs(data_path)
    except OSError:
        if not os.path.isdir(data_path):
            raise
            
    log.init(data_path)

def run():
    """Loads module and runs it. Runs it in loop if needed. Runs it in thread if needed."""
    sys.path.append(os.getcwd()+'/src')
    name = config.config['run']
    log.log('Running module: %s' % name )
    module = importlib.import_module( name )
    
    call = _run_loop if config.config['loop'] else _run_once
    if sys.flags.interactive:
        call = utils.Thread(call, daemon=True)
        
    call( module )