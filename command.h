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

#ifndef COMMAND_H_
#define COMMAND_H_

typedef struct cmd_s {

	char *cmd;
	void (*func)(char *);
	float *fVar;
	int *iVar;

} cmd_t;

extern cmd_t cmd[];

void cmdExecute(char *command);
int cmdFind(char *string);
char *cmdGetCommand(int i);

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

#endif
