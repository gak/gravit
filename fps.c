
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

