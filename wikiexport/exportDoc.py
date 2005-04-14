#!/usr/bin/python

import os

pages = ['Docs/Index', 'Docs/Console', 'Docs/StereoMode']

# dodgy! what are u gonna do? :)
# todo: find a way not to be lazy

f = open("header.html", "rb")
header = f.read()
f.close()

f = open("footer.html", "rb")
footer = f.read()
f.close()

for page in pages:
	print page
	filename = "../www/" + page + ".php";
	os.system('trac-admin /trac/gravit wiki export ' + page + '>tmp')
	os.system('./wiki2html.py < tmp > lah')
	f = open('lah', "rb")
	buf = f.read()
	f.close()
	
	page = page.replace("Docs/","")
	buf = buf.replace("/Docs/","")
	
	buf = header.replace("[[TITLE]]", page) + buf + footer
	
	f = open(filename, "wb")
	f.write(buf)
	f.close()
	
