# -*- coding: utf-8

from py24.config import config
from py24.client import Client, Action
from py24.failed import Failed
from py24.utils import Fields
import py24.log as log

from time import sleep, time
import random
from collections import deque

STATUS = """
Punktow: {game.points}
Turns to end: {game.turns_to_end}
Gamma: {gamma}  ({spent_points} -> {free_points})
Scoring:  O={game.O}   S={game.S}
Units: {game.units}
Enemies: {game.enemies}
POI: {game.poi}
"""

YELLOW = lambda x : '\033[33m'+x+'\033[0m'
GREEN = lambda x : '\033[32m'+x+'\033[0m'
RED = lambda x : '\033[31m'+x+'\033[0m'

def starmina(s):
    if s == 'INF': return None
    return int(s)
        
def owner(s):
    if s == 'NONE': return None
    if s == 'YOURS': return -1
    return int(s)
    
def distance(a,b):
    if (a.x < b.x) == (a.y < b.y):
        return max(abs(a.x-b.x), abs(a.y-b.y))
    else:
        return abs(a.x-b.x) + abs(a.y-b.y)
        
def distancexy(a,b,x,y):
    if (a < x) == (b < y):
        return max(abs(a-x), abs(b-y))
    else:
        return abs(a-x) + abs(b-y)
        
map_file = None
def store_map(game, dist):
    global map_file
    if map_file is None:
        data_path = config['data-path'].replace(':id', config['id'])
        map_file = open('%s/map' % data_path, 'w')
    
    def visual(x,y,c):
        for u in game.units:
            if (u.x, u.y) == (x,y):
                return GREEN( u.type[0] )
                
        for u in game.enemies:
            if (u.x, u.y) == (x,y):
                return RED( u.type[0] )
                
        for u in game.poi:
            if (u.x, u.y) == (x,y):
                if u.owner == -1:
                    return GREEN( ',' ) 
                else:
                    return YELLOW( ',' ) 
                
        return c
    
    map_with_units = [ ''.join( visual(x+1,y+1,c) for x,c in enumerate(L) ) for y,L in enumerate(game.map or [ ' '*game.N ] * game.N) ]
    mapstr = '\n'.join(map_with_units)
    
    map_file.truncate(0)
    map_file.write(mapstr)
    map_file.write('\n')
    map_file.flush()
    
@Fields('id', 'x', 'y', 'type', 'w', 'c')
class MyUnit:
    def __repr__(self):
        return '%s@%d(%d, %d)[%s]' % (self.type, self.id, self.x, self.y, str(self.w))
        
@Fields('x','y','type','w','team')
class Enemy:
     def __repr__(self):
        return '%s@(%d, %d)[%s]' % (self.type, self.x, self.y, str(self.w))
        
@Fields('x','y','owner')
class Point:
    def __repr__(self):
        return '(%d, %d)[%s]' % (self.x, self.y, str(self.owner))

class Battle(Client):
    def __init__(self):
        Client.__init__(self)
    
    @Action
    def DESCRIBE_WORLD(self):
        self.world = self.line()[:-1]
        self.N = int(self.world[0])
        self.Es = int(self.world[4])
        self.O = int(self.world[2])
        self.S = int(self.world[3])
        
    @Action
    def LOOK_AROUND(self):
        self.map = [ self.getline() for i in xrange(self.N) ]
        self.map = [ ''.join( '~' if c == 'L' else ' ' for c in L ) for L in self.map ]
     
    @Action
    def CREATE_UNIT(self, type):
        uid, ux, uy = self.iline()

    @Action
    def SHOOT(self,id,x,y):
        hx, hy = self.iline()
        enemy = [ e for e in self.enemies if (e.x, e.y) == (hx, hy) ]
        
    @Action
    def TIME_TO_END(self):
        self.turns_to_end, = self.iline()
        
    @Action
    def LIST_UNITS(self):
        count, = self.iline()
        self.units = [ MyUnit(*self.line(int,int,int,str,starmina,int)) for i in xrange(count) ]
        
    @Action
    def LIST_ENEMIES(self):
        count, = self.iline()
        self.enemies = [ Enemy(*self.line(int,int,str,starmina,int)) for i in xrange(count) ]

        
    @Action
    def LIST_STRATEGIC_POINTS(self):
        count, = self.iline()
        self.poi = [ Point(*self.line(int,int,owner)) for i in xrange(count) ]

    @Action
    def MY_STAT(self):
        self.points, = self.iline()
        return self.points
        
def generate_map():
    map = {}
    for u in game.units:
        map[(u.x, u.y)] = u.type[0]
    for e in game.enemies:
        map[(e.x, e.y)] = '*'
       
    minX = min( [ x for x,y in map ] )
    minY = min( [ y for x,y in map ] )
    maxX = max( [ x for x,y in map ] )
    maxY = max( [ y for x,y in map ] )
    
    mapstr = [ [ ' ' for y in xrange(maxX-minX+1) ] for x in xrange(maxY-minY+1) ]
    
    for (x,y),c in map.items():
        mapstr[y-minY][x-minX] = c
        
    return '\n'.join(''.join(L) for L in mapstr)
    
def bfs(game):
    Q = []
    dist = {}
    for p in game.poi:
        if p.owner != -1 or game.S >= 3:
            Q.append((p.x, p.y))
            dist[(p.x, p.y)] = 0

    for e in game.enemies:
        Q.append((e.x, e.y))
        dist[(e.x, e.y)] = 0
        
    for x, y in Q:
        d = dist[(x,y)]
        for dx, dy in [ (1,1), (1,0), (-1,0), (0,1), (0,-1), (-1,-1) ]:
            p = px, py = x+dx, y+dy
            if p not in dist and px > 0 and py > 0 and px <= game.N and py <= game.N and (not game.map or game.map[y+dy-1][x+dx-1] != '~'):
                dist[p] = d + 1
                Q.append(p)
    
    return dist
    
def run():
    global game
    game = Battle()
    
    prev_turn = 0
    prev_world = None
    prev_score = None
    spent_points = 0
    turn_no = 99999999999
    
    while True:
        try:
            turn_start = time()
            
            game.DESCRIBE_WORLD()
            game.TIME_TO_END()
            
            if prev_turn < game.turns_to_end:
                prev_world = game.world
                game.map = None
                log.log('New round')
                if prev_score is not None:
                    log.log('Previous score: %d' % prev_score)
                spent_points = 0
                turn_no = 0
            else:
                turn_no += 1
                
            if not game.map:
                try:
                    game.LOOK_AROUND()
                    log.log('Loaded map')
                except Failed(106):
                    pass
                
            game.LIST_UNITS()
            game.LIST_ENEMIES()
            game.LIST_STRATEGIC_POINTS()
            game.MY_STAT()
            prev_score = game.points
            
            #if prev_turn not in (0, game.turns_to_end+1):
            #    log.log('Missed %d turns' % (prev_turn-game.turns_to_end+1) )
                
            
            prev_turn = game.turns_to_end
            
            for u in game.units:
                enemies = [ e for e in game.enemies ]
                enemies.sort( key = lambda e : distance(e,u) )
                for e in enemies:
                    if distance(u, e) <= {'CANNON':8, 'TANK':4, 'ARMORED':5}.get(u.type,0) and u.type != 'HQ' and e.type != 'HQ':
                        try:
                            game.SHOOT(u.id, e.x-u.x, e.y-u.y)
                            break
                        except Failed(111):
                            pass # za daleko
                        except Failed(110):
                            pass # nie widac
                        except Failed(109):
                            pass # nie widac
                        except Failed(108):
                            break # nie ma amunicji
            
            dist = bfs(game)
            gamma = max( 0.1, 1.0 - 1.0 * turn_no / game.Es )
            if len(game.units) < 7:
                gamma = 1.0
                
            for u in game.units:
                moves = [ (1,1), (1,0), (-1,0), (0,1), (0,-1), (-1,-1) ]
                random.shuffle( moves )
                if u.c == 0:
                    if random.randint(0,30) == 0:
                        moves = random.sample(moves, 1)
                    if u.type == 'HQ':
                        ex = sum( v.x for v in game.units ) / len(game.units)
                        ey = sum( v.y for v in game.units ) / len(game.units)
                        dx, dy = min( moves, key = lambda (dx, dy) : distancexy(u.x+dx, u.y+dy, ex, ey) )
                    else:
                        if any( True for p in game.poi if p.x == u.x and p.y == u.y ):
                            continue
                            
                        moves = [ ((dx, dy), dist.get((u.x+dx, u.y+dy), None)) for dx, dy in moves ]
                        moves = [ (p,d) for p,d in moves if d is not None ]
                        
                        if moves:
                            dx, dy = min( moves, key = lambda (p,d) : d )[0]
                        else:
                            dx, dy = random.choice( [ (1,1), (1,0), (-1,0), (0,1), (0,-1), (-1,-1) ] )
                    try:
                        if (dx, dy) != (0,0):
                            game.MOVE(u.id, dx, dy)
                    except Failed(101):
                        pass # zle pole docelowe
                    except Failed(102):
                        pass
                            
            free_points = gamma * (game.points + spent_points) - spent_points
            try:
                while free_points >= 10:
                    try:
                        game.CREATE_UNIT( 'CANNON' ) # 'TANK', 'ARMORED'
                    except Failed(103):
                        game.CREATE_UNIT( random.choice(['TANK', 'TANK', 'ARMORED']) )
                    free_points -= 10
                    spent_points += 10
            except Failed(105):
                pass # no empty field
            except Failed(107):
                pass # zniszczyli mi baze
                
            log.status_format( STATUS, locals(), globals() )
            #store_map(game, dist)
            
            if time() - turn_start > 1:
                log.log('Turn lasted for %f seconds' % (time() - turn_start))            
            game.WAIT()
        except Failed(100):
            pass # wrong id
        except Failed(6):
            #mapstr = generate_map()
            log.status_format( STATUS, locals(), globals() )
            #store_map(game, dist)
            pass #waiting
    