 
import heapq

class heap:
    def __init__(self, data=[], key=None):
        self.__key = key
        if self.__key:
            self.__heap = [ (self.__key(x), x) for x in data ]
        else:
            self.__heap = list(data)
        heapq.heapify( self.__heap )
        
    def push(self, x):
        if self.__key:
            x = (self.__key(x), x)
        heapq.heappush( self.__heap, x )
        
    def pop(self):
        x = heapq.heappop( self.__heap )
        return x[1] if self.__key else x