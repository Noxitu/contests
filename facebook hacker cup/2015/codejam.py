
import sys

inputFileList = [None]

if len(sys.argv) > 1:
	inputFileList = sys.argv[1:]
	for inputFile in inputFileList:
		if inputFile[-3:] != '.in':
			raise Exception("Input filename must match *.in")

	inputFile = inputFileList[0]


def readline(): return sys.stdin.readline().strip().split(' ')
    
def readiline(): return map( int, readline() )
def readfline(): return map( float, readline() )

def codejam(testCallback, newline=False):
	for inputFile in inputFileList:
		if inputFile != None:
		 	sys.stdin = open(inputFile, 'rt')
			sys.stdout = open(inputFile[:-3]+'.out', 'wt')

		T, = readiline()
		for i in xrange(1, T+1):
			sys.stdout.write( ('Case #%d:' % i) + ('\n' if newline else ' ') )
			testCallback()

		if inputFile != None:
			sys.stdin.close()
			sys.stdout.close()

			sys.stdin, sys.stdout = sys.__stdin__, sys.__stdout__

