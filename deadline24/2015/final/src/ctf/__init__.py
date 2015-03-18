# -*- coding: utf-8

from py24.config import config
from py24.client import Client, Action
from py24.failed import Failed
import py24.log as log

from time import sleep

@log.calls
class CTF(Client):
    @Action
    def GET_TIME(self):
        return self.getiline()
        
    @Action
    def GET_MY_SCORE(self):
        return self.getiline()
        
    @Action
    def GET_ALL_SCORES(self):
        self.getiline()
        return self.getiline()

def run():
    #game = CTF()
    
    while True:
        try:
            log.status_format('{game}\n{config}', locals(), globals())
            raise Failed(5)('No way!')
            game.WAIT()
        except Failed(9):
            log.status('No current round')
            sleep(10)

    