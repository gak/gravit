FINAL = gravit
OBJS = 	main.o font.o frame.o frame-pp.o frame-ot.o gfx.o input.o console.o osd.o spawn.o tool.o command.o fps.o color.o config.o
BASEFILES = README COPYING Vera.ttf gravit.cfg demo.cfg ChangeLog particle.png gravit.png gravit.ico

CFLAGS = -O4 -Wall `sdl-config --cflags` -Wall
LDFLAGS = -L/usr/X11R6/lib -lGL -lGLU -lSDL_ttf -lSDL_image `sdl-config --libs`

DISTDIR = gravit-0.2
TMPBASE = /tmp
TMPDIR = $(TMPBASE)/$(DISTDIR)

all: dep final

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
	cp $(BASEFILES) Release/gravit.exe $(TMPDIR)
	cd $(TMPDIR); sed -i 's/$$/\r/' README COPYING *.cfg ChangeLog
	cd $(TMPBASE); zip -r $(DISTDIR)-win32.zip $(DISTDIR)
	cp $(TMPBASE)/$(DISTDIR)-win32.zip dist/
	cp $(TMPBASE)/$(DISTDIR)-win32.zip www/dist/
	rm -fr $(TMPDIR)

packagewindll:
	if [ -d $(TMPDIR) ]; then rm -fr $(TMPDIR); fi
	if [ -f $(TMPBASE)/$(DISTDIR)-win32-dll.zip ]; then rm -f $(TMPBASE)/$(DISTDIR)-win32-dll.zip; fi
	mkdir $(TMPDIR)
	cp $(BASEFILES) Release/gravit.exe SDL.dll SDL_ttf.dll SDL_image.dll jpeg.dll libpng13.dll zlib1.dll $(TMPDIR)
	cd $(TMPDIR); sed -i 's/$$/\r/' README COPYING *.cfg ChangeLog
	cd $(TMPBASE); zip -r $(DISTDIR)-win32-dll.zip $(DISTDIR)
	cp $(TMPBASE)/$(DISTDIR)-win32-dll.zip dist/
	cp $(TMPBASE)/$(DISTDIR)-win32-dll.zip www/dist/
	rm -fr $(TMPDIR)

packageall: packagesrc packagewin packagewindll
# DO NOT DELETE

color.o: gravit.h /usr/include/stdlib.h /usr/include/features.h
color.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
color.o: /usr/include/string.h /usr/include/errno.h /usr/include/bits/errno.h
color.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
color.o: /usr/include/termios.h /usr/include/bits/termios.h
color.o: /usr/include/sys/select.h /usr/include/bits/types.h
color.o: /usr/include/bits/wordsize.h /usr/include/bits/typesizes.h
color.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
color.o: /usr/include/time.h /usr/include/bits/time.h /usr/include/sys/time.h
color.o: /usr/include/sys/types.h /usr/include/unistd.h
color.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
color.o: /usr/include/sys/timeb.h /usr/include/pthread.h /usr/include/sched.h
color.o: /usr/include/bits/sched.h /usr/include/signal.h
color.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/initspin.h
color.o: /usr/include/bits/sigthread.h /usr/include/dirent.h
color.o: /usr/include/bits/dirent.h /usr/include/sys/stat.h
color.o: /usr/include/bits/stat.h /usr/include/SDL/SDL.h
color.o: /usr/include/SDL/SDL_main.h /usr/include/SDL/SDL_types.h
color.o: /usr/include/SDL/SDL_getenv.h /usr/include/SDL/SDL_error.h
color.o: /usr/include/SDL/begin_code.h /usr/include/SDL/close_code.h
color.o: /usr/include/SDL/SDL_rwops.h /usr/include/stdio.h
color.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
color.o: /usr/include/bits/wchar.h /usr/include/gconv.h
color.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
color.o: /usr/include/SDL/SDL_timer.h /usr/include/SDL/SDL_audio.h
color.o: /usr/include/SDL/SDL_byteorder.h /usr/include/SDL/SDL_cdrom.h
color.o: /usr/include/SDL/SDL_joystick.h /usr/include/SDL/SDL_events.h
color.o: /usr/include/SDL/SDL_active.h /usr/include/SDL/SDL_keyboard.h
color.o: /usr/include/SDL/SDL_keysym.h /usr/include/SDL/SDL_mouse.h
color.o: /usr/include/SDL/SDL_video.h /usr/include/SDL/SDL_mutex.h
color.o: /usr/include/SDL/SDL_quit.h /usr/include/SDL/SDL_version.h
color.o: /usr/include/SDL/SDL_ttf.h /usr/include/SDL/SDL_opengl.h
color.o: /usr/include/GL/gl.h /usr/include/GL/glu.h
color.o: /usr/include/SDL/SDL_image.h /usr/include/math.h
color.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
color.o: /usr/include/bits/mathcalls.h /usr/include/ctype.h
color.o: /usr/include/endian.h /usr/include/bits/endian.h command.h font.h
command.o: gravit.h /usr/include/stdlib.h /usr/include/features.h
command.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
command.o: /usr/include/string.h /usr/include/errno.h
command.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
command.o: /usr/include/asm/errno.h /usr/include/termios.h
command.o: /usr/include/bits/termios.h /usr/include/sys/select.h
command.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
command.o: /usr/include/bits/typesizes.h /usr/include/bits/select.h
command.o: /usr/include/bits/sigset.h /usr/include/time.h
command.o: /usr/include/bits/time.h /usr/include/sys/time.h
command.o: /usr/include/sys/types.h /usr/include/unistd.h
command.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
command.o: /usr/include/sys/timeb.h /usr/include/pthread.h
command.o: /usr/include/sched.h /usr/include/bits/sched.h
command.o: /usr/include/signal.h /usr/include/bits/pthreadtypes.h
command.o: /usr/include/bits/initspin.h /usr/include/bits/sigthread.h
command.o: /usr/include/dirent.h /usr/include/bits/dirent.h
command.o: /usr/include/sys/stat.h /usr/include/bits/stat.h
command.o: /usr/include/SDL/SDL.h /usr/include/SDL/SDL_main.h
command.o: /usr/include/SDL/SDL_types.h /usr/include/SDL/SDL_getenv.h
command.o: /usr/include/SDL/SDL_error.h /usr/include/SDL/begin_code.h
command.o: /usr/include/SDL/close_code.h /usr/include/SDL/SDL_rwops.h
command.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
command.o: /usr/include/wchar.h /usr/include/bits/wchar.h
command.o: /usr/include/gconv.h /usr/include/bits/stdio_lim.h
command.o: /usr/include/bits/sys_errlist.h /usr/include/SDL/SDL_timer.h
command.o: /usr/include/SDL/SDL_audio.h /usr/include/SDL/SDL_byteorder.h
command.o: /usr/include/SDL/SDL_cdrom.h /usr/include/SDL/SDL_joystick.h
command.o: /usr/include/SDL/SDL_events.h /usr/include/SDL/SDL_active.h
command.o: /usr/include/SDL/SDL_keyboard.h /usr/include/SDL/SDL_keysym.h
command.o: /usr/include/SDL/SDL_mouse.h /usr/include/SDL/SDL_video.h
command.o: /usr/include/SDL/SDL_mutex.h /usr/include/SDL/SDL_quit.h
command.o: /usr/include/SDL/SDL_version.h /usr/include/SDL/SDL_ttf.h
command.o: /usr/include/SDL/SDL_opengl.h /usr/include/GL/gl.h
command.o: /usr/include/GL/glu.h /usr/include/SDL/SDL_image.h
command.o: /usr/include/math.h /usr/include/bits/huge_val.h
command.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
command.o: /usr/include/ctype.h /usr/include/endian.h
command.o: /usr/include/bits/endian.h command.h font.h
config.o: gravit.h /usr/include/stdlib.h /usr/include/features.h
config.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
config.o: /usr/include/string.h /usr/include/errno.h
config.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
config.o: /usr/include/asm/errno.h /usr/include/termios.h
config.o: /usr/include/bits/termios.h /usr/include/sys/select.h
config.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
config.o: /usr/include/bits/typesizes.h /usr/include/bits/select.h
config.o: /usr/include/bits/sigset.h /usr/include/time.h
config.o: /usr/include/bits/time.h /usr/include/sys/time.h
config.o: /usr/include/sys/types.h /usr/include/unistd.h
config.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
config.o: /usr/include/sys/timeb.h /usr/include/pthread.h
config.o: /usr/include/sched.h /usr/include/bits/sched.h
config.o: /usr/include/signal.h /usr/include/bits/pthreadtypes.h
config.o: /usr/include/bits/initspin.h /usr/include/bits/sigthread.h
config.o: /usr/include/dirent.h /usr/include/bits/dirent.h
config.o: /usr/include/sys/stat.h /usr/include/bits/stat.h
config.o: /usr/include/SDL/SDL.h /usr/include/SDL/SDL_main.h
config.o: /usr/include/SDL/SDL_types.h /usr/include/SDL/SDL_getenv.h
config.o: /usr/include/SDL/SDL_error.h /usr/include/SDL/begin_code.h
config.o: /usr/include/SDL/close_code.h /usr/include/SDL/SDL_rwops.h
config.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
config.o: /usr/include/wchar.h /usr/include/bits/wchar.h /usr/include/gconv.h
config.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
config.o: /usr/include/SDL/SDL_timer.h /usr/include/SDL/SDL_audio.h
config.o: /usr/include/SDL/SDL_byteorder.h /usr/include/SDL/SDL_cdrom.h
config.o: /usr/include/SDL/SDL_joystick.h /usr/include/SDL/SDL_events.h
config.o: /usr/include/SDL/SDL_active.h /usr/include/SDL/SDL_keyboard.h
config.o: /usr/include/SDL/SDL_keysym.h /usr/include/SDL/SDL_mouse.h
config.o: /usr/include/SDL/SDL_video.h /usr/include/SDL/SDL_mutex.h
config.o: /usr/include/SDL/SDL_quit.h /usr/include/SDL/SDL_version.h
config.o: /usr/include/SDL/SDL_ttf.h /usr/include/SDL/SDL_opengl.h
config.o: /usr/include/GL/gl.h /usr/include/GL/glu.h
config.o: /usr/include/SDL/SDL_image.h /usr/include/math.h
config.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
config.o: /usr/include/bits/mathcalls.h /usr/include/ctype.h
config.o: /usr/include/endian.h /usr/include/bits/endian.h command.h font.h
console.o: gravit.h /usr/include/stdlib.h /usr/include/features.h
console.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
console.o: /usr/include/string.h /usr/include/errno.h
console.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
console.o: /usr/include/asm/errno.h /usr/include/termios.h
console.o: /usr/include/bits/termios.h /usr/include/sys/select.h
console.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
console.o: /usr/include/bits/typesizes.h /usr/include/bits/select.h
console.o: /usr/include/bits/sigset.h /usr/include/time.h
console.o: /usr/include/bits/time.h /usr/include/sys/time.h
console.o: /usr/include/sys/types.h /usr/include/unistd.h
console.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
console.o: /usr/include/sys/timeb.h /usr/include/pthread.h
console.o: /usr/include/sched.h /usr/include/bits/sched.h
console.o: /usr/include/signal.h /usr/include/bits/pthreadtypes.h
console.o: /usr/include/bits/initspin.h /usr/include/bits/sigthread.h
console.o: /usr/include/dirent.h /usr/include/bits/dirent.h
console.o: /usr/include/sys/stat.h /usr/include/bits/stat.h
console.o: /usr/include/SDL/SDL.h /usr/include/SDL/SDL_main.h
console.o: /usr/include/SDL/SDL_types.h /usr/include/SDL/SDL_getenv.h
console.o: /usr/include/SDL/SDL_error.h /usr/include/SDL/begin_code.h
console.o: /usr/include/SDL/close_code.h /usr/include/SDL/SDL_rwops.h
console.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
console.o: /usr/include/wchar.h /usr/include/bits/wchar.h
console.o: /usr/include/gconv.h /usr/include/bits/stdio_lim.h
console.o: /usr/include/bits/sys_errlist.h /usr/include/SDL/SDL_timer.h
console.o: /usr/include/SDL/SDL_audio.h /usr/include/SDL/SDL_byteorder.h
console.o: /usr/include/SDL/SDL_cdrom.h /usr/include/SDL/SDL_joystick.h
console.o: /usr/include/SDL/SDL_events.h /usr/include/SDL/SDL_active.h
console.o: /usr/include/SDL/SDL_keyboard.h /usr/include/SDL/SDL_keysym.h
console.o: /usr/include/SDL/SDL_mouse.h /usr/include/SDL/SDL_video.h
console.o: /usr/include/SDL/SDL_mutex.h /usr/include/SDL/SDL_quit.h
console.o: /usr/include/SDL/SDL_version.h /usr/include/SDL/SDL_ttf.h
console.o: /usr/include/SDL/SDL_opengl.h /usr/include/GL/gl.h
console.o: /usr/include/GL/glu.h /usr/include/SDL/SDL_image.h
console.o: /usr/include/math.h /usr/include/bits/huge_val.h
console.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
console.o: /usr/include/ctype.h /usr/include/endian.h
console.o: /usr/include/bits/endian.h command.h font.h
font.o: gravit.h /usr/include/stdlib.h /usr/include/features.h
font.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
font.o: /usr/include/string.h /usr/include/errno.h /usr/include/bits/errno.h
font.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
font.o: /usr/include/termios.h /usr/include/bits/termios.h
font.o: /usr/include/sys/select.h /usr/include/bits/types.h
font.o: /usr/include/bits/wordsize.h /usr/include/bits/typesizes.h
font.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
font.o: /usr/include/time.h /usr/include/bits/time.h /usr/include/sys/time.h
font.o: /usr/include/sys/types.h /usr/include/unistd.h
font.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
font.o: /usr/include/sys/timeb.h /usr/include/pthread.h /usr/include/sched.h
font.o: /usr/include/bits/sched.h /usr/include/signal.h
font.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/initspin.h
font.o: /usr/include/bits/sigthread.h /usr/include/dirent.h
font.o: /usr/include/bits/dirent.h /usr/include/sys/stat.h
font.o: /usr/include/bits/stat.h /usr/include/SDL/SDL.h
font.o: /usr/include/SDL/SDL_main.h /usr/include/SDL/SDL_types.h
font.o: /usr/include/SDL/SDL_getenv.h /usr/include/SDL/SDL_error.h
font.o: /usr/include/SDL/begin_code.h /usr/include/SDL/close_code.h
font.o: /usr/include/SDL/SDL_rwops.h /usr/include/stdio.h
font.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
font.o: /usr/include/bits/wchar.h /usr/include/gconv.h
font.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
font.o: /usr/include/SDL/SDL_timer.h /usr/include/SDL/SDL_audio.h
font.o: /usr/include/SDL/SDL_byteorder.h /usr/include/SDL/SDL_cdrom.h
font.o: /usr/include/SDL/SDL_joystick.h /usr/include/SDL/SDL_events.h
font.o: /usr/include/SDL/SDL_active.h /usr/include/SDL/SDL_keyboard.h
font.o: /usr/include/SDL/SDL_keysym.h /usr/include/SDL/SDL_mouse.h
font.o: /usr/include/SDL/SDL_video.h /usr/include/SDL/SDL_mutex.h
font.o: /usr/include/SDL/SDL_quit.h /usr/include/SDL/SDL_version.h
font.o: /usr/include/SDL/SDL_ttf.h /usr/include/SDL/SDL_opengl.h
font.o: /usr/include/GL/gl.h /usr/include/GL/glu.h
font.o: /usr/include/SDL/SDL_image.h /usr/include/math.h
font.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
font.o: /usr/include/bits/mathcalls.h /usr/include/ctype.h
font.o: /usr/include/endian.h /usr/include/bits/endian.h command.h font.h
fps.o: gravit.h /usr/include/stdlib.h /usr/include/features.h
fps.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
fps.o: /usr/include/string.h /usr/include/errno.h /usr/include/bits/errno.h
fps.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
fps.o: /usr/include/termios.h /usr/include/bits/termios.h
fps.o: /usr/include/sys/select.h /usr/include/bits/types.h
fps.o: /usr/include/bits/wordsize.h /usr/include/bits/typesizes.h
fps.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
fps.o: /usr/include/time.h /usr/include/bits/time.h /usr/include/sys/time.h
fps.o: /usr/include/sys/types.h /usr/include/unistd.h
fps.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
fps.o: /usr/include/sys/timeb.h /usr/include/pthread.h /usr/include/sched.h
fps.o: /usr/include/bits/sched.h /usr/include/signal.h
fps.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/initspin.h
fps.o: /usr/include/bits/sigthread.h /usr/include/dirent.h
fps.o: /usr/include/bits/dirent.h /usr/include/sys/stat.h
fps.o: /usr/include/bits/stat.h /usr/include/SDL/SDL.h
fps.o: /usr/include/SDL/SDL_main.h /usr/include/SDL/SDL_types.h
fps.o: /usr/include/SDL/SDL_getenv.h /usr/include/SDL/SDL_error.h
fps.o: /usr/include/SDL/begin_code.h /usr/include/SDL/close_code.h
fps.o: /usr/include/SDL/SDL_rwops.h /usr/include/stdio.h /usr/include/libio.h
fps.o: /usr/include/_G_config.h /usr/include/wchar.h
fps.o: /usr/include/bits/wchar.h /usr/include/gconv.h
fps.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
fps.o: /usr/include/SDL/SDL_timer.h /usr/include/SDL/SDL_audio.h
fps.o: /usr/include/SDL/SDL_byteorder.h /usr/include/SDL/SDL_cdrom.h
fps.o: /usr/include/SDL/SDL_joystick.h /usr/include/SDL/SDL_events.h
fps.o: /usr/include/SDL/SDL_active.h /usr/include/SDL/SDL_keyboard.h
fps.o: /usr/include/SDL/SDL_keysym.h /usr/include/SDL/SDL_mouse.h
fps.o: /usr/include/SDL/SDL_video.h /usr/include/SDL/SDL_mutex.h
fps.o: /usr/include/SDL/SDL_quit.h /usr/include/SDL/SDL_version.h
fps.o: /usr/include/SDL/SDL_ttf.h /usr/include/SDL/SDL_opengl.h
fps.o: /usr/include/GL/gl.h /usr/include/GL/glu.h
fps.o: /usr/include/SDL/SDL_image.h /usr/include/math.h
fps.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
fps.o: /usr/include/bits/mathcalls.h /usr/include/ctype.h
fps.o: /usr/include/endian.h /usr/include/bits/endian.h command.h font.h
frame.o: gravit.h /usr/include/stdlib.h /usr/include/features.h
frame.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
frame.o: /usr/include/string.h /usr/include/errno.h /usr/include/bits/errno.h
frame.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
frame.o: /usr/include/termios.h /usr/include/bits/termios.h
frame.o: /usr/include/sys/select.h /usr/include/bits/types.h
frame.o: /usr/include/bits/wordsize.h /usr/include/bits/typesizes.h
frame.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
frame.o: /usr/include/time.h /usr/include/bits/time.h /usr/include/sys/time.h
frame.o: /usr/include/sys/types.h /usr/include/unistd.h
frame.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
frame.o: /usr/include/sys/timeb.h /usr/include/pthread.h /usr/include/sched.h
frame.o: /usr/include/bits/sched.h /usr/include/signal.h
frame.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/initspin.h
frame.o: /usr/include/bits/sigthread.h /usr/include/dirent.h
frame.o: /usr/include/bits/dirent.h /usr/include/sys/stat.h
frame.o: /usr/include/bits/stat.h /usr/include/SDL/SDL.h
frame.o: /usr/include/SDL/SDL_main.h /usr/include/SDL/SDL_types.h
frame.o: /usr/include/SDL/SDL_getenv.h /usr/include/SDL/SDL_error.h
frame.o: /usr/include/SDL/begin_code.h /usr/include/SDL/close_code.h
frame.o: /usr/include/SDL/SDL_rwops.h /usr/include/stdio.h
frame.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
frame.o: /usr/include/bits/wchar.h /usr/include/gconv.h
frame.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
frame.o: /usr/include/SDL/SDL_timer.h /usr/include/SDL/SDL_audio.h
frame.o: /usr/include/SDL/SDL_byteorder.h /usr/include/SDL/SDL_cdrom.h
frame.o: /usr/include/SDL/SDL_joystick.h /usr/include/SDL/SDL_events.h
frame.o: /usr/include/SDL/SDL_active.h /usr/include/SDL/SDL_keyboard.h
frame.o: /usr/include/SDL/SDL_keysym.h /usr/include/SDL/SDL_mouse.h
frame.o: /usr/include/SDL/SDL_video.h /usr/include/SDL/SDL_mutex.h
frame.o: /usr/include/SDL/SDL_quit.h /usr/include/SDL/SDL_version.h
frame.o: /usr/include/SDL/SDL_ttf.h /usr/include/SDL/SDL_opengl.h
frame.o: /usr/include/GL/gl.h /usr/include/GL/glu.h
frame.o: /usr/include/SDL/SDL_image.h /usr/include/math.h
frame.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
frame.o: /usr/include/bits/mathcalls.h /usr/include/ctype.h
frame.o: /usr/include/endian.h /usr/include/bits/endian.h command.h font.h
frame-ot.o: gravit.h /usr/include/stdlib.h /usr/include/features.h
frame-ot.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
frame-ot.o: /usr/include/string.h /usr/include/errno.h
frame-ot.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
frame-ot.o: /usr/include/asm/errno.h /usr/include/termios.h
frame-ot.o: /usr/include/bits/termios.h /usr/include/sys/select.h
frame-ot.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
frame-ot.o: /usr/include/bits/typesizes.h /usr/include/bits/select.h
frame-ot.o: /usr/include/bits/sigset.h /usr/include/time.h
frame-ot.o: /usr/include/bits/time.h /usr/include/sys/time.h
frame-ot.o: /usr/include/sys/types.h /usr/include/unistd.h
frame-ot.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
frame-ot.o: /usr/include/sys/timeb.h /usr/include/pthread.h
frame-ot.o: /usr/include/sched.h /usr/include/bits/sched.h
frame-ot.o: /usr/include/signal.h /usr/include/bits/pthreadtypes.h
frame-ot.o: /usr/include/bits/initspin.h /usr/include/bits/sigthread.h
frame-ot.o: /usr/include/dirent.h /usr/include/bits/dirent.h
frame-ot.o: /usr/include/sys/stat.h /usr/include/bits/stat.h
frame-ot.o: /usr/include/SDL/SDL.h /usr/include/SDL/SDL_main.h
frame-ot.o: /usr/include/SDL/SDL_types.h /usr/include/SDL/SDL_getenv.h
frame-ot.o: /usr/include/SDL/SDL_error.h /usr/include/SDL/begin_code.h
frame-ot.o: /usr/include/SDL/close_code.h /usr/include/SDL/SDL_rwops.h
frame-ot.o: /usr/include/stdio.h /usr/include/libio.h
frame-ot.o: /usr/include/_G_config.h /usr/include/wchar.h
frame-ot.o: /usr/include/bits/wchar.h /usr/include/gconv.h
frame-ot.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
frame-ot.o: /usr/include/SDL/SDL_timer.h /usr/include/SDL/SDL_audio.h
frame-ot.o: /usr/include/SDL/SDL_byteorder.h /usr/include/SDL/SDL_cdrom.h
frame-ot.o: /usr/include/SDL/SDL_joystick.h /usr/include/SDL/SDL_events.h
frame-ot.o: /usr/include/SDL/SDL_active.h /usr/include/SDL/SDL_keyboard.h
frame-ot.o: /usr/include/SDL/SDL_keysym.h /usr/include/SDL/SDL_mouse.h
frame-ot.o: /usr/include/SDL/SDL_video.h /usr/include/SDL/SDL_mutex.h
frame-ot.o: /usr/include/SDL/SDL_quit.h /usr/include/SDL/SDL_version.h
frame-ot.o: /usr/include/SDL/SDL_ttf.h /usr/include/SDL/SDL_opengl.h
frame-ot.o: /usr/include/GL/gl.h /usr/include/GL/glu.h
frame-ot.o: /usr/include/SDL/SDL_image.h /usr/include/math.h
frame-ot.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
frame-ot.o: /usr/include/bits/mathcalls.h /usr/include/ctype.h
frame-ot.o: /usr/include/endian.h /usr/include/bits/endian.h command.h font.h
frame-pp.o: gravit.h /usr/include/stdlib.h /usr/include/features.h
frame-pp.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
frame-pp.o: /usr/include/string.h /usr/include/errno.h
frame-pp.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
frame-pp.o: /usr/include/asm/errno.h /usr/include/termios.h
frame-pp.o: /usr/include/bits/termios.h /usr/include/sys/select.h
frame-pp.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
frame-pp.o: /usr/include/bits/typesizes.h /usr/include/bits/select.h
frame-pp.o: /usr/include/bits/sigset.h /usr/include/time.h
frame-pp.o: /usr/include/bits/time.h /usr/include/sys/time.h
frame-pp.o: /usr/include/sys/types.h /usr/include/unistd.h
frame-pp.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
frame-pp.o: /usr/include/sys/timeb.h /usr/include/pthread.h
frame-pp.o: /usr/include/sched.h /usr/include/bits/sched.h
frame-pp.o: /usr/include/signal.h /usr/include/bits/pthreadtypes.h
frame-pp.o: /usr/include/bits/initspin.h /usr/include/bits/sigthread.h
frame-pp.o: /usr/include/dirent.h /usr/include/bits/dirent.h
frame-pp.o: /usr/include/sys/stat.h /usr/include/bits/stat.h
frame-pp.o: /usr/include/SDL/SDL.h /usr/include/SDL/SDL_main.h
frame-pp.o: /usr/include/SDL/SDL_types.h /usr/include/SDL/SDL_getenv.h
frame-pp.o: /usr/include/SDL/SDL_error.h /usr/include/SDL/begin_code.h
frame-pp.o: /usr/include/SDL/close_code.h /usr/include/SDL/SDL_rwops.h
frame-pp.o: /usr/include/stdio.h /usr/include/libio.h
frame-pp.o: /usr/include/_G_config.h /usr/include/wchar.h
frame-pp.o: /usr/include/bits/wchar.h /usr/include/gconv.h
frame-pp.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
frame-pp.o: /usr/include/SDL/SDL_timer.h /usr/include/SDL/SDL_audio.h
frame-pp.o: /usr/include/SDL/SDL_byteorder.h /usr/include/SDL/SDL_cdrom.h
frame-pp.o: /usr/include/SDL/SDL_joystick.h /usr/include/SDL/SDL_events.h
frame-pp.o: /usr/include/SDL/SDL_active.h /usr/include/SDL/SDL_keyboard.h
frame-pp.o: /usr/include/SDL/SDL_keysym.h /usr/include/SDL/SDL_mouse.h
frame-pp.o: /usr/include/SDL/SDL_video.h /usr/include/SDL/SDL_mutex.h
frame-pp.o: /usr/include/SDL/SDL_quit.h /usr/include/SDL/SDL_version.h
frame-pp.o: /usr/include/SDL/SDL_ttf.h /usr/include/SDL/SDL_opengl.h
frame-pp.o: /usr/include/GL/gl.h /usr/include/GL/glu.h
frame-pp.o: /usr/include/SDL/SDL_image.h /usr/include/math.h
frame-pp.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
frame-pp.o: /usr/include/bits/mathcalls.h /usr/include/ctype.h
frame-pp.o: /usr/include/endian.h /usr/include/bits/endian.h command.h font.h
gfx.o: /usr/include/dlfcn.h /usr/include/features.h /usr/include/sys/cdefs.h
gfx.o: /usr/include/gnu/stubs.h /usr/include/bits/dlfcn.h gravit.h
gfx.o: /usr/include/stdlib.h /usr/include/string.h /usr/include/errno.h
gfx.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
gfx.o: /usr/include/asm/errno.h /usr/include/termios.h
gfx.o: /usr/include/bits/termios.h /usr/include/sys/select.h
gfx.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
gfx.o: /usr/include/bits/typesizes.h /usr/include/bits/select.h
gfx.o: /usr/include/bits/sigset.h /usr/include/time.h
gfx.o: /usr/include/bits/time.h /usr/include/sys/time.h
gfx.o: /usr/include/sys/types.h /usr/include/unistd.h
gfx.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
gfx.o: /usr/include/sys/timeb.h /usr/include/pthread.h /usr/include/sched.h
gfx.o: /usr/include/bits/sched.h /usr/include/signal.h
gfx.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/initspin.h
gfx.o: /usr/include/bits/sigthread.h /usr/include/dirent.h
gfx.o: /usr/include/bits/dirent.h /usr/include/sys/stat.h
gfx.o: /usr/include/bits/stat.h /usr/include/SDL/SDL.h
gfx.o: /usr/include/SDL/SDL_main.h /usr/include/SDL/SDL_types.h
gfx.o: /usr/include/SDL/SDL_getenv.h /usr/include/SDL/SDL_error.h
gfx.o: /usr/include/SDL/begin_code.h /usr/include/SDL/close_code.h
gfx.o: /usr/include/SDL/SDL_rwops.h /usr/include/stdio.h /usr/include/libio.h
gfx.o: /usr/include/_G_config.h /usr/include/wchar.h
gfx.o: /usr/include/bits/wchar.h /usr/include/gconv.h
gfx.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
gfx.o: /usr/include/SDL/SDL_timer.h /usr/include/SDL/SDL_audio.h
gfx.o: /usr/include/SDL/SDL_byteorder.h /usr/include/SDL/SDL_cdrom.h
gfx.o: /usr/include/SDL/SDL_joystick.h /usr/include/SDL/SDL_events.h
gfx.o: /usr/include/SDL/SDL_active.h /usr/include/SDL/SDL_keyboard.h
gfx.o: /usr/include/SDL/SDL_keysym.h /usr/include/SDL/SDL_mouse.h
gfx.o: /usr/include/SDL/SDL_video.h /usr/include/SDL/SDL_mutex.h
gfx.o: /usr/include/SDL/SDL_quit.h /usr/include/SDL/SDL_version.h
gfx.o: /usr/include/SDL/SDL_ttf.h /usr/include/SDL/SDL_opengl.h
gfx.o: /usr/include/GL/gl.h /usr/include/GL/glu.h
gfx.o: /usr/include/SDL/SDL_image.h /usr/include/math.h
gfx.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
gfx.o: /usr/include/bits/mathcalls.h /usr/include/ctype.h
gfx.o: /usr/include/endian.h /usr/include/bits/endian.h command.h font.h
input.o: gravit.h /usr/include/stdlib.h /usr/include/features.h
input.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
input.o: /usr/include/string.h /usr/include/errno.h /usr/include/bits/errno.h
input.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
input.o: /usr/include/termios.h /usr/include/bits/termios.h
input.o: /usr/include/sys/select.h /usr/include/bits/types.h
input.o: /usr/include/bits/wordsize.h /usr/include/bits/typesizes.h
input.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
input.o: /usr/include/time.h /usr/include/bits/time.h /usr/include/sys/time.h
input.o: /usr/include/sys/types.h /usr/include/unistd.h
input.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
input.o: /usr/include/sys/timeb.h /usr/include/pthread.h /usr/include/sched.h
input.o: /usr/include/bits/sched.h /usr/include/signal.h
input.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/initspin.h
input.o: /usr/include/bits/sigthread.h /usr/include/dirent.h
input.o: /usr/include/bits/dirent.h /usr/include/sys/stat.h
input.o: /usr/include/bits/stat.h /usr/include/SDL/SDL.h
input.o: /usr/include/SDL/SDL_main.h /usr/include/SDL/SDL_types.h
input.o: /usr/include/SDL/SDL_getenv.h /usr/include/SDL/SDL_error.h
input.o: /usr/include/SDL/begin_code.h /usr/include/SDL/close_code.h
input.o: /usr/include/SDL/SDL_rwops.h /usr/include/stdio.h
input.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
input.o: /usr/include/bits/wchar.h /usr/include/gconv.h
input.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
input.o: /usr/include/SDL/SDL_timer.h /usr/include/SDL/SDL_audio.h
input.o: /usr/include/SDL/SDL_byteorder.h /usr/include/SDL/SDL_cdrom.h
input.o: /usr/include/SDL/SDL_joystick.h /usr/include/SDL/SDL_events.h
input.o: /usr/include/SDL/SDL_active.h /usr/include/SDL/SDL_keyboard.h
input.o: /usr/include/SDL/SDL_keysym.h /usr/include/SDL/SDL_mouse.h
input.o: /usr/include/SDL/SDL_video.h /usr/include/SDL/SDL_mutex.h
input.o: /usr/include/SDL/SDL_quit.h /usr/include/SDL/SDL_version.h
input.o: /usr/include/SDL/SDL_ttf.h /usr/include/SDL/SDL_opengl.h
input.o: /usr/include/GL/gl.h /usr/include/GL/glu.h
input.o: /usr/include/SDL/SDL_image.h /usr/include/math.h
input.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
input.o: /usr/include/bits/mathcalls.h /usr/include/ctype.h
input.o: /usr/include/endian.h /usr/include/bits/endian.h command.h font.h
main.o: gravit.h /usr/include/stdlib.h /usr/include/features.h
main.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
main.o: /usr/include/string.h /usr/include/errno.h /usr/include/bits/errno.h
main.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
main.o: /usr/include/termios.h /usr/include/bits/termios.h
main.o: /usr/include/sys/select.h /usr/include/bits/types.h
main.o: /usr/include/bits/wordsize.h /usr/include/bits/typesizes.h
main.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
main.o: /usr/include/time.h /usr/include/bits/time.h /usr/include/sys/time.h
main.o: /usr/include/sys/types.h /usr/include/unistd.h
main.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
main.o: /usr/include/sys/timeb.h /usr/include/pthread.h /usr/include/sched.h
main.o: /usr/include/bits/sched.h /usr/include/signal.h
main.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/initspin.h
main.o: /usr/include/bits/sigthread.h /usr/include/dirent.h
main.o: /usr/include/bits/dirent.h /usr/include/sys/stat.h
main.o: /usr/include/bits/stat.h /usr/include/SDL/SDL.h
main.o: /usr/include/SDL/SDL_main.h /usr/include/SDL/SDL_types.h
main.o: /usr/include/SDL/SDL_getenv.h /usr/include/SDL/SDL_error.h
main.o: /usr/include/SDL/begin_code.h /usr/include/SDL/close_code.h
main.o: /usr/include/SDL/SDL_rwops.h /usr/include/stdio.h
main.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
main.o: /usr/include/bits/wchar.h /usr/include/gconv.h
main.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
main.o: /usr/include/SDL/SDL_timer.h /usr/include/SDL/SDL_audio.h
main.o: /usr/include/SDL/SDL_byteorder.h /usr/include/SDL/SDL_cdrom.h
main.o: /usr/include/SDL/SDL_joystick.h /usr/include/SDL/SDL_events.h
main.o: /usr/include/SDL/SDL_active.h /usr/include/SDL/SDL_keyboard.h
main.o: /usr/include/SDL/SDL_keysym.h /usr/include/SDL/SDL_mouse.h
main.o: /usr/include/SDL/SDL_video.h /usr/include/SDL/SDL_mutex.h
main.o: /usr/include/SDL/SDL_quit.h /usr/include/SDL/SDL_version.h
main.o: /usr/include/SDL/SDL_ttf.h /usr/include/SDL/SDL_opengl.h
main.o: /usr/include/GL/gl.h /usr/include/GL/glu.h
main.o: /usr/include/SDL/SDL_image.h /usr/include/math.h
main.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
main.o: /usr/include/bits/mathcalls.h /usr/include/ctype.h
main.o: /usr/include/endian.h /usr/include/bits/endian.h command.h font.h
osd.o: gravit.h /usr/include/stdlib.h /usr/include/features.h
osd.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
osd.o: /usr/include/string.h /usr/include/errno.h /usr/include/bits/errno.h
osd.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
osd.o: /usr/include/termios.h /usr/include/bits/termios.h
osd.o: /usr/include/sys/select.h /usr/include/bits/types.h
osd.o: /usr/include/bits/wordsize.h /usr/include/bits/typesizes.h
osd.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
osd.o: /usr/include/time.h /usr/include/bits/time.h /usr/include/sys/time.h
osd.o: /usr/include/sys/types.h /usr/include/unistd.h
osd.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
osd.o: /usr/include/sys/timeb.h /usr/include/pthread.h /usr/include/sched.h
osd.o: /usr/include/bits/sched.h /usr/include/signal.h
osd.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/initspin.h
osd.o: /usr/include/bits/sigthread.h /usr/include/dirent.h
osd.o: /usr/include/bits/dirent.h /usr/include/sys/stat.h
osd.o: /usr/include/bits/stat.h /usr/include/SDL/SDL.h
osd.o: /usr/include/SDL/SDL_main.h /usr/include/SDL/SDL_types.h
osd.o: /usr/include/SDL/SDL_getenv.h /usr/include/SDL/SDL_error.h
osd.o: /usr/include/SDL/begin_code.h /usr/include/SDL/close_code.h
osd.o: /usr/include/SDL/SDL_rwops.h /usr/include/stdio.h /usr/include/libio.h
osd.o: /usr/include/_G_config.h /usr/include/wchar.h
osd.o: /usr/include/bits/wchar.h /usr/include/gconv.h
osd.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
osd.o: /usr/include/SDL/SDL_timer.h /usr/include/SDL/SDL_audio.h
osd.o: /usr/include/SDL/SDL_byteorder.h /usr/include/SDL/SDL_cdrom.h
osd.o: /usr/include/SDL/SDL_joystick.h /usr/include/SDL/SDL_events.h
osd.o: /usr/include/SDL/SDL_active.h /usr/include/SDL/SDL_keyboard.h
osd.o: /usr/include/SDL/SDL_keysym.h /usr/include/SDL/SDL_mouse.h
osd.o: /usr/include/SDL/SDL_video.h /usr/include/SDL/SDL_mutex.h
osd.o: /usr/include/SDL/SDL_quit.h /usr/include/SDL/SDL_version.h
osd.o: /usr/include/SDL/SDL_ttf.h /usr/include/SDL/SDL_opengl.h
osd.o: /usr/include/GL/gl.h /usr/include/GL/glu.h
osd.o: /usr/include/SDL/SDL_image.h /usr/include/math.h
osd.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
osd.o: /usr/include/bits/mathcalls.h /usr/include/ctype.h
osd.o: /usr/include/endian.h /usr/include/bits/endian.h command.h font.h
spawn.o: gravit.h /usr/include/stdlib.h /usr/include/features.h
spawn.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
spawn.o: /usr/include/string.h /usr/include/errno.h /usr/include/bits/errno.h
spawn.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
spawn.o: /usr/include/termios.h /usr/include/bits/termios.h
spawn.o: /usr/include/sys/select.h /usr/include/bits/types.h
spawn.o: /usr/include/bits/wordsize.h /usr/include/bits/typesizes.h
spawn.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
spawn.o: /usr/include/time.h /usr/include/bits/time.h /usr/include/sys/time.h
spawn.o: /usr/include/sys/types.h /usr/include/unistd.h
spawn.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
spawn.o: /usr/include/sys/timeb.h /usr/include/pthread.h /usr/include/sched.h
spawn.o: /usr/include/bits/sched.h /usr/include/signal.h
spawn.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/initspin.h
spawn.o: /usr/include/bits/sigthread.h /usr/include/dirent.h
spawn.o: /usr/include/bits/dirent.h /usr/include/sys/stat.h
spawn.o: /usr/include/bits/stat.h /usr/include/SDL/SDL.h
spawn.o: /usr/include/SDL/SDL_main.h /usr/include/SDL/SDL_types.h
spawn.o: /usr/include/SDL/SDL_getenv.h /usr/include/SDL/SDL_error.h
spawn.o: /usr/include/SDL/begin_code.h /usr/include/SDL/close_code.h
spawn.o: /usr/include/SDL/SDL_rwops.h /usr/include/stdio.h
spawn.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
spawn.o: /usr/include/bits/wchar.h /usr/include/gconv.h
spawn.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
spawn.o: /usr/include/SDL/SDL_timer.h /usr/include/SDL/SDL_audio.h
spawn.o: /usr/include/SDL/SDL_byteorder.h /usr/include/SDL/SDL_cdrom.h
spawn.o: /usr/include/SDL/SDL_joystick.h /usr/include/SDL/SDL_events.h
spawn.o: /usr/include/SDL/SDL_active.h /usr/include/SDL/SDL_keyboard.h
spawn.o: /usr/include/SDL/SDL_keysym.h /usr/include/SDL/SDL_mouse.h
spawn.o: /usr/include/SDL/SDL_video.h /usr/include/SDL/SDL_mutex.h
spawn.o: /usr/include/SDL/SDL_quit.h /usr/include/SDL/SDL_version.h
spawn.o: /usr/include/SDL/SDL_ttf.h /usr/include/SDL/SDL_opengl.h
spawn.o: /usr/include/GL/gl.h /usr/include/GL/glu.h
spawn.o: /usr/include/SDL/SDL_image.h /usr/include/math.h
spawn.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
spawn.o: /usr/include/bits/mathcalls.h /usr/include/ctype.h
spawn.o: /usr/include/endian.h /usr/include/bits/endian.h command.h font.h
texture.o: gravit.h /usr/include/stdlib.h /usr/include/features.h
texture.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
texture.o: /usr/include/string.h /usr/include/errno.h
texture.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
texture.o: /usr/include/asm/errno.h /usr/include/termios.h
texture.o: /usr/include/bits/termios.h /usr/include/sys/select.h
texture.o: /usr/include/bits/types.h /usr/include/bits/wordsize.h
texture.o: /usr/include/bits/typesizes.h /usr/include/bits/select.h
texture.o: /usr/include/bits/sigset.h /usr/include/time.h
texture.o: /usr/include/bits/time.h /usr/include/sys/time.h
texture.o: /usr/include/sys/types.h /usr/include/unistd.h
texture.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
texture.o: /usr/include/sys/timeb.h /usr/include/pthread.h
texture.o: /usr/include/sched.h /usr/include/bits/sched.h
texture.o: /usr/include/signal.h /usr/include/bits/pthreadtypes.h
texture.o: /usr/include/bits/initspin.h /usr/include/bits/sigthread.h
texture.o: /usr/include/dirent.h /usr/include/bits/dirent.h
texture.o: /usr/include/sys/stat.h /usr/include/bits/stat.h
texture.o: /usr/include/SDL/SDL.h /usr/include/SDL/SDL_main.h
texture.o: /usr/include/SDL/SDL_types.h /usr/include/SDL/SDL_getenv.h
texture.o: /usr/include/SDL/SDL_error.h /usr/include/SDL/begin_code.h
texture.o: /usr/include/SDL/close_code.h /usr/include/SDL/SDL_rwops.h
texture.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
texture.o: /usr/include/wchar.h /usr/include/bits/wchar.h
texture.o: /usr/include/gconv.h /usr/include/bits/stdio_lim.h
texture.o: /usr/include/bits/sys_errlist.h /usr/include/SDL/SDL_timer.h
texture.o: /usr/include/SDL/SDL_audio.h /usr/include/SDL/SDL_byteorder.h
texture.o: /usr/include/SDL/SDL_cdrom.h /usr/include/SDL/SDL_joystick.h
texture.o: /usr/include/SDL/SDL_events.h /usr/include/SDL/SDL_active.h
texture.o: /usr/include/SDL/SDL_keyboard.h /usr/include/SDL/SDL_keysym.h
texture.o: /usr/include/SDL/SDL_mouse.h /usr/include/SDL/SDL_video.h
texture.o: /usr/include/SDL/SDL_mutex.h /usr/include/SDL/SDL_quit.h
texture.o: /usr/include/SDL/SDL_version.h /usr/include/SDL/SDL_ttf.h
texture.o: /usr/include/SDL/SDL_opengl.h /usr/include/GL/gl.h
texture.o: /usr/include/GL/glu.h /usr/include/SDL/SDL_image.h
texture.o: /usr/include/math.h /usr/include/bits/huge_val.h
texture.o: /usr/include/bits/mathdef.h /usr/include/bits/mathcalls.h
texture.o: /usr/include/ctype.h /usr/include/endian.h
texture.o: /usr/include/bits/endian.h command.h font.h
tool.o: gravit.h /usr/include/stdlib.h /usr/include/features.h
tool.o: /usr/include/sys/cdefs.h /usr/include/gnu/stubs.h
tool.o: /usr/include/string.h /usr/include/errno.h /usr/include/bits/errno.h
tool.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
tool.o: /usr/include/termios.h /usr/include/bits/termios.h
tool.o: /usr/include/sys/select.h /usr/include/bits/types.h
tool.o: /usr/include/bits/wordsize.h /usr/include/bits/typesizes.h
tool.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
tool.o: /usr/include/time.h /usr/include/bits/time.h /usr/include/sys/time.h
tool.o: /usr/include/sys/types.h /usr/include/unistd.h
tool.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
tool.o: /usr/include/sys/timeb.h /usr/include/pthread.h /usr/include/sched.h
tool.o: /usr/include/bits/sched.h /usr/include/signal.h
tool.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/initspin.h
tool.o: /usr/include/bits/sigthread.h /usr/include/dirent.h
tool.o: /usr/include/bits/dirent.h /usr/include/sys/stat.h
tool.o: /usr/include/bits/stat.h /usr/include/SDL/SDL.h
tool.o: /usr/include/SDL/SDL_main.h /usr/include/SDL/SDL_types.h
tool.o: /usr/include/SDL/SDL_getenv.h /usr/include/SDL/SDL_error.h
tool.o: /usr/include/SDL/begin_code.h /usr/include/SDL/close_code.h
tool.o: /usr/include/SDL/SDL_rwops.h /usr/include/stdio.h
tool.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
tool.o: /usr/include/bits/wchar.h /usr/include/gconv.h
tool.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
tool.o: /usr/include/SDL/SDL_timer.h /usr/include/SDL/SDL_audio.h
tool.o: /usr/include/SDL/SDL_byteorder.h /usr/include/SDL/SDL_cdrom.h
tool.o: /usr/include/SDL/SDL_joystick.h /usr/include/SDL/SDL_events.h
tool.o: /usr/include/SDL/SDL_active.h /usr/include/SDL/SDL_keyboard.h
tool.o: /usr/include/SDL/SDL_keysym.h /usr/include/SDL/SDL_mouse.h
tool.o: /usr/include/SDL/SDL_video.h /usr/include/SDL/SDL_mutex.h
tool.o: /usr/include/SDL/SDL_quit.h /usr/include/SDL/SDL_version.h
tool.o: /usr/include/SDL/SDL_ttf.h /usr/include/SDL/SDL_opengl.h
tool.o: /usr/include/GL/gl.h /usr/include/GL/glu.h
tool.o: /usr/include/SDL/SDL_image.h /usr/include/math.h
tool.o: /usr/include/bits/huge_val.h /usr/include/bits/mathdef.h
tool.o: /usr/include/bits/mathcalls.h /usr/include/ctype.h
tool.o: /usr/include/endian.h /usr/include/bits/endian.h command.h font.h
