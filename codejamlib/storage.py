
import shelve, sys

def store(name, version=None, subname='object'):
  def decorator(callable):
    try:
      storage = shelve.open('{}.shelve'.format(name))
      if version is not None and storage.has_key(subname+'_version') and storage[subname+'_version'] == version:
        print >>sys.stderr, "Loading {}...".format(callable.__name__)
        object = storage[subname]
      else:
        print >>sys.stderr, "Calculating {}...".format(callable.__name__)
        storage[subname] = object = callable()
        storage[subname+'_version'] = version
      print >>sys.stderr, "Done"
      return object
    finally:
      storage.close()
  return decorator