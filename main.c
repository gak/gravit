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

#ifndef NO_GUI
conf_t conf;
#endif

state_t state;
view_t view;

void loadDefaults() {

	strcpy(conf.fontFile, "Vera.ttf");
	conf.fontSize = 11;
	conf.screenAA = 0;
	conf.memoryAvailable = 128;

#ifndef NO_GUI

	conf.screenBPP = 32;
	conf.screenW = 800;
	conf.screenH = 600;
	conf.screenFS = 0;

#endif

}

void cleanMemory() {

	if (state.particleHistory) {

		conAdd(0, "Freeing state.particleHistory");
		_aligned_free(state.particleHistory);

	}

	if (state.particleDetail) {

		conAdd(0, "Freeing state.particleDetail");
		_aligned_free(state.particleDetail);

	}

	state.memoryAllocated = 0;


}

void viewInit() {

	view.rot[0] = view.rot[1] = view.rot[2] = 0;
	view.zoom = 10000;
	view.textMode = view.textMode;
	memset(view.pos, 0, sizeof(view.pos));
	memset(view.face, 0, sizeof(view.face));
	view.pos[2] -= 30;

	view.tailWidth = 1.0f;
	view.tailLength = 32;
	view.tailOpacity = 0.5f;
	view.tailFaded = 1;
	view.tailSkip = 4;

	// 50ms by default (20fps)
	view.recordingVideoRefreshTime = 50;

	view.drawAxis = 1;
	view.consoleMode = 0;

	view.fps = 100;
	view.drawTree = 0;
	view.frameSkip = 0;

	view.drawOSD = 1;

	view.blendMode = 1;

	view.particleColourMode = CM_MASS;
	view.particleRenderMode = 0;
	view.particleRenderTexture = 1;

	view.particleSizeMin = 4;
	view.particleSizeMax = 1024;

	view.verboseMode = 0;

	view.screenshotLoop = 0;
	view.screenshotIndex = 0;

	view.colourSpectrumSteps = 0;
	view.colourSpectrum = 0;

	cmdFps(NULL);

	#ifndef NO_GUI
	memset(view.keys, 0, sizeof(view.keys));
	#endif

}

void stateInit() {

	state.processFrameThreads = 1;
	state.particlesToSpawn = 1000;
	state.particleHistory = 0;
	state.memoryAllocated = 0;
	state.currentlySpawning = 0;
	state.restartSpawning = 0;

}

int init() {

	srand(time(0));
	
	conInit();
	loadDefaults();
	viewInit();
	spawnDefaults();
	stateInit();
	
	configRead("gravit.cfg");

#ifndef NO_GUI

	if (!gfxInit())
		return 1;

#endif

	if (state.historyFrames % 2)
		state.historyFrames--;


	fpsInit();

	conAdd(1, "Welcome to Gravit!");

#ifndef NO_GUI

	conAdd(0, "Quick Start: Hit F8 a few times then F6 to start recording.");
	conAdd(0, "Use the mouse to move around. Use A and Z keys to zoom in and out.");

#endif

	return 0;

}

void runInput() {

	processKeys();

#ifndef NO_GUI

	processMouse();

#endif

}

void runVideo() {

#ifndef NO_GUI

	Uint32 ts;

	ts = getMS();

	drawAll();

	view.deltaVideoFrame = ts - view.lastVideoFrame;
	view.lastVideoFrame = ts;
	fpsUpdate((float)view.deltaVideoFrame);

#endif

}

void run() {

	view.lastRecordFrame = view.lastVideoFrame = getMS();

	while (!view.quit) {

		Uint32 ts;

		if (state.mode & SM_RECORD) {

			if (view.verboseMode)
				conAdd(0, "R frame:%5i dt:%5i fs:%2i", state.totalFrames, view.deltaVideoFrame, state.historyNFrame);

			setTitle(va("%s frame:%i/%i (skip:%i)", STRING_RECORD, state.totalFrames, state.historyFrames, state.historyNFrame));

			processFrame();
			ts = getMS();
			view.deltaRecordFrame = ts - view.lastRecordFrame;
			view.lastRecordFrame = ts;

		}

		else if (state.mode & SM_PLAY) {

			state.currentFrame++;
			state.currentFrame+=view.frameSkip;

			if (state.currentFrame >= state.frame)
				state.currentFrame = 0;

			if (view.verboseMode)
				conAdd(0, "P frame:%5i dt:%5i fs:%2i", state.currentFrame, view.deltaVideoFrame, state.historyNFrame);

		}

		runInput();
		if (view.quit) return;

		runVideo();

	}

}

int main(int argc, char *argv[]) {

	if (init()) {

		conAdd(2, "There has been an error on start-up. Read gravit.cfg to possibly fix this.");

	} else {

		run();

	}

	cleanMemory();
	colourSpectrumClear();

#ifndef NO_GUI

	TTF_Quit();
	SDL_Quit();

#endif

	return 0;

}
/*

LRESULT WINAPI ScreenSaverProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch ( msg ) {

	case WM_CREATE:

		init();
		run();
		break;

	case WM_DESTROY:

		view.quit = 1;
		break;


	}

	return DefScreenSaverProc(hWnd, msg, wParam, lParam );

}


BOOL WINAPI ScreenSaverConfigureDialog(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) {

	return TRUE;

}


BOOL WINAPI RegisterDialogClasses(HANDLE hInst)
{
  return TRUE;
}

*/

