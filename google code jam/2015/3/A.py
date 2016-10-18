#!/usr/bin/python

from utils import iline
from heap import heap
from gchack import gchack

class Tree(object):
    __slots__ = ['salary', 'child', 'parent', 'in_tree']
    def __init__(self, salary):
        self.salary = salary
        self.child = []
        self.parent = None
        self.in_tree = False

def sequence(N):
    val, mul, add, mod = iline()
    def generator(val):
        for _ in xrange(N):
            yield val
            val = (val * mul + add) % mod
    return generator(val)

def test():
    with gchack:
        N, D = iline()
        S = sequence(N)
        M = sequence(N)
        yield
        
        tree = map(Tree, S)
        root = tree[0]
        
        for i, m in enumerate(M):
            if i > 0:
                m = m%i
                tree[m].child.append(tree[i])
                tree[i].parent = tree[m]
        
        tree_size = [0]
        H = heap()
        
        def add_tree(tree, smin, smax):
            T = [tree]
            for tree in T:
                if tree.parent and tree.parent.in_tree == False:
                    continue
                if smin <= tree.salary <= smax:
                    tree_size[0] += 1
                    tree.in_tree = True
                    H.push( (tree.salary+D, 1, tree) )
                    for child in tree.child:
                        T.append(child)
                elif tree.salary > smax:
                    H.push( (tree.salary, 0, tree) )
               
        def remove_tree(tree):
            T = [tree]
            for tree in T:
                if tree.in_tree:
                    tree.in_tree = False
                    tree_size[0] -= 1
                    for child in tree.child:
                        T.append(tree)
             
        add_tree(root, root.salary-D, root.salary)
        answer = tree_size[0]
        while True:
            try:
                salary, _, tree = H.pop()
            except IndexError:
                break
            if tree.in_tree:
                remove_tree(tree)
            else:
                add_tree( tree, salary-D, salary )
            answer = max( answer, tree_size[0] )
        
        print answer
            
