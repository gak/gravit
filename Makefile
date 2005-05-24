FINAL = gravit
OBJS = 	main.o font.o frame.o frame-pp.o frame-ot.o gfx.o input.o console.o osd.o spawn.o tool.o command.o fps.o color.o config.o timer.o lua.o

CFLAGS = -O4 -Wall `sdl-config --cflags` -Wall
LDFLAGS = -L/usr/X11R6/lib -lGL -lGLU -lSDL_ttf -lSDL_image `sdl-config --libs` -llua -llualib

all: final

dep:
	@makedepend $(CFLAGS) *.c 2> /dev/null

final: $(OBJS)
	gcc $(LDFLAGS) $(OBJS) -o $(FINAL)

clean:
	rm -f *.o $(FINAL)

.c.o:
	gcc -c $(CFLAGS) $(subst ,, $*.c) -o $*.o

