# path to cfg files
sysconfdir=@sysconfdir@/gravit
datadir=@datadir@/gravit
# path to ttf,png,etc
miscdir=@datadir@/gravit/data
# path to .gravitspawn files
spawndir=@datadir@/gravit/spawn

bin_PROGRAMS=gravit
gravit_SOURCES=color.c command.c command.h config.c console.c font.c font.h fps.c frame-ot.c frame-pp.c frame.c gfx.c gravit.h input.c main.c osd.c sdlk.h spawn.c texture.c timer.c tool.c
EXTRA_DIST=README COPYING gravit.cfg demo.cfg screensaver.cfg ChangeLog Makefile.old $(shell ls data/*) $(shell ls spawn/*)
sysconf_DATA = gravit.cfg screensaver.cfg
misc_DATA = $(shell ls data/*)
spawn_DATA = $(shell ls spawn/*)

if LUA
EXTRA_gravit_SOURCES = lua.c
gravit_LDADD = lua.o
else
EXTRA_gravit_SOURCES = 
gravit_LDADD =
endif

AM_CPPFLAGS = -DSYSCONFDIR=\"$(sysconfdir)\" -DDATA_DIR=\"$(datadir)\" -g -Wall

