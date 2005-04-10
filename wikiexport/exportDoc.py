#!/usr/bin/python

import os

pages = ['Docs/Index','Docs/Overview']

# dodgy! what are u gonna do? :)
# todo: find a way not to be lazy

for page in pages:
	print page
	os.system('trac-admin /trac/gravit wiki export ' + page + '>tmp')
	os.system('./wiki2html.py < tmp > ../www/' + page + ".html")
	