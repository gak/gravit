#include "graviton.h"

#define shitzta 2

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

void gfxNormalToRGB(float *c, float n) {

	int bits;
	int i;

	float cm[][4] = {

		{0.5f,	0.5f,	0.5f,	0.1f}
		,{0.0f,	0.0f,	0.5f,	0.3f}
		,{0.5f,	0.0f,	0.5f,	0.5f}
		,{0.9f,	0.0f,	0.0f,	0.8f}
		,{0.9f,	1.0f,	0.5f,	0.9f}
		,{1.0f,	1.0f,	1.0f,	1.0f}

	};

	bits = 5;

	if (n < 0)
		n = 0;
	if (n > 1)
		n = 1;

	for (i = 0; i < bits; i++) {

		if (n <= (float)(i+1) / bits) {

			float j;
			float l,h;
			int col;

			j = (n - (float)i / bits) * (float)bits;

			for (col = 0; col < 4; col++) {

				if (cm[i][col] < cm[i+1][col]) {
					l = cm[i][col];
					h = cm[i+1][col];
					c[col] = (h - l) * j + l;
				} else {
					l = cm[i+1][col];
					h = cm[i][col];
					c[col] = (h - l) * (1-j) + l;
				}

			}

			break;

		}

	}

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

	while (!feof(fp)) {

		if (pos >= size)
			break;

		if (size - pos < chunkMax)
			amountToRead = size - pos;
		else
			amountToRead = chunkMax;

		p = fread(d, 1, amountToRead, fp);
		d += p;
		pos += p;

		if (ferror(fp)) {

			conAdd(1, "%s", strerror( errno ));
			fclose(fp);
			return 0;

		}

		if (i) {
			conAdd(0, "%3.1f%% (%.0f / %.0f)", 100*(float)pos/(float)size,(float)pos / 1024 / 1024,(float)size / 1024 / 1024);
#ifndef NO_GUI
			drawAll();
#endif
			i=1000;
		}

		i--;

	}

	fclose(fp);

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

	conAdd(0, "written %u/%u bytes", written, total);

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
