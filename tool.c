/*

Gravit - A gravity simulator
Copyright 2003-2005 Gerald Kaszuba

Gravit is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

Gravit is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Gravit; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

#include "gravit.h"

#define shitzta 4

char * va( char *format, ... ) {

	va_list argptr;
	static char string[shitzta][10000];    // in case va is called by nested functions
	static unsigned short index = 0;
	char *buf;

	buf = string[index];
	index++;

	if (index == shitzta)
			index = 0;

	va_start (argptr, format);
	vsprintf (buf, format, argptr);
	va_end (argptr);

	return buf;
}

int gfxPowerOfTwo(int input) {

	int value = 1;

	while ( value < input )
		value <<= 1;

	return value;

}

int LoadMemoryDump(char *fileName, unsigned char *d, unsigned int size) {

	FILE *fp;
	unsigned int i, pos, p, amountToRead;
	unsigned int chunkMax = 1024*1024;

	fp = fopen(fileName, "rb");
	if (!fp) {

		conAdd(1, "Count not open %s for reading.", fileName);
		return 0;

	}

	i = 0;
	pos = 0;

	while (pos < size) {

		if (size - pos < chunkMax)
			amountToRead = size - pos;
		else
			amountToRead = chunkMax;

		p = fread(d, 1, amountToRead, fp);

		if (p == 0) {
			conAdd(2, "Short read on %s", fileName);
			fclose(fp);
			return 0;
		}
		

		d += p;
		pos += p;

		if (ferror(fp)) {

			conAdd(1, "%s", strerror( errno ));
			fclose(fp);
			return 0;

		}
#if 0
		if (i) {
			conAdd(0, "%3.1f%% (%.0f / %.0f)", 100*(float)pos/(float)size,(float)pos / 1024 / 1024,(float)size / 1024 / 1024);
#ifndef NO_GUI
			drawAll();
#endif
			i=1000;
		}

		i--;
#endif
		
	}

	fclose(fp);

	conAdd(0, "Loaded %i bytes from %s", size, fileName);

	return 1;
}

int SaveMemoryDump(char *fileName, unsigned char *d, unsigned int total) {

	FILE *fp;
	unsigned int written, p, write;

	fp = fopen(fileName, "wb");
	if (!fp) {

		conAdd(1, "count not open %s for writing");
		return 0;

	}

	written = 0;

	while (written < total) {

		write = 1024*1024;

		if (written + write > total)
			write = total - written;

		p = fwrite(d, 1, write, fp);
		d += p;
		written += p;

	}

	fclose(fp);

	conAdd(0, "written %u bytes to %s", written, fileName);

	return 1;

}

Uint32 getMS() {

#ifndef NO_GUI
	return SDL_GetTicks();
#else

	#ifdef WIN32
		return GetTickCount();
	#else

		struct timeb mytime;
		ftime(&mytime);
		return mytime.time * 1000 + mytime.millitm;

	#endif

#endif

}

void setTitle(char *state) {

	char *a;

	if (state)
		a = va("%s - %s", GRAVIT_VERSION, state);
	else
		a = GRAVIT_VERSION;

	SDL_WM_SetCaption(a, a);	

}

int mymkdir(char *path) {

#ifdef WIN32
	{
		WIN32_FIND_DATA damnwindows;
		if (FindFirstFile(path, &damnwindows) == INVALID_HANDLE_VALUE) {
			CreateDirectory(path, NULL);
		}
	}
#else
	{

		DIR *d;
		d = opendir(path);
		if (!d) {
			mkdir(path, 0755);
		} else {
			closedir(d);
		}

	}
#endif

	return 1;
	
}