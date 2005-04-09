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

static cmd_t cmd[] = {

	//	   cmd				func			varf,	vari
	{ "quit",			cmdQuit,			NULL,	NULL }

	,{ "exec",			cmdRunScript,		NULL,	NULL }

	,{ "memoryavailable",		NULL,		NULL,	&state.memoryAvailable }

#ifndef NO_GUI
	,{ "videowidth",		NULL,			NULL,	&conf.screenW }
	,{ "videoheight",		NULL,			NULL,	&conf.screenH }
	,{ "videobpp",			NULL,			NULL,	&conf.screenBPP }
	,{ "videofullscreen",	NULL,			NULL,	&conf.screenFS }
	,{ "videoantialiasing",	NULL,			NULL,	&conf.screenAA }

	,{ "fontfile",			cmdFontFile,	NULL,	NULL }
	,{ "fontsize",			NULL,			NULL,	&conf.fontSize }

#endif

	,{ "recordingvideorefreshtime",	NULL,	NULL,	&view.recordingVideoRefreshTime }

	,{ "start",			cmdStart,			NULL,	NULL }
	,{ "record",		cmdRecord,			NULL,	NULL }
	,{ "play",			cmdPlay,			NULL,	NULL }
	,{ "stop",			cmdStop,			NULL,	NULL }
	,{ "spawn",			cmdSpawn,			NULL,	NULL }
	,{ "status",		cmdStatus,			NULL,	NULL }

	,{ "screensaver",		NULL,			NULL,	&view.screenSaver }
	,{ "installscreensaver",		cmdInstallScreenSaver,			NULL,	NULL }

	,{ "cd",			cmdChangeDir,		NULL,	NULL }

	,{ "showcursor",		NULL,			NULL,	&view.showCursor }
	
	,{ "g",				cmdSetG,			NULL,	&state.gbase }

	,{ "blendmode",		NULL,				NULL,	&view.blendMode }

	,{ "screenshot",		cmdScreenshot,		NULL,	NULL }
	,{ "screenshotloop",	NULL,			NULL,	&view.screenshotLoop}

	,{ "load",			cmdLoadFrameDump,		NULL,	NULL }
	,{ "save",			cmdSaveFrameDump,		NULL,	NULL }

//	,{ "fps",			cmdFps,				&view.fps,	NULL }
	,{ "frameskip",		NULL,				NULL, &view.frameSkip }
	,{ "frame",			NULL,				NULL, &state.currentFrame }

	,{ "tailskip",		cmdTailSkipCheck,	NULL, &view.tailSkip }
	,{ "tailfaded",		NULL,				NULL, &view.tailFaded }
	,{ "tailopacity",	NULL,				&view.tailOpacity, NULL }
	,{ "taillength",	NULL,				NULL, &view.tailLength }
	,{ "tailwidth",		NULL,				&view.tailWidth, NULL }

	,{ "drawaxis",		NULL,				NULL, &view.drawAxis }
	,{ "drawtree",		NULL,				NULL, &view.drawTree }
	,{ "drawosd",		NULL,				NULL, &view.drawOSD }
	,{ "drawcolourscheme",		NULL,				NULL, &view.drawColourScheme }

	,{ "particlecount",			NULL,				NULL, &state.particlesToSpawn }

	,{ "particlerendermode",	NULL,				NULL, &view.particleRenderMode }
	,{ "particlerendertexture",	NULL,				NULL, &view.particleRenderTexture }
	,{ "particlecolourmode",	NULL,				NULL, &view.particleColourMode }
	,{ "particlesizemin",			NULL,			&view.particleSizeMin, NULL }
	,{ "particlesizemax",			NULL,			&view.particleSizeMax, NULL }

	,{ "spawngalcountmin",		NULL,				NULL, &spawnVars.minGalCount }
	,{ "spawngalcountmax",		NULL,				NULL, &spawnVars.maxGalCount }
	,{ "spawngalmassmin",		NULL,				&spawnVars.minGalMass, NULL }
	,{ "spawngalmassmax",		NULL,				&spawnVars.maxGalMass, NULL }
	,{ "spawngalsizemin",		NULL,				&spawnVars.minGalSize, NULL }
	,{ "spawngalsizemax",		NULL,				&spawnVars.maxGalSize, NULL }
	,{ "spawngalvelmin",		NULL,				&spawnVars.minGalVel, NULL }
	,{ "spawngalvelmax",		NULL,				&spawnVars.maxGalVel, NULL }
	,{ "spawnrangemin",			NULL,				&spawnVars.minSpawnRange, NULL }
	,{ "spawnrangemax",			NULL,				&spawnVars.maxSpawnRange, NULL }

	,{ "colourschemenew",		cmdColourSchemeNew,			NULL, NULL }
	,{ "colourschemeadd",		cmdColourSchemeAdd,			NULL, NULL }

	,{ "framecompression",		NULL,				NULL, &state.frameCompression }

	,{ "verbose",				NULL,				NULL, &view.verboseMode }

	,{ "processors",			NULL,				NULL, &state.processFrameThreads }

	,{ "autorotate",			cmdAutoRotate,			NULL,	NULL }

	,{ NULL,			NULL,				NULL }


};

void cmdPrint(cmd_t *c) {

	if (c->fVar)
		conAdd(1, "%s = %f", c->cmd, *c->fVar);
	else if (c->iVar)
		conAdd(1, "%s = %i", c->cmd, *c->iVar);
	else
		conAdd(1, "%s", c->cmd);

}

int cmdFind(char *string) {

	cmd_t *c;
	int lenString;
	int lenCommand;
	int i;

	lenString = strlen(string);
	i = -1;

	while (1) {

		i++;
		c = &cmd[i];

		if (!c->cmd)
			return -1;

		lenCommand = strlen(c->cmd);

		if (lenString > lenCommand) {
			continue;
		}

		if (!strncmp(string, c->cmd, lenCommand)) {
			return i;
		}

	}

}

int cmdPrintStartingWith(char *string) {

	cmd_t *c;
	int lenString;
	int lenCommand;
	int i;

	lenString = strlen(string);
	i = -1;

	while (1) {

		i++;
		c = &cmd[i];

		if (!c->cmd)
			return -1;

		lenCommand = strlen(c->cmd);

		if (lenString > lenCommand) {
			continue;
		}

		if (!strncmp(string, c->cmd, lenString)) {
			cmdPrint(c);
		}

	}

}

void cmdExecute(char *string) {

    int j;
	cmd_t *c;
	char cmdbuf[CONSOLE_LENGTH];
	int stringLength;
	char *args;
	int ivar;
	float fvar;

	if (!string)
		return;

	stringLength = strlen(string);

	if (!stringLength)
		return;

	strcpy(cmdbuf, string);

	strtok(cmdbuf, " ");
	args = strtok(NULL, "");

	j = cmdFind(cmdbuf);

	if (j == -1) {
		conAdd(1, "%s: Command not found", string);
		cmdPrintStartingWith(cmdbuf);
		return;
	}

	c = &cmd[j];

	if (c->fVar) {

		if (args) {

			fvar = (float)atof(args);
			memcpy(c->fVar, &fvar, sizeof(float));

		}

		cmdPrint(c);

	} else if (c->iVar) {

		if (args) {

			ivar = atoi(args);
			memcpy(c->iVar, &ivar, sizeof(int));

		}

		cmdPrint(c);

	}

	if (c->func)
		c->func(args);


}

void cmdQuit(char *arg) {

	view.quit = 1;
	state.mode = 0;

}

void cmdStop(char *arg) {

	if (isSpawning())
		return;

	conAdd(1, "Stopped...");
	state.mode &= ~SM_PLAY;
	state.mode &= ~SM_RECORD;

	setTitle(0);

}

void cmdSpawn(char *arg) {

	if (state.currentlySpawning) {
		state.restartSpawning = 1;
		return;
	}

cmdSpawnRestartSpawning:

	state.currentlySpawning = 1;
	state.restartSpawning = 0;
	state.mode = 0;

	state.particleCount = state.particlesToSpawn;
	state.historyFrames = (int)((float)(state.memoryAvailable * 1024 * 1024) / FRAMESIZE);
	
	if (!initFrame()) {
		conAdd(1, "Could not init frame");
		return;
	}

	pickPositions();

	if (state.restartSpawning)
		goto cmdSpawnRestartSpawning;

#ifndef NO_GUI
	setColours();
#endif

	state.currentlySpawning = 0;
	conAdd(0, "You have spawned some particles. Hit F6 to start recording the simulation!");

	state.mode = 0;
	
}

void cmdStart(char *arg) {

	cmdSpawn(NULL);

}

void cmdRecord(char *arg) {

	if (isSpawning())
		return;

	if (state.particleCount == 0) {

		conAdd(1, "You need to spawn some particles first! Press F8 to spawn some!");
		return;

	}

	state.mode &= ~SM_PLAY;

	if (state.mode & SM_RECORD) {

		conAdd(1, "Stopped Recording.");
		conAdd(0, "Press F5 to play your recording. Press F6 to continue recording.");
		state.mode &= ~SM_RECORD;
		setTitle(0);

	} else {

		conAdd(1, "Recording...");
		conAdd(0, "Press F5 to play your recording. Press F6 to stop recording.");
		state.mode |= SM_RECORD;
		setTitle(STRING_RECORD);

	}

}

void cmdPlay(char *arg) {

	if (isSpawning())
		return;

	if (state.particleCount == 0) {

		conAdd(1, "You need to spawn some particles first! Press F8 to spawn some!");
		return;

	}

	state.mode &= ~SM_RECORD;

	if (state.mode & SM_PLAY) {

		conAdd(1, "Stopped Playback.");
		conAdd(0, "Press F5 to continue playback. Press F6 to continue recording.");
		state.mode &= ~SM_PLAY;
		setTitle(0);

	} else {

		conAdd(1, "Playing...");
		conAdd(0, "Press F5 to stop playback. Press F6 to continue recording.");
		state.mode |= SM_PLAY;
		setTitle(STRING_PLAY);

	}

}
#if 0
void cmdSaveFrame(char *arg) {

    char *fileName, *s2, *s3, *s4;
	int staf, endf, skif;

	if (isSpawning())
		return;

	fileName = strtok(arg, " ");
	s2 = strtok(NULL, " ");
	s3 = strtok(NULL, " ");
	s4 = strtok(NULL, " ");

	if (!fileName || !s2 || !fileName[0] || !s2[0]) {

		cmdSaveFrameUsage();
		return;

	}

	if (!strcmp(s2, "all")) {

		staf = 0;
		endf = state.frame;

		if (s3 && s3[0]) {
			skif = atoi(s3);
		} else {
			skif = 0;
		}

	} else {

		if (!s2 || !s3 || !s2[0] || !s3[0]) {

			cmdSaveFrameUsage();
			return;

		}

		staf = atoi(s2);
		endf = atoi(s2);

		if (s4 && s4[0]) {
			skif = atoi(s4);
		} else {
			skif = 0;
		}

	}

	conAdd(2, "saving frames %i to %i skipping %i to %s", staf, endf, skif, fileName);

}
#endif

void cmdSaveFrameDump(char *arg) {

	saveInfo_t si;
	char *fileName;

	if (isSpawning())
		return;

	if (!arg) {

		if (!state.fileName) {

			conAdd(2, "Please specify a name (not extensions necessary).");
			conAdd(1, "usage: save [name]");
			return;

		}

		arg = state.fileName;

	}
	
	if (!mymkdir(SAVE_PATH)) {
		conAdd(2, "Could not create %s directory", SAVE_PATH);
		return;
	}

	si.particleCount = state.particleCount;
	si.historyFrames = state.historyFrames;
	si.totalFrames = state.totalFrames;
	si.frame = state.frame;
	si.historyNFrame = state.historyNFrame;

	conAdd(1, "Saving %s...", arg);
	conAdd(1, "Please Wait...");
	runVideo();

	fileName = va(SAVE_PATH "/%s.info", arg);
	if (!SaveMemoryDump(fileName, (unsigned char *)&si, sizeof(si))) {
		conAdd(2, "Failed to create %s", fileName);
		return;
	}

	fileName = va(SAVE_PATH "/%s.particledetail", arg);
	if (!SaveMemoryDump(fileName, (unsigned char *)state.particleDetail, FRAMEDETAILSIZE)) {
		conAdd(2, "Failed to create %s", fileName);
		return;
	}

	if (!SaveMemoryDump(va(SAVE_PATH "/%s.particles", arg), (unsigned char *)state.particleHistory, FRAMESIZE * (state.frame+1))) {
		conAdd(2, "Failed to create %s", fileName);
		return;
	}
	conAdd(1, "Simulation saved sucesfully!");

	setFileName(arg);

}

void cmdLoadFrameDump(char *arg) {

	saveInfo_t si;
	char *fileName;

	if (isSpawning())
		return;

	if (!arg) {

		if (!state.fileName) {

			conAdd(2, "Please specify a name (not extensions necessary).");
			return;

		}

		arg = state.fileName;

	}

	fileName = va(SAVE_PATH "/%s.info", arg);
	if (!LoadMemoryDump(fileName, (unsigned char *)&si, sizeof(si))) {
		conAdd(2, "Failed to load %s", fileName);
		return;
	}

	// for mallocing in initFrame
	state.historyFrames = si.historyFrames;
	state.particleCount = si.particleCount;

	conAdd(1, "Loading %s...", arg);
	conAdd(0, "Particles: %i", state.particleCount);
	conAdd(0, "Frames: %i", state.historyFrames);

	if (!initFrame()) {
		conAdd(2, "Could not init frame");
		return;
	}

	state.totalFrames = si.totalFrames;
	state.frame = si.frame;
	state.historyNFrame = si.historyNFrame;

	conAdd(1, "Please Wait...");
	runVideo();

	if (!LoadMemoryDump(va(SAVE_PATH "/%s.particledetail", arg), (unsigned char *)state.particleDetail, FRAMEDETAILSIZE)) {
		conAdd(2, "Failed to load %s", fileName);
		return;
	}

	if (!LoadMemoryDump(va(SAVE_PATH "/%s.particles", arg), (unsigned char *)state.particleHistory, FRAMESIZE * (state.frame+1))) {
		conAdd(2, "Failed to load %s", fileName);
		return;
	}

	state.currentFrame = 0;
	state.mode = 0;
	conAdd(1, "Simulation loaded sucesfully!");

	setFileName(arg);
	
}

void cmdFps(char *arg) {

//	temporary removed fps
//	view.ft = (int)((float)1000 / view.fps);

}

void cmdFrameSkip(char *arg) {

    char *sz;

	sz = strtok(arg, " ");

	if (sz[0])
		view.frameSkip = atoi(sz);
	else
		conAdd(2, "nope");

}

void cmdStatus(char *arg) {

if (state.mode & SM_RECORD)
	conAdd(0, "RECORDING");
else
	conAdd(0, "NOT RECORDING");

#define DUH(s, v) conAdd(0, "%s\t%s", s, v);

	DUH("actual frames     ", va("%i", state.totalFrames));
	DUH("compression skip  ", va("%i", state.historyNFrame));
	DUH("display frame     ", va("%i", state.currentFrame));
	DUH("record frame      ", va("%i", state.frame));
	DUH("max frames        ", va("%i", state.historyFrames));
	DUH("particles         ", va("%i", state.particleCount));
	DUH("frametime         ", va("%ims", view.deltaVideoFrame));
	DUH("fps               ", va("%3.2f", (float)1000 / view.deltaVideoFrame));
	DUH("particle verticies", va("%i", view.verticies));
	DUH("memory allocated  ", va("%.1fmb", (float)state.memoryAllocated / 1024 / 1024));

#undef DUH

}

#ifndef NO_GUI

void cmdFontFile(char *arg) {

    char *sz;
	sz = strtok(arg, " ");
	if (!sz)
		return;
	strncpy(conf.fontFile, arg, MAX_FONT_LENGTH);

}

#endif

void cmdRunScript(char *arg) {

    char *sz;
	sz = strtok(arg, " ");
	if (!sz)
		return;
	configRead(sz);

}

void cmdTailSkipCheck(char *arg) {

	if (view.tailSkip <= 0) {
		conAdd(1, "tailskip %i is not valid. tailskip is now 1.", view.tailSkip);
		view.tailSkip = 1;
	}

}

void cmdScreenshot(char *arg) {

#ifndef NO_GUI

	SDL_Surface *sdlSurfUpsideDown;
	SDL_Surface *sdlSurfNormal;
	int i;
	char *fileName;

	sdlSurfUpsideDown = SDL_CreateRGBSurface(SDL_SWSURFACE, conf.screenW, conf.screenH, 24, 0x000000FF, 0x0000FF00, 0x00FF0000, 0);
	sdlSurfNormal = SDL_CreateRGBSurface(SDL_SWSURFACE, conf.screenW, conf.screenH, 24, 0x000000FF, 0x0000FF00, 0x00FF0000, 0);

	SDL_LockSurface(sdlSurfUpsideDown);
	glReadPixels(0,0,conf.screenW, conf.screenH, GL_RGB,GL_UNSIGNED_BYTE, sdlSurfUpsideDown->pixels);
	SDL_UnlockSurface(sdlSurfUpsideDown);

	SDL_LockSurface(sdlSurfUpsideDown);
	SDL_LockSurface(sdlSurfNormal);
	
	for (i = 0; i < conf.screenH; i++) {

		memcpy(
			(unsigned char *)sdlSurfNormal->pixels + (conf.screenH - i - 1) * sdlSurfNormal->pitch,
			(unsigned char *)sdlSurfUpsideDown->pixels + i * sdlSurfUpsideDown->pitch,
			3 * conf.screenW		
		);

	}

	SDL_UnlockSurface(sdlSurfUpsideDown);
	SDL_UnlockSurface(sdlSurfNormal);

	if (!mymkdir(SCREENSHOT_PATH)) {
		conAdd(2, "Could not create screenshot directory");		
		return;
	}

	// find next free screenshot file name
	while (1) {

		FILE *fp;

		fileName = va(SCREENSHOT_PATH "/gravit%05u.bmp", view.screenshotIndex++);
		fp = fopen(fileName, "rb");

		if (!fp)
			break;

		fclose(fp);
		
	}

	SDL_SaveBMP(sdlSurfNormal, fileName);

	SDL_FreeSurface(sdlSurfUpsideDown);
	SDL_FreeSurface(sdlSurfNormal);

#endif

}

void cmdColourSchemeNew(char *arg) {

#ifndef NO_GUI
	colourSpectrumClear();
	conAdd(0, "Colour scheme reset");
#endif

}

void cmdColourSchemeAdd(char *arg) {

#ifndef NO_GUI

	float c[4];
    char *r,*g,*b,*a;

	r = strtok(arg, " \t");
	if (!r) return;
	c[0] = atof(r);

	g = strtok(NULL, " \t");
	if (!g) return;
	c[1] = atof(g);
	
	b = strtok(NULL, " \t");
	if (!b) return;
	c[2] = atof(b);
	
	a = strtok(NULL, " \t");
	if (!a) c[3] = 1; else c[3] = atof(a);

	if (!view.colourSpectrum) {
		colourSpectrumClear();
	}

	view.colourSpectrumSteps++;
	view.colourSpectrum = (float *)realloc(view.colourSpectrum, view.colourSpectrumSteps*sizeof(float)*4);

	memcpy(&view.colourSpectrum[(view.colourSpectrumSteps-1)*4], &c, sizeof(float)*4);

	conAdd(0, "Added colour (%f %f %f %f) to colour scheme", c[0], c[1], c[2], c[3]);

#endif

}

void cmdSetG(char *arg) {

//	if (isSpawning()) {
		conAdd(2, "Warning! Changing the gravitantional constant while recording may cause fundimental problems! :)");
//	}

	state.g = -pow(10, -state.gbase);
	
}

void cmdAutoRotate(char *arg) {

	char *t;
	VectorNew(v);

	t = strtok(arg, " ");
	if (!t) goto cmdAutoRotateUsage;
	v[0] = atof(t);

	t = strtok(NULL, " ");
	if (!t) goto cmdAutoRotateUsage;
	v[1] = atof(t);

	t = strtok(NULL, " ");
	if (!t) goto cmdAutoRotateUsage;
	v[2] = atof(t);

	VectorCopy(v, view.autoRotate);

cmdAutoRotateUsage:

	conAdd(1, "autorotate %f %f %f", view.autoRotate);
	return;

}

void cmdInstallScreenSaver(char *arg) {

#ifdef WIN32SCREENSAVER

	char systemDir[MAX_PATH];
	char currentDir[MAX_PATH];

	GetCurrentDirectory(MAX_PATH, currentDir);
	GetSystemDirectory(systemDir, MAX_PATH);
/*
	// create a placeholder gravit.cfg file so that we know where to look when gravit is run from system32
	fp = fopen(va("%s\\screensaver.cfg", systemDir), "w");
	fprintf(fp, "# This file is automatically generated by Gravit to let Windows know where Gravit is\n");
	fprintf(fp, "cd %s\n", currentDir);
	fprintf(fp, "exec screensaver.cfg\n");
	fclose(fp);
*/

	// copy gravit.exe to c:\windows\system32 (orwhatever)
	CopyFile("gravit.exe", va("%s\\gravit.scr", systemDir), FALSE);
	ShellExecute(NULL, "open", "rundll32.exe", "desk.cpl,InstallScreenSaver gravit.scr", NULL, SW_SHOWNORMAL);

	

#endif

}

void cmdChangeDir(char *arg) {

#ifdef WIN32
	SetCurrentDirectory(arg);
#endif

}
