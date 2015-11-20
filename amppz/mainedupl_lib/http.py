import urllib2, cookielib

class NoRedirection(urllib2.HTTPErrorProcessor):

    def http_response(self, request, response):
        return response

    https_response = http_response


jar = cookielib.CookieJar()
opener = urllib2.build_opener(NoRedirection, urllib2.HTTPCookieProcessor(jar))


def http_ex(url, data = None):
	if data:
		data = '&'.join( key+'='+quote(str(value)) for key, value in data )
	
	req = urllib2.Request(url, data)
	res = opener.open(req)
	return res
    
def http(*a, **kw):
    return http_ex(*a, **kw).read()