import pygame, pygame.locals
from threading import Thread, Lock
from time import sleep

def text(t, c, pos, s):
	fg = 0,0,0
	bg = c
	font = pygame.font.Font(None, 20)
	text = t
	size = font.size(text)
	ren = font.render(text, 0, fg, bg)
	s.blit(ren, pos)

class Display:
	WIDTH = 800
	HEIGHT = 600

	def __init__(self, game):
		self.zoom = 3
		self.game = game
		self.pos = (0,0)
		self.drag_from = None
		self.active_ship = None

		pygame.init()
		pygame.display.set_mode( (self.WIDTH, self.HEIGHT) )

		self.surface = pygame.Surface( (self.WIDTH, self.HEIGHT) )

		self.thread = Thread(target=self.loop)
		self.thread.setDaemon(True)
		self.thread.start()

	def event(self, event):
		if event.type == pygame.QUIT:
			raise SystemExit
		elif event.type == pygame.KEYUP:
			if event.key == pygame.locals.K_ESCAPE:
				raise SystemExit
		elif event.type == pygame.MOUSEBUTTONDOWN:
			if event.button == 2:
				self.drag_from = event.pos
			if event.button == 3:
				pos = self.screen2game(*event.pos)
				if self.active_ship and self.active_ship.role != 'CAPTAIN':
					self.game.SI[self.active_ship.id] = (21, (pos,pos, False))

		elif event.type == pygame.MOUSEBUTTONUP:
			if event.button == 1:
				pos = self.screen2game(*event.pos)
				self.active_ship = min(self.game.Survivors.values(), key=lambda man: abs(pos[0]-man.x)+abs(pos[1]-man.y))
			if event.button == 2:
				self.drag_from = None
			if event.button == 3:
				pos = self.screen2game(*event.pos)
				if self.active_ship:
					if self.active_ship.role != 'CAPTAIN':
						pos1 = self.game.SI[self.active_ship.id][1][0]
						self.game.SI[self.active_ship.id] = (21, (pos1,pos, False))
					else:
						self.game.bonfire = pos
			if event.button == 4:
				self.pos = self.screen2game(*event.pos)
				self.zoom += 1
				self.pos = self.pos[0]*self.zoom-event.pos[0], self.pos[1]*self.zoom-event.pos[1]
			if event.button == 5:
				if self.zoom > 1:
					self.pos = self.screen2game(*event.pos)
					self.zoom -= 1
					self.pos = self.pos[0]*self.zoom-event.pos[0], self.pos[1]*self.zoom-event.pos[1]
		elif event.type == pygame.MOUSEMOTION:
			if self.drag_from != None:
				delta = event.pos[0] - self.drag_from[0], event.pos[1] - self.drag_from[1]
				self.pos = self.pos[0]-delta[0], self.pos[1]-delta[1]
				self.drag_from = event.pos
#		else:
#			print event

	def loop(self):
		try:
			while True:
				for e in pygame.event.get():
					self.event(e)

				self.show()
				pygame.display.flip()
				sleep(0.15)
		except SystemExit:
			pygame.quit()
		except Exception as e:
			print e
			pygame.quit()

	def screen2game(self,x,y):
		posx, posy = self.pos
		return (x+posx)/self.zoom, (y+posy)/self.zoom
	
	def game2screen(self,x,y):
		posx, posy = self.pos
		return max(0, x*self.zoom-posx), max(y*self.zoom-posy, 0), min(x*self.zoom+self.zoom-posx, self.WIDTH), min(y*self.zoom+self.zoom-posy, self.HEIGHT)

	def show(self):
		ar = pygame.PixelArray(self.surface)
		ar[0:self.WIDTH,0:self.HEIGHT] = (0,0,0)
		sx, sy = self.game2screen(0,0)[:2]
		ex, ey = self.game2screen(self.game.N, self.game.N)[2:]
		ar[sx:ex, sy:ey] = (0,0,8)
		max_p = max(self.game.best.values()) ** 3

		T_Q = []

		for (x,y), p in self.game.lands.items():
			sx, sy, ex, ey = self.game2screen(x,y)
			if ex > sx and ey > sy:
				ar[sx:ex, sy:ey] = (75,25,0) if p > 0 else (0,0,50)
				if self.zoom > 12 and (x,y) not in self.game.best:
					T_Q.append(( str(p), (75,25,0) if p > 0 else (0,0,50), (sx, sy), ))

		for (x,y), p in self.game.best.items():
			t = p
			p = p ** 3
			sx, sy, ex, ey = self.game2screen(x,y)
			if ex > sx and ey > sy:
				ar[sx:ex, sy:ey] = (170*p/max_p+75,125*p/max_p+25,0)
				if self.zoom > 12:
					T_Q.append(( str(t), (170*p/max_p+75,125*p/max_p+25,0), (sx, sy), ))


		for id, (mode,data) in self.game.SI.items():
			if data[0]:
				x,y = self.game.Survivors[id].x, self.game.Survivors[id].y
				data = data[0]
				while (x,y) != data:
					dx = data[0] - x
					dy = data[1] - y
					if abs(dx)>abs(dy):
						dx /= abs(dx)
						dy = 0
					else:
						dx = 0
						dy /= abs(dy)
					x += dx
					y += dy
					sx, sy, ex, ey = self.game2screen(x,y)
					if ex > sx and ey > sy:
						ar[sx:ex, sy:ey] = (150,0,255)

		surv_txt = {}
		for man in self.game.Survivors.values():
			sx, sy, ex, ey = self.game2screen(man.x,man.y)
			if ex > sx and ey > sy:
				color = (0,255,255)
				if self.active_ship and man.id == self.active_ship.id:
					color = (255,0,255)
				ar[sx:ex, sy:ey] = color
				if self.zoom > 12:
					pp = (sx, sy+self.zoom/2)
					surv_txt[pp] = surv_txt.get(pp, 0) + man.sticks

		for p, t in surv_txt.items():
			T_Q.append(( str(t), (0,255,255), p ))
#		for y in xrange(0, self.HEIGHT, self.zoom):
#			y = max(0,y)
#			ey = min(self.HEIGHT-1, y+self.zoom)
#			py = y/self.zoom + 1
#			if py > self.game.N: continue
#			for x in xrange(0, self.WIDTH, self.zoom):
#				x = max(0,x)
#				ex = min(self.WIDTH-1, x+self.zoom)
#				px = x/self.zoom + 1
#				if px > self.game.N: continue
#
#				color = (0,0,6)
#				px = x/self.zoom
#				py = y/self.zoom
#				r = ((px-self.game.N/2)**2+(py-self.game.N/2)**2) ** .5
#				R = self.game.N/2

#				if (px,py) in self.game.best:
#					color = (255,128,0)
#				elif R > r > R/2:
#					color = (0,16,0)
#				ar[x:ex, y:ey] = color
		del ar

		screen = pygame.display.get_surface()
		screen.fill( (0, 0, 0) )
		screen.blit(self.surface, (0, 0) )

		for t, c, p in T_Q:
			text(t, c, p, screen)

