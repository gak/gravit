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
video_t video;
#endif

state_t state;
view_t view;

void loadDefaults() {

	state.memoryAvailable = 128;

#ifndef NO_GUI

	video.sdlStarted = 0;
	video.screenWtoApply = 800;
	video.screenHtoApply = 600;
	video.screenBPP = 32;
	video.screenFS = 0;
	video.screenAA = 0;

	strcpy(video.fontFile, "Vera.ttf");
	video.fontSize = 11;
	
#endif

	viewInit();
	spawnDefaults();
	stateInit();
	commandInit();

}

void cleanMemory() {

	if (state.particleHistory) {

		_aligned_free(state.particleHistory);
		state.particleHistory = 0;

	}

	if (state.particleDetail) {

		_aligned_free(state.particleDetail);
		state.particleDetail = 0;

	}

	state.memoryAllocated = 0;

}

void viewInit() {

#ifndef NO_GUI

	view.rot[0] = view.rot[1] = view.rot[2] = 0;
	view.zoom = 10000;
	view.zoomFitAuto = 1;
	view.textMode = view.textMode;
	memset(view.pos, 0, sizeof(view.pos));
	memset(view.face, 0, sizeof(view.face));
	view.pos[2] -= 30;

	VectorZero(view.autoRotate);

	view.tailWidth = 1.0f;
	view.tailLength = 32;
	view.tailOpacity = 0.5f;
	view.tailFaded = 1;
	view.tailSkip = 1;

	view.screenSaver = 0;

	// 50ms by default (20fps)
	view.recordingVideoRefreshTime = 50;

	view.drawAxis = 1;
	view.consoleMode = 0;

	view.mouseButtons[0] = view.mouseButtons[1] = 0;

	view.lastMousePosition[0] = view.currentMousePosition[0] = video.screenW / 2;
	view.lastMousePosition[1] = view.currentMousePosition[1] = video.screenH / 2;
	view.showCursor = 1;

	view.fps = 100;
	view.drawTree = 0;
	view.frameSkip = 0;
	view.frameSkipCounter = 0;

	view.drawOSD = 1;
	view.drawColourScheme = 1;

	view.blendMode = 1;

	view.particleColourMode = CM_MASS;
	view.particleRenderMode = 2;
	view.particleRenderTexture = 1;

	view.particleSizeMin = 4;
	view.particleSizeMax = 1024;

	view.verboseMode = 0;

	view.screenshotLoop = 0;
	view.screenshotIndex = 0;

	view.colourSpectrumSteps = 0;
	view.colourSpectrum = 0;

	view.stereoMode = 0;
	view.stereoModeCurrentBit = 0;
	// http://www.angelfire.com/ca/erker/freeview.html
	view.stereoSeparation = 500; // negative for "Parallel", positive for "Cross-eyed"

	cmdFps(NULL);

	memset(view.keys, 0, sizeof(view.keys));

	view.recordStatus = 0;
	view.recordParticlesDone = 0;
	view.recordNodes = 0;

	strncpy(view.popupTextMessage, "Welcome to " GRAVIT_VERSION, 255);
	view.popupTextStart = 0;
	view.popupTextLength = 6000;
	view.popupTextFadeTime = 2000;	// ms

	view.autoCenter = 1;

	view.maxVertices = 100000;

	#endif


}

void stateInit() {

	state.processFrameThreads = 1;
	state.particlesToSpawn = 1000;
	state.particleHistory = 0;
	state.memoryAllocated = 0;
	state.currentlySpawning = 0;
	state.restartSpawning = 0;
	state.fileName = 0;
	state.dontExecuteDefaultScript = 0;
	state.autoSave = 0;
	state.lastSave = 0;
	state.autoRecord = 0;
	state.autoRecordNext = 0;

	state.gbase = 5;
	state.g = -0.00001f;

}

int init(int argc, char *argv[]) {

#if WIN32
	char currentDirectory[MAX_PATH];
#endif

	srand(time(0));
	
	conInit();
	loadDefaults();
	timerInit();

	if (!commandLineRead(argc, argv))
		return 0;

	if (view.quit)
		return 0;

	if (!state.dontExecuteDefaultScript)
		configRead(findFile(CONFIG_FILE), 0);

#ifndef NO_GUI

	if (!gfxInit())
		return 1;

#endif

	if (state.historyFrames % 2)
		state.historyFrames--;

	fpsInit();

#ifdef WIN32
	// if we've gone this far, lets set the registry key even if it exists...
	GetCurrentDirectory(MAX_PATH, currentDirectory);
	setRegistryString(REGISTRY_NAME_PATH, currentDirectory);
#endif

	conAdd(LNORM, "Welcome to Gravit!");

#ifndef NO_GUI

	conAdd(LHELP, "Quick Start: Hit F8 a few times then F6 to start recording.");
	conAdd(LHELP, "Hold down a mouse button and move it around. Use A and Z keys to zoom in and out.");

#endif

	return 0;

}

void clean() {

	cleanMemory();
	freeFileName();
	cmdFree();
	conFree();

#ifndef NO_GUI

	colourSpectrumClear();
	timerFree();

	if (video.sdlStarted) {
		TTF_Quit();
		SDL_Quit();
	}

#endif

}

void runInput() {

#ifndef NO_GUI
	if (!video.sdlStarted)
		return;
#endif

	processKeys();

#ifndef NO_GUI
	processMouse();
#endif

}

void runVideo() {

#ifndef NO_GUI

	Uint32 ts;

	// runVideo might be called before SDL starts, say from a startup script
	if (!video.sdlStarted)
		return;

	ts = getMS();
	drawAll();

	view.deltaVideoFrame = ts - view.lastVideoFrame;
	view.lastVideoFrame = ts;
	fpsUpdate((float)view.deltaVideoFrame);

#endif

}

void run() {

	view.firstTimeStamp = view.lastRecordFrame = view.lastVideoFrame = getMS();

	while (!view.quit) {

		Uint32 ts;

		if (state.mode & SM_RECORD) {

			view.frameSkipCounter = 0;

			if (view.verboseMode)
				conAdd(LLOW, "R frame:%5i dt:%5i fs:%2i", state.totalFrames, view.deltaVideoFrame, state.historyNFrame);

			setTitle(va("%s frame:%i/%i (skip:%i)", STRING_RECORD, state.totalFrames, state.historyFrames, state.historyNFrame));

			processFrame();
			ts = getMS();
			view.deltaRecordFrame = ts - view.lastRecordFrame;
			view.lastRecordFrame = ts;

			if (state.autoSave && (state.totalFrames - state.lastSave) >= state.autoSave) {
				cmdSaveFrameDump(0);
				state.lastSave = state.totalFrames;
			}

		}

		else if (state.mode & SM_PLAY) {

			if (view.frameSkip < 0) {
				view.frameSkipCounter++;
				if (view.frameSkipCounter > -view.frameSkip) {
					view.frameSkipCounter = 0;
					state.currentFrame++;
				}
			} else {
				state.currentFrame++;
				state.currentFrame+=view.frameSkip;
			}

			if (state.currentFrame >= state.frame) {
				state.currentFrame = 0;
				view.frameSkipCounter = 0;
			}

			if (view.verboseMode)
				conAdd(LLOW, "P frame:%5i dt:%5i fs:%2i", state.currentFrame, view.deltaVideoFrame, state.historyNFrame);

		}

		timerUpdate();

		runInput();
		if (view.quit) return;

		if (state.autoRecordNext) {
			state.autoRecordNext = 0;
			cmdRecord(0);
		}

		setColours();
		runVideo();

	}

}

int main(int argc, char *argv[]) {

	if (init(argc, argv)) {

		conAdd(LERR, "There has been an error on start-up. Read your gravit configration file to possibly fix this.");

	} else {

		run();

	}

	clean();

	return 0;

}

#define ShowHelp(p,v) conAdd(LLOW, "  %-25s%s", p, v);
void usage() {

	conAdd(LLOW, GRAVIT_VERSION);
	conAdd(LLOW, GRAVIT_COPYRIGHT);
	conAdd(LLOW, "");
	conAdd(LLOW, "usage: gravit [-nvh] [COMMAND] ...");
	conAdd(LLOW, "");
//	ShowHelp("-e, --exec=COMMAND",	"execute a command. eg. --exec=\"load foo\"")
//	ShowHelp("",					"  commands will execute in order from left to right.")
	ShowHelp("-n, --noscript",		"don't load gravit configuration")
	ShowHelp("-h, --help",			"you're looking at it")
	ShowHelp("-v, --version",		"display version and quit")
	conAdd(LLOW, "");
	conAdd(LLOW, "  'COMMAND' should be encapsulated in quotes when there are spaces involved.");
	conAdd(LLOW, "  For example: gravit spawn record \"save mysim\" \"saveauto 20\"");
	conAdd(LLOW, "");
	cmdQuit(0);

}

int commandLineRead(int argc, char *argv[]) {

	int i;
//	char *parm;
//	char *argvptr;

#define CheckCommand(x) !strncmp(argv[i], x, strlen(x))

	for (i = 1; i < argc; i++) {

		// version, quit
		if (CheckCommand("--version") || CheckCommand("-v")) {
			conAdd(LLOW, GRAVIT_VERSION);
			conAdd(LLOW, GRAVIT_COPYRIGHT);
			cmdQuit(0);
			return 0;
		}

		// --help or -h
		if (CheckCommand("--help") || CheckCommand("-h")) {

			usage();
			return 0;

		}

		// -n or --noscript
		if (CheckCommand("--noscript") || CheckCommand("-n")) {
			state.dontExecuteDefaultScript = 1;			
			continue;
		}

#ifdef WIN32SCREENSAVER

		if (CheckCommand("/S") || CheckCommand("/s")/* || CheckCommand("/P") || CheckCommand("/p")*/) {

			char *path;
			path = getRegistryString(REGISTRY_NAME_PATH);
			if (!path || strlen(path) == 0) {
				MessageBox(NULL, "Can't work out where Gravit lives!\nTry running Gravit by itself first", "Gravit Screensaver Error", MB_OK);
				cmdQuit(0);
				return 0;
			}
			SetCurrentDirectory(path);

			configRead(findFile(SCREENSAVER_FILE), 0);
			state.dontExecuteDefaultScript = 1;
			view.screenSaver = 1;

			continue;
		}

		// just ignore preview mode, configure dialog and change password
		if (CheckCommand("/P") || CheckCommand("/p") || CheckCommand("/C") || CheckCommand("/c") || CheckCommand("/A") || CheckCommand("/a")) {
			cmdQuit(0);
			return 0;
		}

#endif

		cmdExecute(argv[i]);

	}

	return 1;

}

#ifdef WIN32SCREENSAVER

LRESULT WINAPI ScreenSaverProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	return TRUE;

}


BOOL WINAPI ScreenSaverConfigureDialog(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam) {

	return TRUE;

}


BOOL WINAPI RegisterDialogClasses(HANDLE hInst)
{
  return TRUE;
}

#endif

