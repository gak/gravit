bin_PROGRAMS=gravit
gravit_SOURCES=color.c command.c command.h config.c console.c font.c font.h fps.c frame-ot.c frame-pp.c frame.c gfx.c gravit.h input.c main.c osd.c sdlk.h spawn.c texture.c timer.c tool.c

if LUA
EXTRA_gravit_SOURCES = lua.c
gravit_LDADD = lua.o
else
EXTRA_gravit_SOURCES = 
gravit_LDADD =
endif

AM_CPPFLAGS = @SDL_CFLAGS@
AM_LIBS = @SDL_LIBS@
