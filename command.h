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

#ifndef COMMAND_H_
#define COMMAND_H_

typedef struct cmd_s {

	char *cmd;
	void (*func)(char *);
	float *fVar;
	int *iVar;
	char *description;

} cmd_t;

extern cmd_t cmd[];

void cmdExecute(char *command);
int cmdFind(char *string);
char *cmdGetCommand(int i);
void commandInit();
void cmdFree();

void cmdQuit(char *args);
void cmdPause(char *args);
void cmdPlay(char *args);
void cmdRestart(char *arg);
void cmdSaveFrame(char *arg);
void cmdSaveFrameDump(char *arg);
void cmdLoadFrameDump(char *arg);
void cmdRecord(char *arg);
void cmdStop(char *arg);
void cmdStart(char *arg);
void cmdFps(char *arg);
void cmdSpawn(char *arg);
void cmdStatus(char *arg);
void cmdFontFile(char *arg);
void cmdRunScript(char *arg);
void cmdTailSkipCheck(char *arg);
void cmdScreenshot(char *arg);
void cmdScreenshotLoop(char *arg);
void cmdColourSchemeNew(char *arg);
void cmdColourSchemeAdd(char *arg);
void cmdSetG(char *arg);
void cmdAutoRotate(char *arg);
void cmdInstallScreenSaver(char *arg);
void cmdChangeDir(char *arg);
void cmdStereoWarning(char *arg);
void cmdVideoRestart(char *arg);
void cmdTimerAdd(char *arg);
void cmdTimerDel(char *arg);
void cmdTimerList(char *arg);
void cmdPopupText(char *arg);
void cmdHelp(char *arg);
void cmdList(char *arg);
void cmdSaveList(char *arg);
void cmdSaveDelete(char *arg);
void cmdUnhelpful(char *arg);
void cmdZoomFit(char *arg);
void cmdFrameSkip(char *arg);

#endif
