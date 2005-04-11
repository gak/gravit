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
	video.screenW = 800;
	video.screenH = 600;
	video.screenBPP = 32;
	video.screenFS = 0;
	video.screenAA = 0;

	strcpy(video.fontFile, "Vera.ttf");
	video.fontSize = 11;
	
#endif

	viewInit();
	spawnDefaults();
	stateInit();

}

void cleanMemory() {

	if (state.particleHistory) {

//		conAdd(0, "Freeing state.particleHistory");
		_aligned_free(state.particleHistory);

	}

	if (state.particleDetail) {

//		conAdd(0, "Freeing state.particleDetail");
		_aligned_free(state.particleDetail);

	}

	state.memoryAllocated = 0;


}

void viewInit() {

#ifndef NO_GUI

	view.rot[0] = view.rot[1] = view.rot[2] = 0;
	view.zoom = 10000;
	view.textMode = view.textMode;
	memset(view.pos, 0, sizeof(view.pos));
	memset(view.face, 0, sizeof(view.face));
	view.pos[2] -= 30;

	VectorZero(view.autoRotate);

	view.tailWidth = 1.0f;
	view.tailLength = 32;
	view.tailOpacity = 0.5f;
	view.tailFaded = 1;
	view.tailSkip = 4;

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

	view.drawOSD = 1;
	view.drawColourScheme = 1;

	view.blendMode = 1;

	view.particleColourMode = CM_MASS;
	view.particleRenderMode = 1;
	view.particleRenderTexture = 1;

	view.particleSizeMin = 4;
	view.particleSizeMax = 1024;

	view.verboseMode = 0;

	view.screenshotLoop = 0;
	view.screenshotIndex = 0;

	view.colourSpectrumSteps = 0;
	view.colourSpectrum = 0;

	view.stereoMode = 0;
	
	// http://www.angelfire.com/ca/erker/freeview.html
	view.stereoSeparation = 500; // negative for "Parallel", positive for "Cross-eyed"
	
	cmdFps(NULL);

	memset(view.keys, 0, sizeof(view.keys));

	view.recordStatus = 0;
	view.recordParticlesDone = 0;
	view.recordNodes = 0;

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
	state.autoRecord;

	state.gbase = 5;
	// cmdSetG should do: state.g = -0.00001f; because of state.gbase = 5;
	cmdSetG(0);

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
		configRead("gravit.cfg");

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

	conAdd(1, "Welcome to Gravit!");

#ifndef NO_GUI

	conAdd(0, "Quick Start: Hit F8 a few times then F6 to start recording.");
	conAdd(0, "Hold down a mouse button and move it around. Use A and Z keys to zoom in and out.");

#endif

	return 0;

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

			if (view.verboseMode)
				conAdd(0, "R frame:%5i dt:%5i fs:%2i", state.totalFrames, view.deltaVideoFrame, state.historyNFrame);

			setTitle(va("%s frame:%i/%i (skip:%i)", STRING_RECORD, state.totalFrames, state.historyFrames, state.historyNFrame));

			processFrame();
			ts = getMS();
			view.deltaRecordFrame = ts - view.lastRecordFrame;
			view.lastRecordFrame = ts;

			if (state.autoSave && (state.frame - state.lastSave) >= state.autoSave) {
				cmdSaveFrameDump(0);
				state.lastSave = state.frame;
			}

		}

		else if (state.mode & SM_PLAY) {

			state.currentFrame++;
			state.currentFrame+=view.frameSkip;

			if (state.currentFrame >= state.frame)
				state.currentFrame = 0;

			if (view.verboseMode)
				conAdd(0, "P frame:%5i dt:%5i fs:%2i", state.currentFrame, view.deltaVideoFrame, state.historyNFrame);

		}

		timerUpdate();

		runInput();
		if (view.quit) return;

		if (state.autoRecordNext) {
			state.autoRecordNext = 0;
			cmdRecord(0);
		}

		runVideo();
	

	}

}

int main(int argc, char *argv[]) {

	if (init(argc, argv)) {

		conAdd(2, "There has been an error on start-up. Read gravit.cfg to possibly fix this.");

	} else {

		run();

	}

	cleanMemory();
	if (state.fileName) free(state.fileName);

#ifndef NO_GUI

	colourSpectrumClear();
	timerFree();

	if (video.sdlStarted) {
		TTF_Quit();
		SDL_Quit();
	}

#endif

	return 0;

}

#define ShowHelp(p,v) conAdd(0, "  %-25s%s", p, v);
void usage() {

	conAdd(0, GRAVIT_VERSION);
	conAdd(0, GRAVIT_COPYRIGHT);
	conAdd(0, "");
	conAdd(0, "usage: gravit [-nvh] [COMMAND] ...");
	conAdd(0, "");
//	ShowHelp("-e, --exec=COMMAND",	"execute a command. eg. --exec=\"load foo\"")
//	ShowHelp("",					"  commands will execute in order from left to right.")
	ShowHelp("-n, --noscript",		"don't load gravit.cfg")
	ShowHelp("-h, --help",			"you're looking at it")
	ShowHelp("-v, --version",		"display version and quit")
	conAdd(0, "");
	conAdd(0, "  'COMMAND' should be encapsulated in quotes when there are spaces involved.");
	conAdd(0, "  For example: gravit \"drawosd 0\" spawn record \"saveauto 20\"");
	conAdd(0, "");
	cmdQuit(0);

}

int commandLineRead(int argc, char *argv[]) {

	int i;
//	char *parm;
//	char *argvptr;

#define CheckCommand(x) !strncmp(argv[i], x, strlen(x))

/*

	GetValue() is good for:

	--exec value
	--exec=value

*/

#define GetValue() \
	if (strlen(parm) == strlen(argv[i])) \
		argvptr = argv[++i]; \
	else \
		argvptr = argv[i] + strlen(parm); \
	while (argvptr[0] == ' ' || argvptr[0] == '=' || argvptr[0] == 0) argvptr++;

/*

	example of checkCommandAndGetValue(x)

	CheckCommandAndGetValue("--execute")
		cmdExecute(argvptr);
	}

	because it's like this when it expands:

	parm = "--execute";
	if (CheckCommand(parm)) {
		GetValue();
		cmdExecute(argvptr);
	}

*/

#define CheckCommandAndGetValue(x) parm = x; if (CheckCommand(parm)) { GetValue();

	for (i = 1; i < argc; i++) {

		// version, quit
		if (CheckCommand("--version") || CheckCommand("-v")) {
			conAdd(0, GRAVIT_VERSION);
			conAdd(0, GRAVIT_COPYRIGHT);
			cmdQuit(0);
			return 0;
		}

		// --help or -h
		if (CheckCommand("--help") || CheckCommand("-h")) {

			usage();
			return 0;

		}

		// -e or --exec
		// runs a command
		if (CheckCommand("--exec") || CheckCommand("-e")) {
			conAdd(0, "Sorry, --exec has been removed -- just add your commands to the command line as they are");
			conAdd(0, "  eg. gravit exec \"\" ");
			cmdQuit(0);
			return 0;
			continue;
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

			configRead("screensaver.cfg");
			state.dontExecuteDefaultScript = 1;
			view.screenSaver = 1;

			continue;
		}

		// just ignore preview mode, configure dialog and change password
		if (CheckCommand("/P") || CheckCommand("/p") || CheckCommand("/CP") || CheckCommand("/c") || CheckCommand("/A") || CheckCommand("/a")) {
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

