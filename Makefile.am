# This -*-Makefile-*- is created by automake

# path to cfg files
sysconfdir=@sysconfdir@/gravit
datadir=@datadir@/gravit
# path to ttf,png,etc
miscdir=@datadir@/gravit/data
# path to skyboxes
# ugly workaround: need one ...dir and one ..._DATA for each skybox
skybox1dir=@datadir@/gravit/data/skybox
skybox2dir=@datadir@/gravit/data/skybox/purplenebula
# path to .gravitspawn files
spawndir=@datadir@/gravit/spawn

sysconf_DATA = cfg/gravit.cfg cfg/screensaver.cfg
misc_DATA =$(shell echo data/*.??*)
skybox1_DATA =$(shell echo data/skybox/*.??*)
skybox2_DATA =$(shell echo data/skybox/purplenebula/*.??*)
spawn_DATA =$(shell echo spawn/*)

bin_PROGRAMS=gravit
gravit_SOURCES=src/color.c src/command.c src/command.h src/config.c src/console.c src/font.c src/font.h src/fps.c src/frame-ot.c src/frame-pp.c src/frame-pp_sse.c src/frame.c src/gfx.c src/gravit.h src/input.c src/main.c src/osd.c src/sdlk.h src/spawn.c src/texture.c src/timer.c src/tool.c src/png_save.c
EXTRA_DIST=README COPYING cfg/gravit.cfg demo.cfg cfg/screensaver.cfg ChangeLog Makefile.old $(misc_DATA) $(spawn_DATA) $(skybox1_DATA) $(skybox2_DATA)

EXTRA_gravit_SOURCES=
gravit_LDADD=

if USE_ICON_RC
gravit_LDADD += src/gravitrc.o
endif

if LUA
#EXTRA_gravit_SOURCES += lua.c
gravit_SOURCES += src/lua.c
gravit_LDADD += $(LUA_LIBS)
endif

#AM_CPPFLAGS = -DSYSCONFDIR=\"$(sysconfdir)/\" -DDATA_DIR=\"$(datadir)/\" -O3 -Wall $(LUA_CFLAGS)

if AGAR
AM_CPPFLAGS = -DSYSCONFDIR=\"$(sysconfdir)/\" -DDATA_DIR=\"$(datadir)/\" $(LUA_CFLAGS)
LIBS += -Wl,-rpath=$(AGAR_LIBDIR):/usr/local/lib:../lib
else
AM_CPPFLAGS = -DSYSCONFDIR=\"$(sysconfdir)/\" -DDATA_DIR=\"$(datadir)/\" $(LUA_CFLAGS) -DWITHOUT_AGAR
endif

if DEBUG
AM_CPPFLAGS += -DDEBUG -DNDEBUG
endif

# Rule to build windows resources
#%.o : %.rc
src/gravitrc.o: src/gravit.rc
	$(WINDRES) $< $@
