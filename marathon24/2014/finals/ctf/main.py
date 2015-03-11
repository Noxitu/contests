#!/usr/bin/python -i
# -*- coding: utf-8 -*-

import config, sys, os, math
import random
from server import Server, Action
from threading import Thread
import log
from time import time, sleep
from pprint import pprint
from collections import deque

from failed import Failed
from random import randint, choice

def gettimestr():
	t = int(time()) + 2*3600
	return '%.2d:%.2d:%.2d' % (t/3600%24, t/60%60, t%60)

if len(sys.argv) < 2:
	raise Exception("Nie podano numeru serwera")

config.game_id = int(sys.argv[1])
config.path = str(config.game_id)

infinity = 1000000000

STATUS_MSG = """Round: %d
Turn: %d / %d
Score: %d (%d)
Map: %d
My Id: %d
My pos: %d %d
Action: %s
Reason: %s
Shots left: %d
Dist to flag: %d
Carried flags: %d
Bonuses: %d
"""

GREEN = lambda x : '\033[32m'+x+'\033[0m'
RED = lambda x : '\033[31m'+x+'\033[0m'

def angle_diff( a, b ):
	return min( max(a,b)-min(a,b), min(a,b)-max(a,b)+360)
	
def get_angle( dx, dy ):
	return math.atan2(-dx, dy) % (2*math.pi) * 180 / math.pi

###############################################################

class Pos:
	def __init__(self, args):
		self.x, self.y = args
		
	def pos(self):
		return (self.x, self.y)

class Object(Pos):
	def __init__(self, args):
		Pos.__init__(self, args[:2])
		self.z = args[2]

class Game(Server):
	PORT = 4210 + 10*config.game_id
	LOOP = True

	def __init__(self):
		Server.__init__(self)
		self.loaded_map = 0

	def __repr__(self):
		return "Game object"

	@Action
	def DESCRIBE_WORLD(self):
		self.n, self.m, self.P, self.T, self.id = self.getiline()
		self.snipers = []
		self.flags = []
		self.bases = []
		
		for i in xrange(1,self.T+1):
			if i == self.id:
				self.sniper = Object(self.getiline())
				self.flag = Object(self.getiline())
				self.base = Pos(self.getiline())
				self.getiline()
			else:
				self.snipers.append( Object(self.getiline()) )
				self.flags.append( Object(self.getiline()) )
				self.bases.append( Pos(self.getiline()) )
				self.getiline()
		
		b, = self.getiline()
		self.bonuses = [ self.getline() for i in xrange(b) ]
		
		self.load_map()
		
	@Action 
	def DESCRIBE_MONSTERS(self):
		p, = self.getiline()
		self.monsters = []
		for i in xrange(p):
			m = Pos((0,0))
			m.id, m.x, m.y, tx, ty, l1, l2, l3 = self.getiline()
			self.monsters.append(m)
	
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
		
	@Action
	def LOOK(self):
		p, = self.getiline()
		self.enemies = [ Object(self.getiline()) for i in xrange(p) ]
		
	@Action
	def SHOOT(self):
		self.action = 'shoot'
		
	@Action
	def MOVE(self, dir):
		self.action = 'move %s' % dir
		
	@Action
	def ROTATE(self, r):
		self.action = 'rotate %d' % r
		
	@Action
	def PICK_UP(self):
		self.action = 'pick up'
		
	@Action
	def RESPAWN(self):
		self.action = 'respawn'
		
	@Action
	def BONUS_INFO(self,r):
		return self.getline()

###############################################3


	
	def load_map(self):
		if self.loaded_map == self.m:
			return
			
		self.loaded_map = self.m
		self.map = {}
		with open('%d/map/map_%.2d.txt' % (config.game_id, self.m)) as f:
			f.readline()
			for x in xrange(self.n):
				L = f.readline()
				for y in xrange(self.n):
					if L[y] == '.':
						self.map[(x,y)] = Pos((x,y))
						
		for p in self.map.values():
			edges = [ (p.x+1, p.y), (p.x-1, p.y), (p.x,p.y+1), (p.x,p.y-1) ]
			edges = filter( lambda (x,y) : (x,y) in self.map, edges )
			p.edges = [ self.map[s] for s in edges ]
			
	def save_map(self):
		T = [ ['#' for j in xrange(self.n) ] for i in xrange(self.n) ]
		for (x,y) in self.map:
			T[x][y] = ' '
			
		for (x,y) in self.shotline:
			if (x,y) in self.map:
				T[x][y] = '.'
			
		for f in self.flags:
			if f.z == 0:
				T[f.x][f.y] = 'f'
			
		if self.sniper.pos() != (-1,-1):
			T[self.sniper.x][self.sniper.y] = GREEN('*')
			
			
		for s in self.snipers:
			if s.pos() != (-1,-1):
				T[s.x][s.y] = RED('*')

				
		T[self.base.x][self.base.y] = GREEN('^')
		
		for m in self.enemies:
			T[m.x][m.y] = RED('@')
			
		for m in self.monsters:
			T[m.x][m.y] = GREEN('@')
			
		T = [ ''.join(t) for t in T ]
		with open('%d/current_map.txt' % config.game_id, 'w') as f:
			f.write('\r\n'.join(T))
			
	def bfs(self, isSource, attr):
		queue = deque()
		for zone in self.map.values():
			if isSource(zone):
				setattr(zone, attr, 0)
				queue.append( (zone, 0) )
			else:
				setattr(zone, attr, infinity)
		
		while queue:
			target, dist = queue.popleft()
			for zone in target.edges:
				if getattr(zone, attr) == infinity:
					setattr(zone, attr,  dist+1)
					queue.append( (zone, dist+1) )
		
	
	def loop(self):
		previous_round = 0
		previous_turn = 0
		self.max_carry = config.game_id*2
		self.shots_left = 3
		self.reload_at = 0
		self.shot_at = 0
		self.look_history = deque()
		self.the_r = 0
		while self.LOOP:
			try:
				self.action = '-'
#########################################3	
				r, t, T = self.GET_TIME()
				score, = self.GET_MY_SCORE()
				allscores = self.GET_ALL_SCORES()
				place = 1+sum( 1 for s in allscores if s > score )
				what = ''
				reason = ''
				
				if t >= self.reaload_at:
					self.shots_left = 3
				
				while self.look_history and t-self.look_history[0][0] > 6:
					self.look_history.popleft()
			
				self.DESCRIBE_WORLD()
				self.DESCRIBE_MONSTERS()
				if self.sniper.pos() != (-1,-1):
					try:
						self.LOOK()
						self.look_history.append( (t, self.sniper.z) )
					except Failed(111):
						pass
				else:
					self.enemies = []
					
				
				snipers = set( s.pos() for s in self.snipers if s.pos() != (-1, -1) )
				flags = set( f.pos() for f in self.flags if f.z == 0 and f.pos() not in snipers )
				my_monsters = set( p.pos() for p in self.monsters )
				self.enemies = filter( lambda e : e.z != self.id, self.enemies )
				enemies = set( e.pos() for e in self.enemies )
				
				carried_flags = sum( 1 for f in self.flags if f.z == self.id )
				
				self.bfs( lambda f : f.pos() == self.sniper.pos(), 'dist_to_sniper')
				self.bfs( lambda f : f.pos() in snipers, 'dist_to_snipers')
				self.bfs( lambda f : f.pos() in flags, 'dist_to_flag')
				self.bfs( lambda f : f.pos() == self.base.pos(), 'dist_to_base')
				self.bfs( lambda f : f.pos() in enemies, 'dist_to_enemy')
				
				for f in self.map.values():
					targets = []
					if carried_flags < self.max_carry:
						targets.append( f.dist_to_flag )
					if carried_flags > 0:
						targets.append( f.dist_to_base )
						
					f.dist_to_target = min( targets )
				
				dist_to_target = self.map.get( self.sniper.pos(), -1 )
				if dist_to_target != -1: dist_to_target = dist_to_target.dist_to_target
				
				if r == previous_round and t-previous_turn > 1:
					log.log( 'Jumped from turn %d to %d!' % (previous_turn, t) )
				previous_round = r
				previous_turn = t
				self.shotline = []
				
				if self.sniper.pos() == (-1,-1):
					self.RESPAWN()
					self.shots_left = 3
				else:
					vx, vy = -math.sin(self.sniper.z*math.pi/180), math.cos(self.sniper.z*math.pi/180)
					dx = dy = 0
					if vx == 0: vx = 0.00001
					if vy == 0: vy = 0.00001
					
					while dx**2 + dy**2 < 300:
						step = min((int(abs(dx)+1)-abs(dx))/abs(vx), (int(abs(dy)+1)-abs(dy))/abs(vy))
						dx += step * vx
						dy += step * vy
						
						p = int(self.sniper.x+dx), int(self.sniper.y+dy)
						if p not in self.map:
							break
							
						self.shotline.append(p)
						
					shotline_s = set(self.shotline)
						
					# self.map[e.pos()].dist_to_sniper < 4 or (self.map[e.pos()].dist_to_sniper+1 < self.map[e.pos()].dist_to_snipers and self.map[e.pos()].dist_to_target < dist_to_target
					enemies = filter( lambda e : self.map[e.pos()].dist_to_target < dist_to_target, self.enemies)
					enemies_s = set( e.pos() for e in enemies )
					self.bfs( lambda f : f.pos() in enemies_s, 'dist_to_valid_enemy')
					
					aggro = filter( lambda e : self.map[e.pos()].dist_to_sniper == 1, self.enemies )
					aggro_not_targeted = filter( lambda e : e.pos() not in shotline_s or self.shots_left == 0, aggro )
					count = sum( 1 for e in enemies if e.pos() in shotline_s and self.shots_left )
							
					if aggro_not_targeted:
						run_to = filter( lambda f : f.dist_to_sniper == 1, self.map.values() )
						if run_to:
							run_to = min( run_to, key = lambda f : (-min(2, f.dist_to_enemy), f,dist_to_target, angle_diff(get_angle(f.x-self.sniper.x, f.y-self.sniper.y), self.sniper.z)) )
							angle = int(get_angle( run_to.x-self.sniper.x, run_to.y-self.sniper.y ) )
							dir = {0:'E', 90:'N', 180:'W', 270:'S'}.get(angle, 'unknown')
							if dir != 'unknown':
								self.MOVE( dir )
								reason = 'running'
					elif dist_to_target == 0 and self.sniper.pos() in flags:
						self.PICK_UP()
						reason = 'picking'
					else:
						if count > 0:
							try:
								self.SHOOT()
								self.shots_left -= 1
								self.shot_at = t
								self.reaload_at = t+5
								reason = 'shooting'
							except Failed(104):
								self.shots_left = 0
								self.reload_at = self.shot_at+5
								count = 0
								
						if count == 0:
							tires = 0
							if t+1 >= self.reload_at:
								targets = filter( lambda f : f.dist_to_valid_enemy == 1, self.map.values() )
								if targets:
									target = min( targets, key = lambda f : f.dist_to_sniper )
									angle = int(get_angle( target.x-self.sniper.x, target.y-self.sniper.y ))
									if angle_diff(angle, self.sniper.z) > 5:
										self.ROTATE( angle )
										reason = 'targeting'
									else:
										tires = 100000
								else:
									angle = self.sniper.z
									recent = 1
									while recent > 0 and tires < 5:
										angle = (self.sniper.z + 107) % 360
										recent = sum( 1 for (t,a) in self.look_history if angle_diff(a, angle) < 115 )
										tires += 1
										
									if tires < 5:
										log.log('Found new angle after %d tires (%s)' % (tires, str(self.look_history)) )
										self.ROTATE( angle )
										reason = 'looking'
							
							if dist_to_target > 0 and t+1 < self.reload_at or tires >= 5:
								targets = list(flags) if carried_flags < self.max_carry else []
								if carried_flags > 0:
									targets.append((self.base.x, self.base.y))
								flag = min( targets, key = lambda f : self.map[f].dist_to_sniper )
								angle = get_angle( flag[0]-self.sniper.x, flag[1]-self.sniper.y )
								if angle_diff( angle, self.sniper.z ) > 57:
									self.ROTATE( int(angle) )
									reason = 'pathfinding'
								else:
									targets = filter( lambda f : f.dist_to_sniper == 1 and f.dist_to_enemy > 1, self.map.values() )
									if targets:
										target = min( targets, key = lambda f : (f.dist_to_target, angle_diff(get_angle(f.x-self.sniper.x, f.y-self.sniper.y), self.sniper.z)) )
										angle = int(get_angle( target.x-self.sniper.x, target.y-self.sniper.y ) )
										dir = {0:'E', 90:'N', 180:'W', 270:'S'}.get(angle, 'unknown')
										if dir != 'unknown':
											self.MOVE( dir )
											reason = 'wandering'
									else:
										self.ROTATE( randint(0,359) )
					
					"""if count == 0:
				
						if dist_to_flag == 0 and carried_flags < 5 and not must_run:
							self.PICK_UP()
							log.log('picking up flag')
						else:
							
							flag = min( filter( lambda f : f.z == 0, self.flags ), key = lambda f : self.map[f.pos()].dist_to_sniper)
							alpha = get_angle( flag.x-self.sniper.x, flag.y-self.sniper.y )

							if angle_diff(alpha, self.sniper.z) > 45 and not must_run:
								self.ROTATE(int(alpha))
							else:
								dirs = [ ('N',-1,0), ('S',1,0), ('E',0,1), ('W',0,-1) ]
								dirs = [ (d, self.map.get((self.sniper.x+dx, self.sniper.y+dy), None)) for d,dx,dy in dirs ]
								dirs = filter( lambda (d,f) : f and f.dist_to_enemy > 0, dirs )
												
								if dirs:
									if carried_flags == 0:
										dir = min( dirs, key = lambda (d,f) : (f.dist_to_enemy==1, f.dist_to_flag) )[0]
									elif carried_flags < 5:
										dir = min( dirs, key = lambda (d,f) : (f.dist_to_enemy==1, min(f.dist_to_flag, f.dist_to_base)) )[0]
									else:
										dir = min( dirs, key = lambda (d,f) : (f.dist_to_enemy==1, f.dist_to_base) )[0]
									self.MOVE( dir )"""
						
						
				log.status(STATUS_MSG % (r,t,T,score,place,self.m,self.id,self.sniper.x, self.sniper.y, self.action, reason, self.shots_left, dist_to_target, carried_flags, len(self.bonuses)) )
				
				self.save_map()
				if self.the_r < 300:
					try:
						msg = self.BONUS_INFO(self.the_r)
						log.log('Bonus: '+ msg)
					except Failed(110):
						pass
					self.the_r += 1
				
				for m in self.monsters:
					if (m.x,m.y) in self.map:
						f = self.map[(m.x,m.y)]
						
						dirs = [ ('N',-1,0), ('S',1,0), ('E',0,1), ('W',0,-1) ]
						dirs = [ self.map.get((m.x+dx, m.y+dy), None) for d,dx,dy in dirs ]
						dirs = filter( lambda f : f, dirs )
						if dirs:
							dir = None
							dir = min(dirs, key = lambda f : (f.pos() in enemies_s, -min(3, f.dist_to_sniper), f.dist_to_snipers, f.dist_to_base) )
							try:
								self.SEND_MONSTER(m.id, dir.x, dir.y)
							except Failed(101):
								pass
					
					
				
#########################################3
				game.WAIT()
			except Failed(9):
				log.status('No current round')
				sleep(10)
			except Failed(6):
				game.WAIT()
			except Failed(103):
				game.WAIT()
			except Failed(102):
				log.log('Probably missed turn')
				game.WAIT()
			except Failed(108):
				log.log('Probably missed turn')
				game.WAIT()
			#except Failed(108):
			#	game.WAIT()
			#except Failed(112):
			#	game.WAIT()
		


	def while_waiting(self, wait_for):
		pass

	def run(self):
		self.thread = Thread(target=self.loop)
		self.thread.setDaemon(True)
		self.thread.start()

while True:
	try:
		game = Game()
		game.loop()
	except:
		sleep(10)
	
print 'Running'
log.log('App running')

def show():
	reload(display).Display(game)
