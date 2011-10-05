# path to cfg files
sysconfdir=@sysconfdir@/gravit
datadir=@datadir@/gravit
# path to ttf,png,etc
miscdir=@datadir@/gravit/data
# path to .gravitspawn files
spawndir=@datadir@/gravit/spawn

misc_DATA =$(shell echo data/*)
spawn_DATA =$(shell echo spawn/*)

bin_PROGRAMS=gravit
gravit_SOURCES=color.c command.c command.h config.c console.c font.c font.h fps.c frame-ot.c frame-pp.c frame-pp_sse.c frame.c gfx.c gravit.h input.c main.c osd.c sdlk.h spawn.c texture.c timer.c tool.c png_save.c
EXTRA_DIST=README COPYING gravit.cfg demo.cfg screensaver.cfg ChangeLog Makefile.old $(misc_DATA) $(spawn_DATA)
sysconf_DATA = gravit.cfg screensaver.cfg

EXTRA_gravit_SOURCES=
gravit_LDADD=

if USE_ICON_RC
gravit_LDADD += gravitrc.o
endif

if LUA
EXTRA_gravit_SOURCES += lua.c
gravit_LDADD += lua.o $(LUA_LIBS)
endif

#AM_CPPFLAGS = -DSYSCONFDIR=\"$(sysconfdir)/\" -DDATA_DIR=\"$(datadir)/\" -O3 -Wall $(LUA_CFLAGS)
AM_CPPFLAGS = -DSYSCONFDIR=\"$(sysconfdir)/\" -DDATA_DIR=\"$(datadir)/\" $(LUA_CFLAGS)


# Rule to build windows resources
#%.o : %.rc
gravitrc.o: gravit.rc
	$(WINDRES) $< $@
