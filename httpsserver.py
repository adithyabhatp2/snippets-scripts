# adapated from http://www.piware.de/2011/01/creating-an-https-server-in-python/
# generate seperate key+crt files, make sure common name (CN) == ip or hostname
#    openssl req -x509 -sha256 -nodes -days 365 -newkey rsa:2048 -keyout newkey.key -out newkey.crt
# run as follows:
#    python simple-https-server.py

import BaseHTTPServer, SimpleHTTPServer
import ssl

# 0.0.0.0 allows connections from anywhere
httpd = BaseHTTPServer.HTTPServer(('0.0.0.0', 4443), SimpleHTTPServer.SimpleHTTPRequestHandler)
httpd.socket = ssl.wrap_socket (httpd.socket, certfile='./newkey.crt', keyfile='./newkey.key', server_side=True)
print "going to serve"
httpd.serve_forever()