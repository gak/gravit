#!/usr/bin/python

from trac.WikiFormatter import wiki_to_html
from trac import Environment
import sys

env = Environment.Environment("/trac/gravit")
env.href = ""

print wiki_to_html(sys.stdin.read(), "", env, "")
