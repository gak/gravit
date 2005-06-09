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

con_t con[CONSOLE_HISTORY];
int cpos = 0;

char conCommand[CONSOLE_LENGTH+1];
int conCommandPos;
unsigned int conBlinkTime;
int conBlinkOn;

char *conTypedHistory[CONSOLE_TYPED_HISTORY];
int conTypedHistoryPos;
int conTypedHistoryPointer;

#define MAX_COMPLETE_LIST 20
int conCompPos;	// the position of the last BC
char conCompWord[CONSOLE_LENGTH+1]; // the current BC
int conCompWordsFoundCount; // commands found starting with BC
char *conCompWordsFoundPtrs[MAX_COMPLETE_LIST]; // an array of commands starting with BC
int conCompWordsFoundIndex;

static col_t cols[] = {

	{0.5f, 0.5f, 0.5f}
	,{1.0f, 1.0f, 1.0f}
	,{0.5f, 0.2f, 0.2f}
	,{0, .6f, .8f}

};

void conAdd(int mode, char *f, ... ) {

	char s[1024];
	va_list		argptr;

	va_start (argptr, f);
	vsprintf (s, f, argptr);
	va_end (argptr);

	if (strlen(s) >= CONSOLE_LENGTH-1)
		s[CONSOLE_LENGTH-1] = 0;

	if (mode > 3)
		mode = 0;

	cpos++;

	if (cpos > CONSOLE_HISTORY - 1)
		cpos = 0;

	strncpy(con[cpos].s, s, CONSOLE_LENGTH-1);
	memcpy(&con[cpos].c, &cols[mode], sizeof(con[cpos].c));

	printf("%s\n", s);

#if 0
	{

		FILE *fp;
		fp = fopen("meh.txt", "a");
		fprintf(fp, "%s\n", s);
		fclose(fp);
	}
#endif	

}

#ifndef NO_GUI

void conDraw() {

	int i, p;

	float x,y,w;

	unsigned int currentTime;

	p = cpos;

	x = 10;

	if (view.consoleMode) {

		int inputBoxMargin = 3;
	
		drawFrameSet2D();
		glEnable(GL_BLEND);
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		glBindTexture(GL_TEXTURE_2D, 0);

		// cursor position
		y = video.screenH - 10 - fontHeight;

		// draw box
		glDisable(GL_BLEND);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBegin(GL_QUADS);

		glColor4f(0,0,.5f,.5f);
		glVertex2f(x - inputBoxMargin, y + fontHeight + inputBoxMargin);	// bot left
		glVertex2f(x - inputBoxMargin, y - inputBoxMargin);					// top left
		
		glColor4f(0,0,0,0);
		glVertex2f(x + video.screenW / 2, y - inputBoxMargin);				// top right
		glVertex2f(x + video.screenW / 2, y + fontHeight + inputBoxMargin);	// bot right

		glEnd();

		// draw cursor
		currentTime = SDL_GetTicks();

		if (conBlinkTime + CONSOLE_BLINK_TIME < currentTime) {

			conBlinkOn = !conBlinkOn;
			conBlinkTime = currentTime;

		}

		if (conBlinkOn) {

			w = getnWordWidth(conCommand, conCommandPos);
			glDisable(GL_BLEND);
			glLineWidth(1.0f);
			glBindTexture(GL_TEXTURE_2D, 0);
			glColor4f(1,1,1,1);
			glBegin(GL_LINES);

				glVertex2f(x + w, y);
				glVertex2f(x + w, y + fontHeight);

			glEnd();

		}

		// draw current command
		glColor4f(1,1,1,1);
		drawFontWord(x, y, conCommand);

	}

	// draw console history
	y = video.screenH - 15 - fontHeight * 2;

	for (i = 0; i < CONSOLE_HISTORY; i++) {

		glColor4f(con[p].c.r, con[p].c.g, con[p].c.b, (float)(CONSOLE_HISTORY-i) / CONSOLE_HISTORY + 0.2f);

		drawFontWord(x, y, con[p].s);
		y -= fontHeight;

		p--;
		if (p < 0)
			p = CONSOLE_HISTORY - 1;

	}

}

#endif

void conInit() {

	memset(con, 0, sizeof(con));
	memset(conCommand, 0, sizeof(conCommand));
	conCommandPos = 0;
	conBlinkTime = getMS();
	conBlinkOn = 1;

	memset(conTypedHistory, 0, sizeof(conTypedHistory));
	conTypedHistoryPos = 0;
	conTypedHistoryPointer = 0;

	conCompPos = 0;
	conCompWord[0] = 0;
	conCompWordsFoundCount = 0;
	memset(conCompWordsFoundPtrs, 0, sizeof(conCompWordsFoundPtrs));
	conCompWordsFoundIndex = 0;

}

void conFree() {

	int i;

	for (i = 0; i < CONSOLE_TYPED_HISTORY; i++) {
		if (conTypedHistory[i]) {
			free(conTypedHistory[i]);
			conTypedHistory[i] = 0;
		}
	}

}

void conInput(SDLKey c) {

	if (c >= 32 && c < 128) {

		if (c == SDLK_SPACE && conCommandPos == 0)
			return;

		if (conCommandPos < CONSOLE_LENGTH) {
			int i,l;
			l = strlen(conCommand);
			for (i = strlen(conCommand); i > conCommandPos-1; i--)
				conCommand[i+1]=conCommand[i];
			
			conCommand[conCommandPos] = c;
			conCommandPos++;
			conCompWord[0] = 0;

		}
		return;

	}

	if (c == SDLK_BACKSPACE) {

		if (conCommandPos > 0) {

			strcpy(&conCommand[conCommandPos-1], &conCommand[conCommandPos]);
			conCommandPos--;

		}
		conCompWord[0] = 0;
		return;

	}

	if (c == SDLK_RETURN || c == 10) {

		printf("\n\r");

        if (conCommandPos == 0) {
			view.consoleMode = 0;
			return;		
        }

		// add command to typed history
		conTypedHistoryAdd(conCommand);

		view.consoleMode = 0;

		cmdExecute(conCommand);

		conCommand[0] = 0;
		conCommandPos = 0;
		conCompWord[0] = 0;

		return;

	}

	if (c == SDLK_BACKQUOTE || c == SDLK_ESCAPE) {

		view.consoleMode = 0;
		conCommandPos = 0;
		conCommand[0] = 0;
		conCompWord[0] = 0;
		return;

	}

	if (c == SDLK_TAB) {
		conAutoComplete();
		return;
	}

	if (c == SDLK_UP) {
		conTypedHistoryChange(-1);
		return;
	}

	if (c == SDLK_DOWN) {
		conTypedHistoryChange(1);
		return;
	}

	if (c == SDLK_LEFT) {
		if (conCommandPos > 0) {
			conCommandPos--;
		}
		return;
	}

	if (c == SDLK_RIGHT) {
		if (conCommandPos < (signed)strlen(conCommand)) {
			conCommandPos++;
		}
		return;
	}

	conAdd(LLOW, "unknown key: %u", c);

}

void conTypedHistoryAdd(char *s) {
	
	conTypedHistory[conTypedHistoryPos] = realloc(conTypedHistory[conTypedHistoryPos], strlen(s)+1);
	strcpy(conTypedHistory[conTypedHistoryPos], conCommand);

	conTypedHistoryPos++;

	if (conTypedHistoryPos == CONSOLE_TYPED_HISTORY) {
		conTypedHistoryPos = 0;
	}

	conTypedHistoryPointer = conTypedHistoryPos;

}

void conTypedHistoryChange(int i) {

	int lastPtr = conTypedHistoryPointer;
	conTypedHistoryPointer+=i;

	if (conTypedHistoryPointer < 0) conTypedHistoryPointer = CONSOLE_TYPED_HISTORY-1;
	if (conTypedHistoryPointer >= CONSOLE_TYPED_HISTORY) conTypedHistoryPointer = 0;

	// make sure conCommand is at conTypedHistoryPos
	if (conTypedHistoryPos == lastPtr) {
		conTypedHistory[conTypedHistoryPos] = realloc(conTypedHistory[conTypedHistoryPos], strlen(conCommand)+1);
		strcpy(conTypedHistory[conTypedHistoryPos], conCommand);
	}

	//
	if (!conTypedHistory[conTypedHistoryPointer]) {
		conTypedHistoryPointer = lastPtr;
		return;
	}

	strcpy(conCommand, conTypedHistory[conTypedHistoryPointer]);
	conCommandPos = strlen(conCommand);

}

void conAutoComplete() {

	int lenCommand;
	int lenString;
	int i, j;
	cmd_t *c;
	
	if (!strlen(conCommand))
		return;
	
	// see if there is an tab complete in progress
	if (strlen(conCompWord) == 0 || strncmp(conCompWord, conCommand, strlen(conCompWord)) != 0) {

		// new tab complete, lets search for words
		conCompWordsFoundCount = 0;
		conCompWord[0] = 0;
		lenString = strlen(conCommand);
		i = -1;

		while (1) {

			i++;
			c = &cmd[i];

			if (!c->cmd)
				break;

			lenCommand = strlen(c->cmd);

			if (lenString > lenCommand) {
				continue;
			}

			if (!strncmp(conCommand, c->cmd, lenString)) {
				conAdd(LLOW, c->cmd);
				// this is the first command found, so lets save it as the most common word
				if (!conCompWordsFoundCount) {
					strcpy(conCompWord, c->cmd);
				// see what we can match
				} else {
					int l;
					l = strlen(conCompWord);
					if (lenCommand < l)
						l = lenCommand;
					for (j = 0; j < l; j++) {
						if (conCompWord[j] != c->cmd[j])
							break;
					}
					conCompWord[j] = 0;
				}
				if (conCompWordsFoundCount < MAX_COMPLETE_LIST) {
					conCompWordsFoundPtrs[conCompWordsFoundCount] = c->cmd;
					conCompWordsFoundCount++;
				} else {
					break;
				}
			}
		}
	
		conCompWordsFoundIndex = -1;
		
	}

	if (!conCompWordsFoundCount) {
		conAdd(LLOW, "No commands starting with %s", conCommand);
		return;
	}
	
	conCompWordsFoundIndex++;
	if (conCompWordsFoundIndex >= conCompWordsFoundCount)
		conCompWordsFoundIndex = 0;
	strcpy(conCommand, conCompWordsFoundPtrs[conCompWordsFoundIndex]);
	conCommandPos = strlen(conCommand);
	
}

