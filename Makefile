FINAL = gravit
OBJS = 	main.o font.o frame.o frame-pp.o frame-ot.o gfx.o input.o console.o osd.o spawn.o tool.o command.o fps.o color.o config.o
BASEFILES = README COPYING Vera.ttf gravit.cfg demo.cfg screensaver.cfg ChangeLog particle.png gravit.png gravit.ico

CFLAGS = -O4 -Wall `sdl-config --cflags` -Wall
LDFLAGS = -L/usr/X11R6/lib -lGL -lGLU -lSDL_ttf -lSDL_image `sdl-config --libs`

DISTDIR = gravit-0.3.0
TMPBASE = /tmp
TMPDIR = $(TMPBASE)/$(DISTDIR)

all: final

dep:
	@makedepend $(CFLAGS) *.c 2> /dev/null

final: $(OBJS)
	@echo gcc $(LDFLAGS) $(OBJS) -o$(FINAL)
	@gcc $(LDFLAGS) $(OBJS) -o$(FINAL)

clean:
	rm -rf *.o $(FINAL)

.c.o:
	@echo gcc -c $(CFLAGS) $(subst ,, $*.c) -o$*.o
	@gcc -c $(CFLAGS) $(subst ,, $*.c) -o$*.o

packagesrc:
	if [ -d $(TMPDIR) ]; then rm -fr $(TMPDIR); fi
	if [ -f $(TMPBASE)/$(DISTDIR)-src.tgz ]; then rm -fr $(TMPBASE)/$(DISTDIR)-src.tgz; fi
	mkdir $(TMPDIR)
	cp $(BASEFILES) *.c *.h Makefile gravit.dsp gravit.dsw $(TMPDIR)
	cd $(TMPDIR); chmod 644 *
	cd $(TMPBASE); tar czvf $(DISTDIR)-src.tgz $(DISTDIR)
	cp $(TMPBASE)/$(DISTDIR)-src.tgz dist/
	cp $(TMPBASE)/$(DISTDIR)-src.tgz www/dist/
	rm -fr $(TMPDIR)

packagewin:
	if [ -d $(TMPDIR) ]; then rm -fr $(TMPDIR); fi
	if [ -f $(TMPBASE)/$(DISTDIR)-win32.zip ]; then rm -fr $(TMPBASE)/$(DISTDIR)-win32.zip; fi
	mkdir $(TMPDIR)
	cp $(BASEFILES) gravit.exe $(TMPDIR)
	cd $(TMPDIR); sed -i 's/$$/\r/' README COPYING *.cfg ChangeLog
	cd $(TMPDIR); mv README README.txt
	cd $(TMPDIR); mv ChangeLog ChangeLog.txt
	cd $(TMPBASE); zip -r $(DISTDIR)-win32.zip $(DISTDIR)
	cp $(TMPBASE)/$(DISTDIR)-win32.zip dist/
	cp $(TMPBASE)/$(DISTDIR)-win32.zip www/dist/
	rm -fr $(TMPDIR)

packagewindll:
	if [ -d $(TMPDIR) ]; then rm -fr $(TMPDIR); fi
	if [ -f $(TMPBASE)/$(DISTDIR)-win32-dll.zip ]; then rm -f $(TMPBASE)/$(DISTDIR)-win32-dll.zip; fi
	mkdir $(TMPDIR)
	cp $(BASEFILES) gravit.exe SDL.dll SDL_ttf.dll SDL_image.dll jpeg.dll libpng13.dll zlib1.dll $(TMPDIR)
	cd $(TMPDIR); sed -i 's/$$/\r/' README COPYING *.cfg ChangeLog
	cd $(TMPDIR); mv README README.txt
	cd $(TMPDIR); mv ChangeLog ChangeLog.txt
	if [ -f windowsinstaller/* ]; then rm windowsinstaller/*; fi
	cp -r $(TMPBASE)/$(DISTDIR)/* windowsinstaller/files/
	#cd $(TMPBASE); zip -r $(DISTDIR)-win32-dll.zip $(DISTDIR)
	#cp $(TMPBASE)/$(DISTDIR)-win32-dll.zip dist/
	#cp $(TMPBASE)/$(DISTDIR)-win32-dll.zip www/dist/
	rm -fr $(TMPDIR)

packageall: packagesrc packagewin packagewindll
