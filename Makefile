FINAL = graviton
OBJS = 	main.o font.o frame.o frame-pp.o frame-ot.o gfx.o input.o console.o osd.o spawn.o tool.o command.o fps.o color.o

CFLAGS = -g -O4 -Wall `sdl-config --cflags`
LFLAGS = -lGL -lGLU -lSDL_image -lSDL_ttf `sdl-config --libs`

DISTDIR = graviton-0.1

all: final

final: $(OBJS)
	@echo gcc $(LFLAGS) $(OBJS) -o$(FINAL)
	@gcc $(LFLAGS) $(OBJS) -o$(FINAL)

clean:
	rm -rf *.o $(FINAL)

(%.o): %.c
	@echo gcc -c $(CFLAGS) $(subst ,, $*.c) -o$*.o
	@gcc -c $(CFLAGS) $(subst ,, $*.c) -o$*.o

package:
	rm -fr ../$(DISTDIR)
	mkdir ../$(DISTDIR)
	cp *.c *.h Makefile graviton.dsp graviton.dsw README COPYING ../$(DISTDIR)
	cd .. && tar czvf $(DISTDIR)-src.tgz $(DISTDIR)
	rm -fr ../$(DISTDIR)
