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
	unsigned int chunkMax = FILE_CHUNK_SIZE;

	fp = fopen(fileName, "rb");
	if (!fp) {

		conAdd(LNORM, "Count not open %s for reading.", fileName);
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
			conAdd(LERR, "Short read on %s", fileName);
			fclose(fp);
			return 0;
		}
		

		d += p;
		pos += p;

		if (ferror(fp)) {

			conAdd(LNORM, "%s", strerror( errno ));
			fclose(fp);
			return 0;

		}
#if 0
		if (i) {
			conAdd(LLOW, "%3.1f%% (%.0f / %.0f)", 100*(float)pos/(float)size,(float)pos / 1024 / 1024,(float)size / 1024 / 1024);
#ifndef NO_GUI
			drawAll();
#endif
			i=1000;
		}

		i--;
#endif
		
	}

	fclose(fp);

	return 1;
}

int SaveMemoryDump(char *fileName, unsigned char *d, unsigned int total) {

	FILE *fp;
	unsigned int written, p, write;

	fp = fopen(fileName, "wb");
	if (!fp) {

		conAdd(LNORM, "count not open %s for writing");
		return 0;

	}

	written = 0;

	while (written < total) {

		write = FILE_CHUNK_SIZE;

		if (written + write > total)
			write = total - written;

		p = fwrite(d, 1, write, fp);
		d += p;
		written += p;

	}

	fclose(fp);

	conAdd(LLOW, "written %u bytes to %s", written, fileName);

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

#ifndef NO_GUI

	char *a;

	if (state)
		a = va("%s - %s", GRAVIT_VERSION, state);
	else
		a = GRAVIT_VERSION;

	SDL_WM_SetCaption(a, a);	

#endif

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

void freeFileName() {

	if (state.fileName) {
		free(state.fileName);
		state.fileName = 0;
	}

}

void setFileName(char *name) {

	char buf[255];

	strncpy(buf, name, 255);

	freeFileName();

	state.fileName = malloc(strlen(buf)+1);	// +1 for \0
	strcpy(state.fileName, buf);

}

char *getRegistryString(char *variable) {
#ifdef WIN32

	static char buf[MAX_PATH];
	int len = MAX_PATH;

	HKEY hkResult;
	RegCreateKeyEx(HKEY_CURRENT_USER, REGISTRY_KEY, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkResult, NULL);
	RegQueryValueEx(hkResult, variable, 0, NULL, buf, &len);
	RegCloseKey(hkResult);
	return buf;

#endif
	return NULL;
}

void setRegistryString(char *variable, char *value) {
#ifdef WIN32

	HKEY hkResult;
	RegCreateKeyEx(HKEY_CURRENT_USER, REGISTRY_KEY, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkResult, NULL);
	RegSetValueEx(hkResult, variable, 0, REG_SZ, value, strlen(value)+1);
	RegCloseKey(hkResult);

#endif
}

// return 0 on failure
int myunlink(char *filename) {

#ifdef WIN32
	return DeleteFile(filename);
#else
	return !unlink(filename);
#endif

}
