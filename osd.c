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
#define DUHC() glColor4f(1,1,1,0.5f);
#define NEWLINE() y += fontHeight;
#define HEADING(a) y = drawFontWord(x, y, a); 
#define WHITEHEADING(a) glColor4f(1,1,1,1); HEADING(a);
#define WHITEHEADINGNL(a) NEWLINE(); WHITEHEADING(a);

void drawOSD() {

	float x;
	float y;
	float tab;

	drawFrameSet2D();
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glBindTexture(GL_TEXTURE_2D, 0);

	DUHC();
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

			DUHC();
			DUH("time left", va("~%0.1f minutes", (float)fpsCurrentAverageFT * (state.historyFrames - state.frame) / 1000 / 60));
		
		}

		else if (state.mode & SM_PLAY) {

			glColor4f(0,1,0,.8f);
			y += fontHeight; 
			y = drawFontWord(x, y, "PLAY");

		}

		if (view.screenshotLoop) {

			glColor4f(1,0,0,.8f);
			y += fontHeight * 2; 
			y = drawFontWord(x, y, "AUTO SCREENSHOT");

		}
		
/*
		big = state.particleCount*state.particleCount-state.particleCount;
		DUH("total calcs", va("%-5i %3.2f%% (max: %i)", otGetNCalcs(), (float)otGetNCalcs()/big*100, state.particleCount*state.particleCount-state.particleCount));
		DUH("space calcs", va("%-5i %3.2f%%", otGetSCalcs(), (float)100 * otGetSCalcs() / otGetNCalcs()));
		DUH("particle calcs", va("%-5i %3.2f%%", otGetPCalcs(), (float)100 * otGetPCalcs() / otGetNCalcs()));
*/
		
		

	}

	if (view.textMode >= TM_HELP1) {
		
		WHITEHEADING(va("HELP!", view.textMode - TM_HELP1 + 1));

		NEWLINE();

		DUHC();
		DUH("ESC", "Exit Console/Help/Gravit");
		DUH("F1", "General Shortcut Keys");
		DUH("F2", "Visual Effects Keys");

	}

	if (view.textMode == TM_HELP1) {

		WHITEHEADINGNL("GENERAL SHORTCUT KEYS");

		WHITEHEADINGNL("Recording/Playback");

		DUHC();
		DUH("F5", "play");
		DUH("F6", "record");
		DUH("F7", "pause");
		DUH("F8", "spawn particles");
		DUH("F9", "take one screenshot");
		DUH("F10", "take a screenshot every frame (CAREFUL!)");

		WHITEHEADINGNL("View Controls");

		DUHC();
		DUH("mouse X Y", "rotate");
		DUH("A Z", "zoom");

		WHITEHEADINGNL("Other");

		DUHC();
		DUH("T", "display last oct tree");
		DUH("` (above TAB)", "use the console");

	}

	if (view.textMode == TM_HELP2) {

		WHITEHEADINGNL("VISUAL EFFECTS KEYS");

		WHITEHEADINGNL("Colours");
		
		DUHC();
		DUH("/", "colour mode (mass/vel/acc)");
		DUH("L", "blending mode");

		WHITEHEADINGNL("Particles");
		
		DUHC();
		DUH("\\", "particle render mode (circle/texture)");
		DUH("= -", "min particle size");
		DUH("[ ]", "max particle size");

		WHITEHEADINGNL("Particle Tail");

		DUHC();
		DUH("X", "tail fade");
		DUH("C V", "tail opacity");
		DUH("B N", "tail length less/more");
		DUH("M", "tail length infinite/none");
		DUH(", (comma) . (period)", "tail resolution");
		DUH("; (semicolon) ' (quote)", "tail width");

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
