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

#ifndef NO_GUI
conf_t conf;
#endif

state_t state;
view_t view;

void loadDefaults() {

#ifndef NO_GUI

	conf.screenBPP = 32;
	conf.screenW = 640;
	conf.screenH = 480;
	conf.screenFS = 0;

#if 1
	conf.screenW = 1280;
	conf.screenH = 1024;
	conf.screenFS = 1;
#endif

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
	view.zoom = 1000;
	view.textMode = view.textMode;
	memset(view.pos, 0, sizeof(view.pos));
	memset(view.face, 0, sizeof(view.face));
	view.pos[2] -= 30;

	view.tailWidth = 1.0f;
	view.tailLength = 0;
	view.tailOpacity = 0.5f;
	view.tailFaded = 1;
	view.tailSkip = 4;

	view.drawAxis = 1;
	view.consoleMode = 0;

	view.fps = 100;
	view.drawTree = 0;
	view.frameSkip = 0;

	view.particleSize = 2;
	view.particleColorMode = CM_MASS;

	view.verboseMode = 0;

	cmdFps(NULL);
	
	#ifndef NO_GUI
	memset(view.keys, 0, sizeof(view.keys));
	#endif

}

void stateInit() {

	state.processFrameThreads = 1;
	state.particlesToSpawn = 1000;

}

int init() {

	srand(time(0));

	state.memoryAllocated = 0;
	loadDefaults();

#ifndef NO_GUI

	if (!gfxInit())
		return 1;

#endif

	viewInit();

	spawnDefaults();

	conInit();

	stateInit();

	if (state.historyFrames % 2)
		state.historyFrames--;

	state.particleHistory = 0;

	fpsInit();

	conAdd(0, "Graviton version 0.1, Copyright (C) 2003-2005 Gerald Kaszuba");
	conAdd(0, "Graviton comes with ABSOLUTELY NO WARRANTY");
	conAdd(0, "This is free software, and you are welcome to redistribute it");
	conAdd(0, "under certain conditions; for details read the COPYING text file.");

	return 0;

}

void run() {

	Uint32 fs;

	fs = getMS();

	while (!view.quit) {

		view.dt = 0;

#ifndef NO_GUI

		while (view.dt <= view.ft) {

			view.dt = getMS() - fs;
			
			if (view.dt > 1000)
				SDL_Delay(0);
			else
				SDL_Delay(view.dt);

		}
		fs = getMS();
		fpsUpdate((float)view.dt);

		if (processKeys())
			break;

		processMouse();

#else

	view.dt = getMS() - fs;
	fs = getMS();

	if (processKeys())
		break;

#endif

		if (!(state.mode & SM_PAUSED)) {

			if (state.mode & SM_RECORD) {

				if (view.verboseMode)
					conAdd(0, "R frame:%5i dt:%5i fs:%2i", state.totalFrames, view.dt, state.historyNFrame);

				processFrame();

			}

			else if (state.mode & SM_PLAY) {

				state.currentFrame++;
				state.currentFrame+=view.frameSkip;

				if (state.currentFrame >= state.frame)
					state.currentFrame = 0;

				if (view.verboseMode)
					conAdd(0, "P frame:%5i dt:%5i fs:%2i", state.currentFrame, view.dt, state.historyNFrame);

			}

		}

#ifndef NO_GUI

		drawAll();

#endif

	}

}

int main(int argc, char *argv[]) {

	init();

	run();

	cleanMemory();

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
 
