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

static cmd_t cmd[] = {

	//	   cmd				func			varf,	vari
	{ "quit",			cmdQuit,			NULL,	NULL }

	,{ "exec",			cmdRunScript,		NULL,	NULL }

	,{ "memoryavailable",		NULL,		NULL,	&conf.memoryAvailable }

	,{ "videowidth",		NULL,			NULL,	&conf.screenW }
	,{ "videoheight",		NULL,			NULL,	&conf.screenH }
	,{ "videobpp",			NULL,			NULL,	&conf.screenBPP }
	,{ "videofullscreen",	NULL,			NULL,	&conf.screenFS }
	,{ "videoantialiasing",	NULL,			NULL,	&conf.screenAA }

	,{ "fontfile",			cmdFontFile,	NULL,	NULL }
	,{ "fontsize",			NULL,			NULL,	&conf.fontSize }

	,{ "start",			cmdStart,			NULL,	NULL }
	,{ "record",		cmdRecord,			NULL,	NULL }
	,{ "play",			cmdPlay,			NULL,	NULL }
	,{ "stop",			cmdStop,			NULL,	NULL }
	,{ "spawn",			cmdSpawn,			NULL,	NULL }
	,{ "status",		cmdStatus,			NULL,	NULL }

	//,{ "saveframe",		cmdSaveFrame,		NULL,	NULL }

	,{ "loadframedump",	cmdLoadFrameDump,	NULL,	NULL }
	,{ "saveframedump",	cmdSaveFrameDump,	NULL,	NULL }

	,{ "fps",			cmdFps,				&view.fps,	NULL }
	,{ "frameskip",		NULL,				NULL, &view.frameSkip }
	,{ "frame",			NULL,				NULL, &state.currentFrame }

	,{ "tailskip",		NULL,				NULL, &view.tailSkip }
	,{ "tailfaded",		NULL,				NULL, &view.tailFaded }
	,{ "tailfaded",		NULL,				&view.tailOpacity, NULL }
	,{ "taillength",	NULL,				NULL, &view.tailLength }
	,{ "tailwidth",		NULL,				&view.tailWidth, NULL }

	,{ "drawaxis",		NULL,				NULL, &view.drawAxis }
	,{ "drawtree",		NULL,				NULL, &view.drawTree }

	,{ "particlecolormode",		NULL,				NULL, &view.particleColorMode }
	,{ "particlesize",			NULL,				NULL, &view.particleSize }
	,{ "particlecount",			NULL,				NULL, &state.particlesToSpawn }
	
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

	,{ "framecompression",		NULL,				NULL, &state.frameCompression }

	,{ "verbose",				NULL,				NULL, &view.verboseMode }
	
	,{ "processors",			NULL,				NULL, &state.processFrameThreads }

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

}

void cmdStop(char *arg) {

	conAdd(1, "Stopped...");
	state.mode &= ~SM_PLAY;
	state.mode &= ~SM_RECORD;

}

void cmdSpawn(char *arg) {

	if (!initFrame()) {
		conAdd(1, "Could not init frame");
		return;
	}

	pickPositions();

#ifndef NO_GUI
	setColors();
#endif

}

void cmdStart(char *arg) {

	cmdSpawn(NULL);
	state.mode = 0;

}

void cmdRecord(char *arg) {

	if (state.particleCount == 0) {

		conAdd(1, "You need to spawn some particles first!");
		return;

	}

	conAdd(1, "Recording...");
	state.mode |= SM_RECORD;

}

void cmdPlay(char *arg) {

	conAdd(1, "Recording...");
	state.mode |= SM_PLAY;

}

void cmdSaveFrameUsage() {

	conAdd(1, "Usage:");
	conAdd(1, "saveframe [name] [start] [end] [skip]");
	conAdd(1, "saveframe [name] all [skip]");

}

void cmdSaveFrame(char *arg) {

    char *fileName, *s2, *s3, *s4;
	int staf, endf, skif;

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

void cmdSaveFrameDump(char *arg) {

	FILE *fp;
	saveInfo_t si;

	if (!arg) {

		conAdd(1, "Need name");
		return;

	}

	si.particleCount = state.particleCount;
	si.historyFrames = state.historyFrames;
	si.totalFrames = state.totalFrames;
	si.frame = state.frame;
	si.historyNFrame = state.historyNFrame;

	conAdd(0, "Saving %s...", arg);
	conAdd(0, "Particles: %i", state.particleCount);
	conAdd(0, "Frames: %i", state.historyFrames);

	fp = fopen(va("save/%s.info", arg), "wb");

	if (!fp)
		exit(-1);

	if (!fwrite(&si, sizeof(si), 1, fp))
		exit(-1);

	fclose(fp);

	if (!SaveMemoryDump(va("save/%s.particles", arg), (unsigned char *)state.particleHistory, FRAMESIZE * state.historyFrames)) {

		conAdd(1, "something fucked up...");
		return;

	}

}


void cmdLoadFrameDump(char *arg) {

	FILE *fp;
	saveInfo_t si;

	if (!arg) {

		conAdd(1, "Need name");
		return;

	}

	fp = fopen(va("save/%s.info", arg), "rb");

	if (!fp)
		exit(-1);

	if (!fread(&si, sizeof(si), 1, fp))
		exit(-1);

	fclose(fp);

	// for mallocing in initFrame
	state.historyFrames = si.historyFrames;
	state.particleCount = si.particleCount;

	conAdd(0, "Loading %s...", arg);
	conAdd(0, "Particles: %i", state.particleCount);
	conAdd(0, "Frames: %i", state.historyFrames);

	if (!initFrame()) {
		conAdd(1, "Could not init frame");
		return;
	}

	state.totalFrames = si.totalFrames;
	state.frame = si.frame;
	state.historyNFrame = si.historyNFrame;

	if (!LoadMemoryDump(va("save/%s.particles", arg), (unsigned char *)state.particleHistory, FRAMESIZE * state.historyFrames)) {

		conAdd(1, "It fucked up");
//		return;

	}

	state.currentFrame = 0;
	cmdPlay(NULL);

}

void cmdFps(char *arg) {

	view.ft = (int)((float)1000 / view.fps);

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
	DUH("frametime         ", va("%ims", view.dt));
	DUH("fps               ", va("%3.2f", (float)1000 / view.dt));
	DUH("particle verticies", va("%i", view.verticies));
	DUH("memory allocated  ", va("%.1fmb", (float)state.memoryAllocated / 1024 / 1024));

#undef DUH

}

void cmdFontFile(char *arg) {

    char *sz;
	sz = strtok(arg, " ");
	if (!sz)
		return;
	strncpy(conf.fontFile, arg, MAX_FONT_LENGTH);

}

void cmdRunScript(char *arg) {

    char *sz;
	sz = strtok(arg, " ");
	if (!sz)
		return;
	configRead(sz);

}
