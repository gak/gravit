FINAL = gravit
OBJS = 	main.o font.o frame.o frame-pp.o frame-ot.o gfx.o input.o console.o osd.o spawn.o tool.o command.o fps.o color.o config.o

CFLAGS = -g -O4 -Wall `sdl-config --cflags`
LDFLAGS = -lGL -lGLU -lSDL_ttf `sdl-config --libs`

DISTDIR = gravit-0.2-devel

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
	if [ -d $(DISTDIR) ]; then rm -fr $(DISTDIR); fi
	mkdir $(DISTDIR)
	cp README COPYING Vera.ttf gravit.cfg *.c *.h Makefile gravit.dsp gravit.dsw $(DISTDIR)
	tar czvf dist/$(DISTDIR)-src.tgz $(DISTDIR)
	rm -fr $(DISTDIR)

packagewin:
	if [ -d $(DISTDIR) ]; then rm -fr $(DISTDIR); fi
	mkdir $(DISTDIR)
	cp README COPYING Vera.ttf gravit.cfg Release/gravit.exe $(DISTDIR)
	zip -r dist/$(DISTDIR)-win32.zip $(DISTDIR)
	rm -fr $(DISTDIR)

packagewinsdl:
	if [ -d $(DISTDIR) ]; then rm -fr $(DISTDIR); fi
	mkdir $(DISTDIR)
	cp README COPYING Vera.ttf gravit.cfg Release/gravit.exe SDL.dll SDL_ttf.dll $(DISTDIR)
	zip -r dist/$(DISTDIR)-win32-sdl.zip $(DISTDIR)
	rm -fr $(DISTDIR)

packageall: packagesrc packagewin packagewinsdl

copy:
	cp dist/$(DISTDIR)-* /www/gravit/dist/
