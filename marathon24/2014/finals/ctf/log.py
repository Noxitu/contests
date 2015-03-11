from time import time
import config

def gettimestr():
	t = int(time()) + 2*3600
	return '%.2d:%.2d:%.2d' % (t/3600%24, t/60%60, t%60)

def net_in(msg):
	with open("%s/net.log" % config.path, "a") as f:
		for line in msg.split('\n'):
		    f.write(" ->\t%s\n" % line)

def net_out(msg):
	with open("%s/net.log" % config.path, "a") as f:
		for line in msg.split('\n'):
		    f.write("<- \t%s\n" % line)

def net_conn():
	with open("%s/net.log" % config.path, "a") as f:
	    f.write("\n-X-\n\n")

def log(msg):
	with open("%s/log.log" % config.path, "a") as f:
	    f.write("[%s]\t%s\n" % (gettimestr(), str(msg)))

def status(msg):
	with open("%s/status.log" % config.path, "w") as f:
	    f.write("%s\n" % str(msg))
