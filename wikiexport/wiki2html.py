#!/usr/bin/python

from trac.WikiFormatter import wiki_to_html
from trac import Environment
from trac.Wiki import populate_page_dict
# from trac import Href
import sys

class MyHref:
	def __init__(self, base):
		self.base = base

	def wiki(self, page=None, version=None, diff=0, history=0):
		bits = page.split("#", 1)
		if len(bits) == 2:
			return "/" + bits[0] + ".php#" + bits[1]
		else:
			return "/" + bits[0] + ".php"

env = Environment.Environment("/trac/gravit")
env.href = MyHref("")

database = env.get_db_cnx()
populate_page_dict(database, env)

out = wiki_to_html(sys.stdin.read(), "", env, "")

print out
