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
gravit_SOURCES=src/color.c src/command.c src/command.h src/config.c src/console.c src/font.c src/font.h src/fps.c src/frame-ot.c src/frame-pp.c src/frame-pp_sse.c src/frame.c src/gfx.c src/gravit.h src/input.c src/main.c src/osd.c src/sdlk.h src/spawn.c src/texture.c src/timer.c src/tool.c src/png_save.c
EXTRA_DIST=README COPYING cfg/gravit.cfg demo.cfg cfg/screensaver.cfg ChangeLog Makefile.old $(misc_DATA) $(spawn_DATA)
sysconf_DATA = cfg/gravit.cfg cfg/screensaver.cfg

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
LIBS += -Wl,-rpath=/usr/local/lib
else
AM_CPPFLAGS = -DSYSCONFDIR=\"$(sysconfdir)/\" -DDATA_DIR=\"$(datadir)/\" $(LUA_CFLAGS) -DWITHOUT_AGAR
endif


# Rule to build windows resources
#%.o : %.rc
src/gravitrc.o: src/gravit.rc
	$(WINDRES) $< $@
