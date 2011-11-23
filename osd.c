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
    glEnable(GL_BLEND);
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glBindTexture(GL_TEXTURE_2D, 0);

    // Top middle
    glColor4f(1,1,1,1);
    y = 10;
    drawFontWordCA(video.screenW / 2, y, "Hit SPACE to start a new simulation");
    drawFontWordCA(video.screenW / 2, y += fontHeight, "Hold down a mouse button and move it around to change your orientation.");
    drawFontWordCA(video.screenW / 2, y += fontHeight, "Use the scroll wheel, or the A and Z keys to zoom in and out.");
    
    // Right
    DUHC();
    drawFontWordRA((float)video.screenW - 10, (float)video.screenH - 10 - fontHeight * 1.0f, "press F1 for help");
    drawFontWordRA((float)video.screenW - 10, (float)video.screenH - 10 - fontHeight * 3.0f, GRAVIT_VERSION);
    drawFontWordRA((float)video.screenW - 10, (float)video.screenH - 10 - fontHeight * 2.0f, GRAVIT_COPYRIGHT);

    // Left
    y = 10;
    x = 10;
    tab = getWordWidth("M") * 17;

    if (view.textMode == TM_STANDARD) {

        if (state.fileName) {
            DUH("simulation name", state.fileName);
        } else {
            DUH("simulation name", "-");
        }


        DUH("particles", va("%i", state.particleCount));
        DUH("avg video fps", va("%3.2f", fpsCurrentAverageFPS));
        DUH("avg video frame time", va("%.0fms", fpsCurrentAverageFT));
        DUH("last record frame time", va("%ims", view.deltaRecordFrame));

        DUH("actual frames", va("%i", state.totalFrames));
        DUH("recording skip", va("%i", state.historyNFrame));
        if (view.frameSkip && state.mode & SM_PLAY) {
            DUH("display frame", va("%i (%i)", state.currentFrame, view.frameSkip));
        } else {
            DUH("display frame", va("%i", state.currentFrame));
        }
        DUH("recorded frames", va("%i", state.frame));
        DUH("max frames", va("%i", state.historyFrames));
        DUH("particle vertices", va("%i", view.vertices));
        DUH("tree nodes allocated", va("%i", view.recordNodes));
        DUH("memory allocated", va("%.1fmb", (float)state.memoryAllocated / 1024 / 1024));

        if (state.mode & SM_RECORD) {

            glColor4f(1,0,0,.8f);
            y += fontHeight;
            y = drawFontWord(x, y, "RECORDING");

            DUHC();
            DUH("time left", va("~%0.1f minutes", (float)view.deltaRecordFrame * (state.historyFrames - state.frame) / 1000 / 60));

            switch (view.recordStatus) {
            case 0:
            default:
                DUH("status", "dormant");
                break;
            case 1:
                DUH("status", va("generating tree %.1f%%", (float)view.recordParticlesDone/state.particleCount*100));
                break;
            case 2:
                DUH("status", va("applying forces %.1f%%", (float)view.recordParticlesDone/state.particleCount*100));
                break;
            case 3:
                DUH("status", "freeing tree");
                break;
            }

        }

        else if (state.mode & SM_PLAY) {

            glColor4f(0,1,0,.8f);
            y += fontHeight;
            y = drawFontWord(x, y, "PLAY");

        } else if (state.currentlySpawning) {

            glColor4f(1,1,0,.8f);
            y += fontHeight;
            y = drawFontWord(x, y, "SPAWNING");

            DUHC();
            DUH("status", va("%.1f%%", (float)view.recordParticlesDone/state.particleCount*100));

        }

        if (view.screenshotLoop) {

            glColor4f(1,0,0,.8f);
            y += fontHeight * 2;
            y = drawFontWord(x, y, "AUTO SCREENSHOT");

        }


        // show renderer FPS average (if we have meaningful values)
        if ((view.timed_frames > 1) && (view.totalRenderTime > SDL_TIMESLICE )) {
            if (state.mode & SM_RECORD) {
	      glColor4f(0,1,1,.5f); /*torquise*/ }
              else { DUHC(); y += fontHeight; }

	    DUH("avg renderer fps", va("%5.2f",
                     (float) view.timed_frames/ (float) view.totalRenderTime * 1000.0f ))
	    DUH("avg renderer frame time", va("%4.1f ms",
		     (float) view.totalRenderTime / (float) view.timed_frames ));
	    DUHC();
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
        DUH("F1", "General Shortcut Keys");
        DUH("F2", "Visual Effects Keys");
        DUH("` (above TAB) or 1", "Use the Console");
        DUH("ESC", "Exit Console/Help/Gravit");

    }

    if (view.textMode == TM_HELP1) {

        WHITEHEADINGNL("GENERAL SHORTCUT KEYS");

        WHITEHEADINGNL("Recording/Playback");
        DUHC();
        DUH("SPACE", "spawn particles");
        DUH("F5", "replay");
        DUH("F6", "record");
        DUH("F7", "pause");
        DUH("Q W", "decrease / increase playback speed");
        DUH("CTRL + S", "quick save");

        WHITEHEADINGNL("View Controls");
        DUHC();
        DUH("mouse X Y + button", "rotate (with mouse button)");
        DUH("A Z", "zoom");

        WHITEHEADINGNL("Stereoscopy");
        DUHC();
        DUH("S", "toggle stereo mode");
        DUH("D F", "decrease / increase stereoseparation");

        WHITEHEADINGNL("Other");
        DUHC();
        DUH("T", "display current oct tree");
        DUH("O", "toggles drawing text");
        DUH("F9", "take one screenshot");
        DUH("F10", "take a screenshot every frame (CAREFUL!)");

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

}

#endif
