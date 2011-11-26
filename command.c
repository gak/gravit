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

#ifdef __MACH__
  // AudioStreamer does not compile on Windows
#include "AudioStreamer.h"
#endif

cmd_t cmd[] = {

//	   cmd							func					varf,						vari
    { "quit",						cmdQuit,				NULL,						NULL,								NULL }
    ,{ "exec",						cmdRunScript,			NULL,						NULL,								NULL }
    ,{ "help",						cmdHelp,				NULL,						NULL,								NULL }

    ,{ "load",						cmdLoadFrameDump,		NULL,						NULL,								NULL }
    ,{ "save",						cmdSaveFrameDump,		NULL,						NULL,								NULL }
    ,{ "saveauto",					NULL,					NULL,						&state.autoSave,					NULL }
    ,{ "savelist",					cmdSaveList,			NULL,						NULL,								NULL }
    ,{ "savedelete",				cmdSaveDelete,			NULL,						NULL,								NULL }

    ,{ "list",						cmdList,				NULL,						NULL,								NULL }

    ,{ "unhelpful",					cmdUnhelpful,			NULL,						NULL,								NULL }

    ,{ "memoryavailable",			NULL,					NULL,						&state.memoryAvailable,				NULL }

#ifndef NO_GUI
    ,{ "videorestart",				cmdVideoRestart,		NULL,						NULL,								NULL }

    ,{ "videowidth",				NULL,					NULL,						&video.screenWtoApply,				NULL }
    ,{ "videoheight",				NULL,					NULL,						&video.screenHtoApply,				NULL }
    ,{ "videobpp",					NULL,					NULL,						&video.screenBPP,					NULL }
    ,{ "videofullscreen",			NULL,					NULL,						&video.screenFS,					NULL }
    ,{ "videoantialiasing",			NULL,					NULL,						&video.screenAA,					NULL }

    ,{ "fontfile",					cmdFontFile,			NULL,						NULL,								NULL }
    ,{ "fontsize",					NULL,					NULL,						&video.fontSize,					NULL }

#endif

    ,{ "start",						cmdStart,				NULL,						NULL,								NULL }
    ,{ "record",					cmdRecord,				NULL,						NULL,								NULL }
    ,{ "play",						cmdPlay,				NULL,						NULL,								NULL }
    ,{ "stop",						cmdStop,				NULL,						NULL,								NULL }
    ,{ "spawn",						cmdSpawn,				NULL,						NULL,								NULL }
    ,{ "status",					cmdStatus,				NULL,						NULL,								NULL }

    ,{ "recordingvideorefreshtime",	NULL,					NULL,						(int *)(&view.recordingVideoRefreshTime),	NULL }
    ,{ "minvideoframetime",	        NULL,					NULL,						(int *)(&view.minVideoRefreshTime),	NULL }

    ,{ "screensaver",				NULL,					NULL,						&view.screenSaver,					NULL }
    ,{ "installscreensaver",		cmdInstallScreenSaver,	NULL,						NULL,								NULL }

    ,{ "cd",						cmdChangeDir,			NULL,						NULL,								NULL }

    ,{ "showcursor",				NULL,					NULL,						&view.showCursor,					NULL }

    ,{ "g",							cmdSetG,				&state.gbase,				NULL,								NULL }

    ,{ "blendmode",					NULL,					NULL,						&view.blendMode,					NULL }

    ,{ "screenshot",				cmdScreenshot,			NULL,						NULL,								NULL }
    ,{ "screenshotloop",			NULL,					NULL,						&view.screenshotLoop}

//	,{ "fps",						cmdFps,					&view.fps,					NULL,								NULL }
    ,{ "frameskip",					cmdFrameSkip,			NULL,						NULL,								NULL }
//  ,{ "frame",						NULL,					NULL,						&state.currentFrame,				NULL }
    ,{ "frame",						cmdFrame,			NULL,						NULL,				NULL }

    ,{ "tailskip",					cmdTailSkipCheck,		NULL,						&view.tailSkip,						NULL }
    ,{ "tailfaded",					NULL,					NULL,						&view.tailFaded,					NULL }
    ,{ "tailopacity",				NULL,					&view.tailOpacity,			NULL,								NULL }
    ,{ "taillength",				NULL,					NULL,						&view.tailLength,					NULL }
    ,{ "tailwidth",					NULL,					&view.tailWidth,			NULL,								NULL }

    ,{ "drawaxis",					NULL,					NULL,						&view.drawAxis,						NULL }
    ,{ "drawtree",					NULL,					NULL,						&view.drawTree,						NULL }
    ,{ "drawosd",					NULL,					NULL,						&view.drawOSD,						NULL }
    ,{ "drawcolourscheme",			NULL,					NULL,						&view.drawColourScheme,				NULL }

    ,{ "particlecount",				NULL,					NULL,						&state.particlesToSpawn,			NULL }

    ,{ "particlerendermode",		NULL,					NULL,						&view.particleRenderMode,			NULL }
    ,{ "particlerendertexture",		NULL,					NULL,						&view.particleRenderTexture,		NULL }
    ,{ "particlecolourmode",		NULL,					NULL,						&view.particleColourMode,			NULL }
    ,{ "particlesizemin",			NULL,					&view.particleSizeMin,		NULL,								NULL }
    ,{ "particlesizemax",			NULL,					&view.particleSizeMax,		NULL,								NULL }

    ,{ "spawngalcountmin",			NULL,					NULL,						&spawnVars.minGalCount,				NULL }
    ,{ "spawngalcountmax",			NULL,					NULL,						&spawnVars.maxGalCount,				NULL }
    ,{ "spawngalmassmin",			NULL,					&spawnVars.minGalMass,		NULL,								NULL }
    ,{ "spawngalmassmax",			NULL,					&spawnVars.maxGalMass,		NULL,								NULL }
    ,{ "spawngalsizemin",			NULL,					&spawnVars.minGalSize,		NULL,								NULL }
    ,{ "spawngalsizemax",			NULL,					&spawnVars.maxGalSize,		NULL,								NULL }
    ,{ "spawngalvelmin",			NULL,					&spawnVars.minGalVel,		NULL,								NULL }
    ,{ "spawngalvelmax",			NULL,					&spawnVars.maxGalVel,		NULL,								NULL }
    ,{ "spawnrangemin",				NULL,					&spawnVars.minSpawnRange,	NULL,								NULL }
    ,{ "spawnrangemax",				NULL,					&spawnVars.maxSpawnRange,	NULL,								NULL }

    ,{ "colourschemenew",			cmdColourSchemeNew,		NULL,						NULL,								NULL }
    ,{ "colourschemeadd",			cmdColourSchemeAdd,		NULL,						NULL,								NULL }

    ,{ "framecompression",			NULL,					NULL,						&state.frameCompression,			NULL }

    ,{ "verbose",					NULL,					NULL,						&view.verboseMode,					NULL }

    ,{ "processors",				NULL,					NULL,						&state.processFrameThreads,			NULL }

    ,{ "zoom",						NULL,					&view.zoom,					NULL,								NULL }
    ,{ "zoomfit",					cmdZoomFit,				NULL,						NULL,								NULL }
    ,{ "zoomfitauto",				NULL,					NULL,						&view.zoomFitAuto,					NULL }
    ,{ "autorotate",				cmdAutoRotate,			NULL,						NULL,								NULL }
    ,{ "autorecord",				NULL,					NULL,						&state.autoRecord,					NULL }

    ,{ "stereo",					cmdStereoWarning,		NULL,						&view.stereoMode,					NULL }
    ,{ "stereoseparation",			NULL,					&view.stereoSeparation,		NULL,								NULL }
    ,{ "stereoosd",					NULL,					NULL,						&view.stereoOSD,					NULL }

    ,{ "timeradd",					cmdTimerAdd,			NULL,						NULL,								NULL }
    ,{ "timerdel",					cmdTimerDel,			NULL,						NULL,								NULL }
    ,{ "timerlist",					cmdTimerList,			NULL,						NULL,								NULL }

    ,{ "popuptext",					cmdPopupText,			NULL,						NULL,								NULL }

    ,{ "autocenter",				NULL,					NULL,						&view.autoCenter,					NULL }

    ,{ "maxvertices",				NULL,					NULL,						&view.maxVertices,					NULL }

#ifdef __MACH__
    ,{ "audiostream",               cmdPlayAudioStream,		NULL,						NULL,                               NULL }

#endif
    ,{ NULL,						NULL,					NULL,						NULL,								NULL }

};

void commandInit() {

    FILE *fp;
    char buffer[FILE_CHUNK_SIZE_SMALL];

    fp = fopen(MISCDIR "/commandhelp.txt", "rb");
    if (!fp) {
        conAdd(LERR, "Could not open " MISCDIR "/commandhelp.txt");
        return;
    }

    while (fgets(buffer, FILE_CHUNK_SIZE_SMALL, fp)) {

        char *ptrCmd,*ptrDesc;
        int c;

        // get the first space
        ptrCmd = strtok(buffer, " ");
        if (!ptrCmd) continue;
        ptrDesc = strtok(NULL, "\0");
        if (!ptrDesc) continue;
        ptrDesc[strlen(ptrDesc)-1] = 0;

        c = cmdFind(ptrCmd);

        if (c == -1)
            continue;

        cmd[c].description = malloc(strlen(ptrDesc)+1);
        strcpy(cmd[c].description, ptrDesc);

    }

    fclose(fp);

}

void cmdFree() {

    int i = -1;
    cmd_t *c;

    while (1) {

        i++;
        c = &cmd[i];

        if (!c->cmd)
            return;

        if (!c->description)
            continue;

        free(c->description);
        c->description = 0;

    }

}

void cmdPrint(cmd_t *c) {

    if (c->fVar)
        conAdd(LNORM, "%s = %f", c->cmd, *c->fVar);
    else if (c->iVar)
        conAdd(LNORM, "%s = %i", c->cmd, *c->iVar);
    else
        conAdd(LNORM, "%s", c->cmd);

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

    strncpy(cmdbuf, string, CONSOLE_LENGTH);

    strtok(cmdbuf, " ");
    args = strtok(NULL, "");

    j = cmdFind(cmdbuf);

    if (j == -1) {
        conAdd(LERR, "%s: Command not found", string);
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

    conAdd(LNORM, "Stopped...");
    state.mode &= ~SM_PLAY;
    state.mode &= ~SM_RECORD;

    setTitle(0);

}

void cmdSpawnCancel(void) {

#ifdef HAVE_LUA

    conAdd(LERR, "%s", lua_tostring(state.lua, -1));
    cleanMemory();
    state.currentlySpawning = 0;
    state.particleCount = 0;
    luaFree();

#endif

}

void cmdSpawn(char *arg) {

    char *scriptName;
#ifdef HAVE_LUA
    char *scriptFile;
#endif

    if (arg)
        scriptName = arg;
    else
        scriptName = "default";

    freeFileName();

    if (state.currentlySpawning) {
        state.restartSpawning = 1;
        return;
    }

cmdSpawnRestartSpawning:

    state.currentlySpawning = 1;
    state.restartSpawning = 0;
    state.mode = 0;

    state.particleCount = state.particlesToSpawn;
    state.historyFrames = (unsigned int)(((size_t)state.memoryAvailable * 1024 * 1024) / (size_t)FRAMESIZE);

    if (!initFrame()) {
        conAdd(LERR, "Could not init frame");
        return;
    }

#ifdef HAVE_LUA

    luaInit();

    // tell lua where to find the spawn scripts
    lua_pushstring(state.lua, SPAWNDIR "/?");
    lua_setglobal(state.lua, "LUA_PATH");

    lua_pushnumber(state.lua, state.particleCount);
    lua_setglobal(state.lua, "spawnparticles");

    scriptFile = va(SPAWNDIR "/%s.gravitspawn", scriptName);

    luaExecute(findFile(scriptFile));
    lua_getglobal(state.lua, "describe");
    if (lua_pcall(state.lua, 0, 1, 0)) {
        conAdd(LERR, "Could not execute spawn function in %s", scriptFile);
        cmdSpawnCancel();
        return;
    }
    lua_getglobal(state.lua, "spawn");
    if (lua_pcall(state.lua, 0, 1, 0)) {
        conAdd(LERR, "Could not execute spawn function in %s", scriptFile);
        cmdSpawnCancel();
        return;
    }

    luaFree();

#else

    pickPositions();

#endif

    if (state.restartSpawning) {
        goto cmdSpawnRestartSpawning;
    }

    // make sure no two particles are in the same spot
    {
        int i,j;
        int needrestart;
restart:
        needrestart = 0;
        for (i = 0; i < state.particleCount; i++) {
            for (j = 0; j < state.particleCount; j++) {
                particle_t *p1;
                particle_t *p2;
                particleDetail_t *pd1;
                particleDetail_t *pd2;
                VectorNew(diff);

                if (j == i)
                    continue;

                p1 = getParticleCurrentFrame(i);
                p2 = getParticleCurrentFrame(j);
                pd1 = getParticleDetail(i);
                pd2 = getParticleDetail(j);

                if (pd1->mass == 0) {
                    conAdd(LERR, "Particle %i has no mass", i);
                    pd1->mass = 1;
                    needrestart = 1;
                }

                VectorSub(p1->pos, p2->pos, diff);
#define MIN_STEP 0.001
                if (fabs(diff[0]) < MIN_STEP && fabs(diff[1]) < MIN_STEP && fabs(diff[2]) < MIN_STEP) {
                    conAdd(LERR, "Particle %i and %i are in the same position, moving...", i, j);
                    p1->pos[0] += frand(-10,10);
                    p1->pos[1] += frand(-10,10);
                    p1->pos[2] += frand(-10,10);
                    p2->pos[0] += frand(-10,10);
                    p2->pos[1] += frand(-10,10);
                    p2->pos[2] += frand(-10,10);
                    needrestart = 1;
                }

            }
        }

        if (needrestart) {
            goto restart;
        }
    }

#ifndef NO_GUI
    setColours();
#endif

    state.currentlySpawning = 0;
    conAdd(LLOW, "You have spawned some particles. Hit F6 to start recording the simulation!");

    state.mode = 0;

    if (state.autoRecord) {
        state.autoRecordNext = 1;
    }

    if (view.zoomFitAuto) {
        cmdZoomFit(NULL);
    }

    view.frameSkipCounter = 0;

}

void cmdStart(char *arg) {

    cmdSpawn(NULL);

}

void cmdRecord(char *arg) {

    if (isSpawning())
        return;

    if (state.particleCount == 0) {

        conAdd(LHELP, "You need to spawn some particles first! Press SPACE to spawn some!");
        return;

    }

    state.mode &= ~SM_PLAY;

    if (state.mode & SM_RECORD) {

        conAdd(LNORM, "Stopped Recording.");
        conAdd(LHELP, "Press F5 to play your recording. Press F6 to continue recording.");
        state.mode &= ~SM_RECORD;
        //state.targetFrame = -1;
        setTitle(0);

    } else {

        conAdd(LNORM, "Recording...");
        conAdd(LHELP, "Press F5 to play your recording. Press F6 to stop recording.");
        state.mode |= SM_RECORD;
        setTitle(STRING_RECORD);
        view.timed_frames=0;
        view.totalRenderTime=0;

    }

}

void cmdPlay(char *arg) {

    if (isSpawning())
        return;

    if (state.particleCount == 0) {

        conAdd(LHELP, "You need to spawn some particles first! Press SPACE to spawn some!");
        return;

    }

    state.mode &= ~SM_RECORD;

    if (state.mode & SM_PLAY) {

        conAdd(LNORM, "Stopped Playback.");
        conAdd(LHELP, "Press F5 to continue playback. Press F6 to continue recording.");
        state.mode &= ~SM_PLAY;
        setTitle(0);

    } else {

        conAdd(LNORM, "Playing...");
        conAdd(LHELP, "Press F5 to stop playback. Press F6 to continue recording.");
        state.mode |= SM_PLAY;
        //state.targetFrame = -1;
        setTitle(STRING_PLAY);

    }

}

void cmdFrame(char *arg) {
    int new_target_frame;

    if ((arg) && (atoi(arg) >= 0)) {
        new_target_frame=atoi(arg);
    } else {
        new_target_frame=state.currentFrame;
    }


    if ( new_target_frame <= state.frame ) {
        // just set frame to display
        state.currentFrame = new_target_frame;
        conAdd(LNORM, "frame = %i", state.currentFrame);

    } else {
        // set target_frame
        state.currentFrame = state.frame;
        state.targetFrame = new_target_frame;
        // change to record_mode
	if ((state.mode & SM_RECORD) == 0) cmdRecord(NULL);
        conAdd(LNORM, "current frame = %i, advancing to frame %i", state.currentFrame, state.targetFrame);
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

    conAdd(LERR, "saving frames %i to %i skipping %i to %s", staf, endf, skif, fileName);

}
#endif

void cmdSaveFrameDump(char *arg) {

    saveInfo_t si;
    char *fileName;

    if (isSpawning())
        return;

    if (!arg) {

        if (!state.fileName) {

            conAdd(LERR, "Please specify a name (not extensions necessary).");
            conAdd(LNORM, "usage: save [name]");
            return;

        }

        arg = state.fileName;

    }

    if (!checkHomePath()) return;

    if (!mymkdir(SAVE_PATH)) {
        conAdd(LERR, "Could not create %s directory", SAVE_PATH);
        return;
    }

    si.particleCount = state.particleCount;
    si.historyFrames = state.historyFrames;
    si.totalFrames = state.totalFrames;
    si.frame = state.frame;
    si.historyNFrame = state.historyNFrame;

    conAdd(LNORM, "Saving %s...", arg);
    conAdd(LNORM, "Please Wait...");
    runVideo();

    fileName = va("%s/%s.info", SAVE_PATH, arg);
    if (!SaveMemoryDump(fileName, (unsigned char *)&si, sizeof(si))) {
        conAdd(LERR, "Failed to create %s", fileName);
        return;
    }

    fileName = va("%s/%s.particledetail", SAVE_PATH, arg);
    if (!SaveMemoryDump(fileName, (unsigned char *)state.particleDetail, FRAMEDETAILSIZE)) {
        conAdd(LERR, "Failed to create %s", fileName);
        return;
    }

    fileName = va("%s/%s.particles", SAVE_PATH, arg);
    if (!SaveMemoryDump(fileName, (unsigned char *)state.particleHistory, FRAMESIZE * (state.frame+1))) {
        conAdd(LERR, "Failed to create %s", fileName);
        return;
    }
    conAdd(LNORM, "Simulation saved sucesfully!");

    setFileName(arg);

}

void cmdLoadFrameDump(char *arg) {

    saveInfo_t si;
    char *fileName;

    if (isSpawning())
        return;

    if (!arg) {

        if (!state.fileName) {

            conAdd(LERR, "Please specify a name (not extensions necessary).");
            return;

        }

        arg = state.fileName;

    }

    if (!checkHomePath()) return;

    fileName = va("%s/%s.info", SAVE_PATH, arg);
    if (!LoadMemoryDump(fileName, (unsigned char *)&si, sizeof(si))) {
        conAdd(LERR, "Failed to load %s", fileName);
        return;
    }

    // for mallocing in initFrame
    state.historyFrames = si.historyFrames;
    state.particleCount = si.particleCount;

    conAdd(LNORM, "Loading %s...", arg);
    conAdd(LLOW, "Particles: %i", state.particleCount);
    conAdd(LLOW, "Frames: %i", state.historyFrames);

    if (!initFrame()) {
        conAdd(LERR, "Could not init frame");
        return;
    }

    state.totalFrames = si.totalFrames;
    state.frame = si.frame;
    state.historyNFrame = si.historyNFrame;

    conAdd(LNORM, "Please Wait...");
    runVideo();

    fileName = va("%s/%s.particledetail", SAVE_PATH, arg);
    if (!LoadMemoryDump(fileName, (unsigned char *)state.particleDetail, FRAMEDETAILSIZE)) {
        conAdd(LERR, "Failed to load %s", fileName);
        return;
    }

    fileName = va("%s/%s.particles", SAVE_PATH, arg);
    if (!LoadMemoryDump(fileName, (unsigned char *)state.particleHistory, FRAMESIZE * (state.frame+1))) {
        conAdd(LERR, "Failed to load %s", fileName);
        return;
    }

    state.currentFrame = 0;
    state.mode = 0;
    conAdd(LNORM, "Simulation loaded sucesfully!");

    setFileName(arg);

}

void cmdFps(char *arg) {

//	temporary removed fps
//	view.ft = (int)((float)1000 / view.fps);

}

void cmdFrameSkip(char *arg) {

    if (arg) {
        view.frameSkip = atoi(arg);
        view.frameSkipCounter = 0;
    }
    conAdd(LNORM, "frameSkip = %i", view.frameSkip);

}

void cmdStatus(char *arg) {

    if (state.mode & SM_RECORD)
        conAdd(LLOW, "RECORDING");
    else
        conAdd(LLOW, "NOT RECORDING");

#define DUH(s, v) conAdd(LLOW, "%s\t%s", s, v);

    DUH("actual frames     ", va("%i", state.totalFrames));
    DUH("compression skip  ", va("%i", state.historyNFrame));
    DUH("display frame     ", va("%i", state.currentFrame));
    DUH("record frame      ", va("%i", state.frame));
    DUH("max frames        ", va("%i", state.historyFrames));
    DUH("particles         ", va("%i", state.particleCount));
    DUH("frametime         ", va("%ims", view.deltaVideoFrame));
    DUH("fps               ", va("%3.2f", (float)1000 / view.deltaVideoFrame));
    DUH("particle vertices", va("%i", view.vertices));
    DUH("memory allocated  ", va("%.1fmb", (float)state.memoryAllocated / 1024 / 1024));

#undef DUH

}

#ifndef NO_GUI

void cmdFontFile(char *arg) {

    char *sz;
    sz = strtok(arg, " ");
    if (!sz)
        return;
    strncpy(video.fontFile, arg, MAX_FONT_LENGTH);

}

#endif

void cmdRunScript(char *arg) {

    char *sz;
    char *opt;
    sz = strtok(arg, " ");
    if (!sz) return;
    opt = strtok(NULL, " ");
    configRead(findFile(sz), (opt && !strcmp(opt, "ignoremissing")));

}

void cmdTailSkipCheck(char *arg) {

    if (view.tailSkip <= 0) {
        conAdd(LNORM, "tailskip %i is not valid. tailskip is now 1.", view.tailSkip);
        view.tailSkip = 1;
    }

}

void cmdScreenshot(char *arg) {

#ifndef NO_GUI

    SDL_Surface *sdlSurfUpsideDown;
    SDL_Surface *sdlSurfNormal;
    int i;
    char *fileName;

    if (!checkHomePath())
        return;

    sdlSurfUpsideDown = SDL_CreateRGBSurface(SDL_SWSURFACE, video.screenW, video.screenH, 24, 0x000000FF, 0x0000FF00, 0x00FF0000, 0);
    sdlSurfNormal = SDL_CreateRGBSurface(SDL_SWSURFACE, video.screenW, video.screenH, 24, 0x000000FF, 0x0000FF00, 0x00FF0000, 0);

    SDL_LockSurface(sdlSurfUpsideDown);
    glReadPixels(0,0,video.screenW, video.screenH, GL_RGB,GL_UNSIGNED_BYTE, sdlSurfUpsideDown->pixels);
    SDL_UnlockSurface(sdlSurfUpsideDown);

    SDL_LockSurface(sdlSurfUpsideDown);
    SDL_LockSurface(sdlSurfNormal);

    for (i = 0; i < video.screenH; i++) {

        memcpy(
            (unsigned char *)sdlSurfNormal->pixels + (video.screenH - i - 1) * sdlSurfNormal->pitch,
            (unsigned char *)sdlSurfUpsideDown->pixels + i * sdlSurfUpsideDown->pitch,
            3 * video.screenW
        );

    }

    SDL_UnlockSurface(sdlSurfUpsideDown);
    SDL_UnlockSurface(sdlSurfNormal);

    if (!mymkdir(SCREENSHOT_PATH)) {
        conAdd(LERR, "Could not create screenshot directory");
        return;
    }

    // find next free screenshot file name
    while (1) {

        FILE *fp;

#ifdef HAVE_PNG
        fileName = va("%s/gravit%05u.png", SCREENSHOT_PATH, view.screenshotIndex++);
#else
        fileName = va("%s/gravit%05u.bmp", SCREENSHOT_PATH, view.screenshotIndex++);
#endif
        fp = fopen(fileName, "rb");

        if (!fp)
            break;

        fclose(fp);

    }

#ifdef HAVE_PNG
    png_save_surface(fileName, sdlSurfNormal);
#else
    SDL_SaveBMP(sdlSurfNormal, fileName);
#endif

    SDL_FreeSurface(sdlSurfUpsideDown);
    SDL_FreeSurface(sdlSurfNormal);

#endif

}

void cmdColourSchemeNew(char *arg) {

#ifndef NO_GUI
    colourSpectrumClear();
    conAdd(LLOW, "Colour scheme reset");
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
    if (!a) c[3] = 1;
    else c[3] = atof(a);

    if (!view.colourSpectrum) {
        colourSpectrumClear();
    }

    view.colourSpectrumSteps++;
    view.colourSpectrum = (float *)realloc(view.colourSpectrum, view.colourSpectrumSteps*sizeof(float)*4);

    memcpy(&view.colourSpectrum[(view.colourSpectrumSteps-1)*4], &c, sizeof(float)*4);

    conAdd(LLOW, "Added colour (%.2f %.2f %.2f %.2f) to colour scheme", c[0], c[1], c[2], c[3]);

#endif

}

void cmdSetG(char *arg) {

    float newg;

    newg = -pow(10, -state.gbase);
    conAdd(LLOW, "\"G\" set to %.20f", newg);
    state.g = newg;

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

    conAdd(LNORM, "autorotate %f %f %f", view.autoRotate[0], view.autoRotate[1], view.autoRotate[2]);
    return;

}

void cmdInstallScreenSaver(char *arg) {

#ifdef WIN32SCREENSAVER

    char systemDir[MAX_PATH];
    char currentDir[MAX_PATH];

    GetCurrentDirectory(MAX_PATH, currentDir);
    GetSystemDirectory(systemDir, MAX_PATH);

    // copy gravit.exe to c:\windows\system32 (orwhatever)
    CopyFile("gravit.exe", va("%s\\gravit.scr", systemDir), FALSE);
    ShellExecute(NULL, "open", "rundll32.exe", "desk.cpl,InstallScreenSaver gravit.scr", NULL, SW_SHOW);

#endif

}

void cmdChangeDir(char *arg) {

#ifdef WIN32
    SetCurrentDirectory(arg);
#endif

}

void cmdStereoWarning(char *arg) {

    if (view.stereoMode) {
        conAdd(LHELP, "You have selected stereoscopic mode. The stereoseparation value is how many degrees between your eyes there are. ");
        conAdd(LHELP, "Use a negative value for parallel viewing and positive for cross-eyed viewing. Recommended are less then 5 degrees.");
    }

    if (view.stereoMode)
        conAdd(LERR, "WARNING: Stereo mode may damage your eyes or other things. Use with caution.");

}

void cmdVideoRestart(char *arg) {

    gfxSetResolution();

}

int cmdGetArgs(int count, char *arg, char *ptrs[]) {

    int i;
    ptrs[0] = strtok(arg, " ");
    if (!ptrs[0]) return 0;
    for (i = 1; i < count-1; i++) {
        ptrs[i] = strtok(NULL, " ");
        if (!ptrs[i])
            return 0;
    }

    ptrs[count-1] = ptrs[count-2] + strlen(ptrs[count-2]) + 1;
    if (!ptrs[count-1]) return 0;

    return 1;

}

void cmdTimerAdd(char *arg) {

    char *argptr[4];
    if (!cmdGetArgs(4, arg, argptr)) {
        conAdd(LERR, "usage: timeradd [name] [interval] [repetitions] [command]");
        return;
    }

    timerAdd(argptr[0], atoi(argptr[1]), atoi(argptr[2]), argptr[3]);

}

void cmdTimerDel(char *arg) {

    timerDel(arg);

}

void cmdTimerList(char *arg) {

    timerList();

}

void cmdPopupText(char *arg) {

    char *argptr[2];
    if (!cmdGetArgs(2, arg, argptr)) {
        conAdd(LHELP, "usage: popuptext [ms] [message]");
        return;
    }

    view.popupTextLength = atof(argptr[0]);
    strncpy(view.popupTextMessage, argptr[1], 255);
    view.popupTextStart = getMS();

}

void cmdHelp(char *arg) {

    int i;

    if (!arg) {

        conAdd(LHELP, "Gravit Help");
        conAdd(LHELP, "There are quite a few console commands that you can use.");
        conAdd(LHELP, "To display all possible commands, type \"list\" into the console.");
        conAdd(LHELP, "To get help on any command type in: \"help command\" into the console.");
        return;

    }

    i = cmdFind(arg);
    conAdd(LHELP, "Gravit Help");

    if (i == -1) {

        conAdd(LHELP, "Could not find \"%s\"", arg);
        return;

    }

    cmdPrint(&cmd[i]);

    if (!cmd[i].description)
        conAdd(LHELP, "No help available!");
    else
        conAdd(LHELP, cmd[i].description);

}

void cmdList(char *arg) {

    int i = -1;
    cmd_t *c;
    float charsPrinted = 0;
    char buf[1000];
    buf[0] = 0;

    while (1) {

        i++;
        c = &cmd[i];

        if (!c->cmd)
            break;

        charsPrinted+=strlen(c->cmd);
        if (charsPrinted > 100) {
            conAdd(LHELP, buf);
            buf[0] = 0;
            charsPrinted = strlen(c->cmd);
        }
        strcat(buf, c->cmd);
        strcat(buf, "  ");

    }

    conAdd(LHELP, buf);

}

void cmdSaveList(char *arg) {

    char *file;
    saveInfo_t si;

#ifdef WIN32
    HANDLE h;
    WIN32_FIND_DATA fd;
#endif

    if (!checkHomePath()) return;

#ifdef WIN32

    h = FindFirstFile(va("%s/*.info", SAVE_PATH), &fd);
    while (1) {

        file = fd.cFileName;

#else

    DIR *d;
    struct dirent *f;
    d = opendir(SAVE_PATH);
    if (!d) {
        conAdd(LERR, "Could not access %s", SAVE_PATH);
        return;
    }

    while ((f = readdir(d)) != NULL) {
        file = f->d_name;
        if (strcmp(&file[strlen(file)-5], ".info"))
            continue;

#endif

        if (!LoadMemoryDump(va("%s/%s", SAVE_PATH, file), (unsigned char *)&si, sizeof(si))) {
            conAdd(LERR, "Failed to load %s", file);
            return;
        }
        file[strlen(file)-5] = 0;
        conAdd(LNORM, "%s - %i particles, %i frames", file, si.particleCount, si.totalFrames);

#ifdef WIN32

        if (!FindNextFile(h, &fd)) break;
    }

#else

    }
    closedir(d);

#endif

}

void cmdSaveDelete(char *arg) {

    char *file;
    if (!arg) {
        conAdd(LHELP, "usage: savedelete [name]");
        return;
    }

    if (!checkHomePath()) return;

    file = va("%s/%s.info", SAVE_PATH, arg);
    if (!myunlink(file)) {
        conAdd(LERR, "Unable to delete %s", file);
        return;
    }
    file = va("%s/%s.particledetail", SAVE_PATH, arg);
    if (!myunlink(file)) {
        conAdd(LERR, "Unable to delete %s", file);
        return;
    }
    file = va("%s/%s.particles", SAVE_PATH, arg);
    if (!myunlink(file)) {
        conAdd(LERR, "Unable to delete %s", file);
        return;
    }

    conAdd(LNORM, "Deleted %s", arg);

    freeFileName();

}

void cmdUnhelpful(char *arg) {

    int i = -1;
    cmd_t *c;

    while (1) {

        i++;
        c = &cmd[i];

        if (!c->cmd)
            return;

        if (!c->description || !c->description[0]) {
            conAdd(LNORM, "%s", c->cmd);
        }

    }

}

void cmdZoomFit(char *arg) {

    particle_t *p;
    float d = 0;
    int i;

    for (i = 0; i < state.particleCount; i++) {

        p = getParticleCurrentFrame(i);

        if (fabs(p->pos[0]) > d) d = fabs(p->pos[0]);
        if (fabs(p->pos[1]) > d) d = fabs(p->pos[1]);
        if (fabs(p->pos[2]) > d) d = fabs(p->pos[2]);

    }

    if (d > 0)
        view.zoom = d * 5;

}

#ifdef __MACH__
void cmdPlayAudioStream(char *arg) {

    char *sz;
    sz = strtok(arg, " ");
    if (!sz)
        return;

    NSString *stream = [NSString stringWithCString:sz];
    [AudioStreamer playWithString:stream];

}
#endif
