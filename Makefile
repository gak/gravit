FINAL = graviton
OBJS = 	main.o font.o frame.o frame-pp.o frame-ot.o gfx.o input.o console.o osd.o spawn.o tool.o command.o fps.o color.o config.o

CFLAGS = -g -O4 -Wall `sdl-config --cflags`
LDFLAGS = -lGL -lGLU -lSDL_image -lSDL_ttf `sdl-config --libs`

DISTDIR = graviton-0.1

all: dep final

dep:
	@makedepend $(CFLAGS) *.c 2> /dev/null

final: $(OBJS)
	@echo gcc $(LDFLAGS) $(OBJS) -o$(FINAL)
	@gcc $(LDFLAGS) $(OBJS) -o$(FINAL)

clean:
	rm -rf *.o $(FINAL)

(%.o): %.c
	@echo gcc -c $(CFLAGS) $(subst ,, $*.c) -o$*.o
	@gcc -c $(CFLAGS) $(subst ,, $*.c) -o$*.o

packagesrc:
	rm -fr ../$(DISTDIR)
	mkdir ../$(DISTDIR)
	cp README COPYING Vera.ttf graviton.cfg *.c *.h Makefile graviton.dsp graviton.dsw ../$(DISTDIR)
	cd .. && tar czvf graviton/dist/$(DISTDIR)-src.tgz $(DISTDIR)
	rm -fr ../$(DISTDIR)

packagewin:
	rm -fr ../$(DISTDIR)
	mkdir ../$(DISTDIR)
	cp README COPYING Vera.ttf graviton.cfg Release/graviton.exe ../$(DISTDIR)
	cd .. && zip -r graviton/dist/$(DISTDIR)-win32.zip $(DISTDIR)
	rm -fr ../$(DISTDIR)

packagewinsdl:
	rm -fr ../$(DISTDIR)
	mkdir ../$(DISTDIR)
	cp README COPYING Vera.ttf graviton.cfg Release/graviton.exe SDL.dll SDL_ttf.dll ../$(DISTDIR)
	cd .. && zip -r graviton/dist/$(DISTDIR)-win32-sdl.zip $(DISTDIR)
	rm -fr ../$(DISTDIR)

packageall: packagesrc packagewin packagewinsdl
