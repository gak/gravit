#include "graviton.h"

con_t con[CONSOLE_HISTORY];
int cpos = 0;

char conCommand[CONSOLE_LENGTH+1];
int conCommandPos;
float conBlink;

static col_t cols[] = {

	{0.5f, 0.5f, 0.5f}
	,{1.0f, 1.0f, 1.0f}
	,{0.5f, 0.2f, 0.2f}

};

void conAdd(int mode, char *f, ... ) {

	char s[1024];
	va_list		argptr;

	va_start (argptr, f);
	vsprintf (s, f, argptr);
	va_end (argptr);

	if (strlen(s) >= CONSOLE_LENGTH-1)
		s[CONSOLE_LENGTH-1] = 0;

	if (mode > 2)
		mode = 0;

	cpos++;

	if (cpos > CONSOLE_HISTORY - 1)
		cpos = 0;

	strncpy(con[cpos].s, s, CONSOLE_LENGTH-1);
	memcpy(&con[cpos].c, &cols[mode], sizeof(con[cpos].c));

	printf("%s\n", s);

}

#ifndef NO_GUI

void conDraw() {

	int i, p;

	float x,y,w;

	p = cpos;

	x = 10;

	if (view.consoleMode) {

		// draw cursor
		y = conf.screenH - 10 - fontHeight;
		conBlink++;

		if (conBlink < CONSOLE_BLINK_TIME/2) {

			w = getWordWidth(conCommand);
			glDisable(GL_BLEND);
			glLineWidth(1.0f);
			glBindTexture(GL_TEXTURE_2D, 0);
			glColor4f(1,1,1,1);
			glBegin(GL_LINES);

				glVertex2f(x + w, y);
				glVertex2f(x + w, y + fontHeight);

			glEnd();

		} else if (conBlink > CONSOLE_BLINK_TIME)
			conBlink = 0;

		// draw current command
		glColor4f(1,1,1,1);
		drawFontWord(x, y, conCommand);

	}

	// draw console history
	y = conf.screenH - 10 - fontHeight * 2;

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
	conBlink = 0;

	conAdd(0, "Console started...");

}

void conInput(char c) {

	if (c == SDLK_MINUS || c == SDLK_PERIOD || c == SDLK_SPACE || (c >= SDLK_a && c <= SDLK_z) || ( c >= SDLK_0 && c <= SDLK_9 ) ) {

		if (c == SDLK_SPACE && conCommandPos == 0)
			return;

		if (conCommandPos < CONSOLE_LENGTH) {

			conCommand[conCommandPos] = c;
			conCommandPos++;
			conCommand[conCommandPos] = 0;

		}

	}

	if (c == SDLK_BACKSPACE) {

		if (conCommandPos > 0) {

			conCommandPos--;
			conCommand[conCommandPos] = 0;

		}

	}

	if (c == SDLK_RETURN || c == 10) {

		printf("\n\r");

        if (conCommandPos == 0)
			return;		

		cmdExecute(conCommand);

		conCommand[0] = 0;
		conCommandPos = 0;

		view.consoleMode = 0;

	}

	if (c == SDLK_TAB) {

		return;

	}

}

