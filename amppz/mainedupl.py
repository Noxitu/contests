#!/usr/bin/python
"""Main.edu.pl mirror'er and judge

Usage:
  mainedupl [options] get <contest> <edition> <task>
  mainedupl [options] list [<contest>] [<edition>]

Options:
  -h --help       Show this screen.
  -v --version    Show version.
  -f --force      Force redownload of page

Arguments: 
"""

from mainedupl_lib.docopt import docopt
from mainedupl_lib.http import http, http_ex
from cStringIO import StringIO
import zipfile

import xml.etree.ElementTree
import re, os, os.path, sys

class Data:
    PATH = 'mainedupl_data/data.xml'
    VERSION = '1'
    def __init__(self):
        try:
            self.data = xml.etree.ElementTree.parse(self.PATH).getroot()
            assert self.data.get('version') == self.VERSION
            self.changed = False
        except:
            self.data = xml.etree.ElementTree.Element('root')
            self.data.set('url', 'http://main.edu.pl/pl/archive')
            self.data.set('cached', 'false')
            self.data.set('version', self.VERSION)
            self.changed = True
            
            
    def save(self):
        if not self.changed:
            return
        with open(self.PATH, 'w') as f:
            for s in xml.etree.ElementTree.tostringlist(self.data):
                f.write(s)
                
    def download(self, elem):
        self.changed = True
        while len(elem) > 0:
            del elem[0]
            
        print >>sys.stderr, 'Downloading', elem.get('url')
        data = http(elem.get('url')).decode('iso-8859-2')
        data = data.split("<div id='body' ")[1].split('</div')[0].split('<hr/>')[0].replace('\n', ' ')
        regex = {'root': r'<a href="user\.phtml\?op=show&page=(.+?)_list">(.+?)</a>'}.get(elem.tag, r'<a href="(user\.phtml.+?)">(.+?)</a>')
        tag_name = {'root': 'contest', 'contest': 'edition', 'edition': 'task'}.get(elem.tag)
        
        if elem.tag == 'edition':
            data2 = http('http://main.edu.pl/pl/user.phtml?op=zasoby').decode('iso-8859-2')
            data2 = data2.split("<div id='body'")[1]
            M = re.findall(regex, data2)
            tests = {}
            for url, name in M:
                tests[name.replace('&nbsp;', ' ').split(' - testy')[0].strip()] = url
                
        M = re.findall(regex, data)
        for id, name in M:
            name = name.strip()
            #print id, name
            e = xml.etree.ElementTree.SubElement(elem, tag_name)
            e.set('name', name)
            e.set('cached', 'false')
            if elem.tag == 'root':
                e.set('id', id)
                e.set('url', 'http://main.edu.pl/pl/archive/%s' % id)
            else:
                res = http_ex('http://main.edu.pl/pl/%s' % id)
                url = res.headers.get('location')
                res.read()
                id = url.split('/')[-1]
                e.set('id', id)
                e.set('url', url)
                
                if elem.tag == 'edition':
                    if name not in tests:
                        print "!!!!! Couldn't find tests for '%s'" % name
                    else:
                        e.set('tests_url', 'http://main.edu.pl/pl/%s' % tests[name])
                        del tests[name]

        if elem.tag == 'edition':
            for name in tests:
                print "!!!!! Left with tests for unknown task '%s'" % name
                
            
        elem.set('cached', 'true')
    
if not os.path.isdir('mainedupl_data'):
    os.mkdir('mainedupl_data')

if not os.path.isdir('mainedupl_data/tests'):
    os.mkdir('mainedupl_data/tests')

if __name__ == '__main__':
    args = docopt(__doc__, version='_____ 0.1')
    data = Data()
    
    chain = [None, args['<task>'], args['<edition>'], args['<contest>'], True]
    path = [ c for c in chain[1:-1] if c ][::-1]
    while chain[-1]:
        filter = chain.pop()
        if filter is True:
            e = data.data
        else:
            e = [e for e in e.iter() if e.get('id') == filter]
            if not e:
                print >>sys.stderr, '"%s" not found' % filter
                sys.exit(0)
            e = e[0]
        if e.tag == 'task':
            break
            
        if e.get('cached') != 'true' or (args['--force'] and not chain[-1]):
            data.download(e)
        
    if args['list']:
        def print_tree(elem, indent = 0):
            for e in elem:
                print '%s * %s [%s]' % (' '*indent, e.get('name'), e.get('id'))
                print_tree(e, indent+4)
                
        print_tree(e)

    elif args['get']:
        directory = 'mainedupl_data/tests/%s' % '_'.join(path)
        if os.path.exists(directory):
            if os.path.isdir(directory):
                if not args['--force']:
                    print >>sys.stderr, 'Directory "%s" exists, use --force to replace' % directory
                    sys.exit(0)
                else:
                    os.rmdir(directory)
            else:
                print >>sys.stderr, "File \"%s\" exists, can't continue" % directory
                sys.exit(0)
        print 'Downloading', e.get('tests_url'), 'into', directory
        os.makedirs(directory)
        z = http(e.get('tests_url'))
        z = StringIO(z)
        z = zipfile.ZipFile(z)
        z.extractall(directory)
        
    
    data.save()
    #print open(Data.PATH).read()