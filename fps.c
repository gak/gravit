/*

Graviton - A gravity simulator
Copyright 2003-2005 Gerald Kaszuba

Graviton is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

Graviton is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Graviton; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/


#include "graviton.h"

#define FPS_SAMPLES 10

int fpsFirstPass;
int fpsPointer;

float fpsFrameTimes[FPS_SAMPLES];
float fpsCurrentAverageFPS;
float fpsCurrentAverageFT;

void fpsInit() {

	fpsFirstPass = 1;
	fpsPointer = 0;
	fpsCurrentAverageFPS = 0;
	fpsCurrentAverageFT = 0;
	memset(fpsFrameTimes, 0, sizeof(fpsFrameTimes));

}

void fpsUpdate(float frameTime) {

	int samples;
	int i;

	fpsFrameTimes[fpsPointer] = frameTime;
	fpsPointer++;

	if (fpsPointer == FPS_SAMPLES) {
		fpsPointer = 0;
		fpsFirstPass = 0;
	}

	if (fpsFirstPass) {

		samples = fpsPointer;

	} else {

		samples = FPS_SAMPLES;

	}

	fpsCurrentAverageFT = 0;
	for (i = 0; i < samples; i++) {

		fpsCurrentAverageFT += fpsFrameTimes[i];

	}

	fpsCurrentAverageFT /= samples;
	fpsCurrentAverageFPS = 1000.f / fpsCurrentAverageFT;

}

