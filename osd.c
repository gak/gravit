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

#define DUH(a,b) drawFontWord(x, y, a); y = drawFontWord(tab, y, b);

void drawOSD() {

	float x;
	float y;
	float tab;

	drawFrameSet2D();
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glBindTexture(GL_TEXTURE_2D, 0);

	glColor4f(1,1,1,0.5f);

	drawFontWordRA((float)conf.screenW - 10, (float)conf.screenH - 10 - fontHeight * 1.0f, "press F1 for help");
	drawFontWordRA((float)conf.screenW - 10, (float)conf.screenH - 10 - fontHeight * 3.0f, GRAVIT_VERSION);
	drawFontWordRA((float)conf.screenW - 10, (float)conf.screenH - 10 - fontHeight * 2.0f, GRAVIT_COPYRIGHT);

	y = 10;
	x = 10;
	tab = getWordWidth("M") * 17;

	if (view.textMode == TM_STANDARD) {

		DUH("particles", va("%i", state.particleCount));
		DUH("avg fps", va("%3.2f", fpsCurrentAverageFPS));
		DUH("avg frametime", va("%.0fms", fpsCurrentAverageFT));

		DUH("actual frames", va("%i", state.totalFrames));
		DUH("recording skip", va("%i", state.historyNFrame));
		DUH("display frame", va("%i", state.currentFrame));
		DUH("recorded frames", va("%i", state.frame));
		DUH("max frames", va("%i", state.historyFrames));
		DUH("particle verticies", va("%i", view.verticies));
		DUH("memory allocated", va("%.1fmb", (float)state.memoryAllocated / 1024 / 1024));

		if (state.mode & SM_RECORD) {
			
			glColor4f(1,0,0,.8f);
			y += fontHeight; 
			y = drawFontWord(x, y, "RECORDING");

			glColor4f(1,1,1,0.5f);
			DUH("time left", va("~%0.1f minutes", (float)fpsCurrentAverageFT * (state.historyFrames - state.frame) / 1000 / 60));
		
		}

		else if (state.mode & SM_PLAY) {

			glColor4f(0,1,0,.8f);
			y += fontHeight; 
			y = drawFontWord(x, y, "PLAY");

		}
		
/*
		big = state.particleCount*state.particleCount-state.particleCount;
		DUH("total calcs", va("%-5i %3.2f%% (max: %i)", otGetNCalcs(), (float)otGetNCalcs()/big*100, state.particleCount*state.particleCount-state.particleCount));
		DUH("space calcs", va("%-5i %3.2f%%", otGetSCalcs(), (float)100 * otGetSCalcs() / otGetNCalcs()));
		DUH("particle calcs", va("%-5i %3.2f%%", otGetPCalcs(), (float)100 * otGetPCalcs() / otGetNCalcs()));
*/
		
		

	}

	if (view.textMode == TM_HELP) {

		glColor4f(1,1,1,1);
		y = drawFontWord(x, y, "SHORTCUT KEYS");

		glColor4f(1,1,1,1);
		y += fontHeight;
		y = drawFontWord(x, y, "Recording/Playback");

		glColor4f(1,1,1,0.5f);
		DUH("F2", "start");
		DUH("F3", "play");
		DUH("F4", "pause");
		DUH("F5", "stop");
		DUH("F6", "record");
		DUH("F10", "go to frame 0");

		glColor4f(1,1,1,1);
		y += fontHeight;
		y = drawFontWord(x, y, "View");

		glColor4f(1,1,1,0.5f);
		DUH("mouse x y", "rotate");
		DUH("a z", "zoom");

		glColor4f(1,1,1,1);
		y += fontHeight;
		y = drawFontWord(x, y, "Particle Tail");

		glColor4f(1,1,1,0.5f);
		DUH("x", "fade particle tail");
		DUH("c v", "particle tail opacity");
		DUH("b n m", "particle tail length less/more/all");
		DUH(", .", "particle tail resolution");
		DUH("; '", "particle tail width");

		glColor4f(1,1,1,1);
		y += fontHeight;
		y = drawFontWord(x, y, "Console");

		glColor4f(1,1,1,0.5f);
		DUH("`", "toggle console");

	}

	conDraw();
	
#if 0
	{

		float i;
		float cols[3];
		char z[2] = {0,0};

		for (i = 0; i < 1; i += 0.01f) {
	
			gfxNormalToRGB(cols, i);
			glColor3fv(cols);
			z[0] = 65 + (int)(i * 26);
			drawFontWord(i * conf.screenW, 0, z);

		}

	}
#endif 
}

#endif
