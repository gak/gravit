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
Foundation, 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

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

#ifdef WITHOUT_AGAR
    // Top middle
    y = 10;
    glColor4f(0,.6,.8,.6f);
    drawFontWordCA(video.screenW / 2, y, "Hit SPACE to start a new simulation");
    drawFontWordCA(video.screenW / 2, y += fontHeight, "Use arrow keys, or hold down a mouse button and move it around to change your orientation.");
    drawFontWordCA(video.screenW / 2, y += fontHeight, "Use the scroll wheel, or the A and Z keys to zoom in and out.");
    glColor4f(1,1,1,.5f);
    drawFontWordCA(video.screenW / 2, y += fontHeight, "press F1 for keyboard commands help.");
#endif

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

        if (view.lastVideoFrameSkip == 0) {
            DUH("avg video fps", va("%3.2f", fpsCurrentAverageFPS));
        } else {
            DUH("avg video fps", va("%3.2f  (* %ld)", fpsCurrentAverageFPS, (long)view.lastVideoFrameSkip + 1));
        }

        if ((view.drawOSD == 1) || (view.consoleMode)) {

        // DUH("avg video frame time", va("%.0fms", fpsCurrentAverageFT));
        // DUH("last record frame time", va("%ims", view.deltaRecordFrame));

        DUH("actual frames", va("%i", state.totalFrames));
        DUH("recording skip", va("%i", state.historyNFrame));
        if (view.frameSkip && state.mode & SM_PLAY) {
            DUH("display frame", va("%i (%i)", state.currentFrame, view.frameSkip));
        } else {
            DUH("display frame", va("%i", state.currentFrame));
        }

        NEWLINE();
        DUH("recorded frames", va("%i", state.frame));
        DUH("max frames", va("%i", state.historyFrames));
        DUH("particle vertices", va("%i", view.vertices));
        
#if NBODY_METHOD == METHOD_OT
        DUH("tree nodes allocated", va("%i", view.recordNodes));
#endif
        
        DUH("memory allocated", va("%.1fmb", (float)state.memoryAllocated / 1024 / 1024));
        }

        if ((view.drawOSD == 1) || (view.consoleMode)) {
            switch(state.physics) {
                case PH_PROPER:   DUH("physics", "Newtonian"); break;
                case PH_MODIFIED: DUH("physics", "modified Newtonian"); break;
                case PH_CLASSIC:  DUH("physics", "Gravit classic"); break;
            }
        }

        if (state.mode & SM_RECORD) {

            glColor4f(1,0,0,.8f);
            y += fontHeight;
            y = drawFontWord(x, y, "RECORDING");

            DUHC();
            DUH("time left", va("~%0.1f minutes", (float)view.deltaRecordFrame * (state.historyFrames - state.frame) / 1000 / 60));

            if ((view.drawOSD == 1) || (view.consoleMode)) {

            switch (view.recordStatus) {
            case 0:
            default:
	        if (state.mode & SM_RECORD) {
		    DUH("status", "rendering ...");
                } else {
                    DUH("status", "dormant"); }
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
            }}

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
            

        // XXX: This might be too many stats. I'll implement this in the ajax windowing screens.
	    DUH("avg renderer fps", va("%5.2f",
                     (float) view.timed_frames/ (float) view.totalRenderTime * 1000.0f ));
	/*
	    DUH("avg renderer frame time", va("%4.1f ms",
		     (float) view.totalRenderTime / (float) view.timed_frames ));
        */
            
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
        DUH("P", "toggles drawing Sky background");
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

#ifndef WITHOUT_AGAR
// Handlers

void osdHandleQuit(AG_Event *event) {
    cmdQuit(0);
}

void osdHandleRespawn(AG_Event *event) {
    cmdSpawn(0);
    osdUpdate();
}

void osdHandleRecord(AG_Event *event) {
    cmdRecord(0);
    osdUpdate();
}

void osdHandlePlay(AG_Event *event) {
    cmdPlay(0);
    osdUpdate();
}

void osdHandlePrev(AG_Event *event) {
    if (state.currentFrame > 0)
        state.currentFrame--;
    cmdStop(0);
    osdUpdate();
}

void osdHandleNext(AG_Event *event) {
    if (state.currentFrame < state.totalFrames - 1)
        state.currentFrame++;
    cmdStop(0);
    osdUpdate();
}

void osdHandleFirst(AG_Event *event) {
    state.currentFrame = 0;
    if (state.mode & SM_RECORD) {
        cmdStop(0);
    }
    osdUpdate();
}
void osdHandleLast(AG_Event *event) {
    state.currentFrame = state.totalFrames - 1;
    cmdStop(0);
    osdUpdate();
}

// Styling

void osdCheckbox(void *cbox, int state, int size) {
    
    AG_Rect r = AG_RECT(0, 0, size, size);    
    AG_DrawBox(cbox, r, 1, agColors[CHECKBOX_COLOR]);

    if (state) {

        float a = 3;
        r.x += a; r.y += a; r.w -= a * 2; r.h -= a * 2;
        AG_DrawBox(cbox, r, 1, AG_ColorRGB(255, 255, 255));
        
    }
}

void osdInitStyle()
{
    AG_ColorsSetRGBA(WINDOW_BG_COLOR, 0, 0, 0, 200);
    AG_ColorsSetRGBA(TITLEBAR_FOCUSED_COLOR, 0, 128, 171, 250);
    AG_ColorsSetRGBA(TITLEBAR_UNFOCUSED_COLOR, 0, 128, 171, 200);
    AG_ColorsSetRGBA(BUTTON_COLOR, 0, 64, 85, 255);    
    
	view.osdStyle = agStyleDefault;
    view.osdStyle.CheckboxButton = osdCheckbox;
    AG_SetStyle(agDriverSw, &view.osdStyle);
    
}

// Helpers

AG_Window *osdNewWindow(const char *title) {
    AG_Window *w = AG_WindowNew(AG_WINDOW_NOMAXIMIZE | AG_WINDOW_NOMINIMIZE | AG_WINDOW_NORESIZE);
    AG_WindowSetCaptionS(w, title);
    AG_WindowSetPadding(w, 8, 8, 8, 8);
    AG_WindowSetSideBorders(w, 1);
    AG_WindowSetBottomBorder(w, 1);
    return w;
}

void osdInitPlaybackWindow() {
    AG_Box *box;
    view.playbackWindow = osdNewWindow("Quick Controls");
    box = AG_BoxNewHoriz(view.playbackWindow, AG_BOX_EXPAND);
    
    // The labels for Record and Pause are the "longest legnth" versions of the
    // state of the button, so that no button resizing is necessary.
    
    AG_ButtonNewFn(box, 0, "Respawn", osdHandleRespawn, 0);
    
    AG_SpacerNewVert(box);
    
    view.recordButton = AG_ButtonNewFn(box, 0, "Record", osdHandleRecord, 0);
    AG_ButtonNewFn(box, 0, "<<", osdHandleFirst, 0);
    AG_ButtonNewFn(box, AG_BUTTON_REPEAT, "<", osdHandlePrev, 0);
    view.playButton = AG_ButtonNewFn(box, 0, "Pause", osdHandlePlay, 0);
    AG_ButtonNewFn(box, AG_BUTTON_REPEAT, ">", osdHandleNext, 0);
    AG_ButtonNewFn(box, 0, ">>", osdHandleLast, 0);
    
    AG_SpacerNewVert(box);
    
    AG_ButtonNewFn(box, 0, "Quit", osdHandleQuit, 0);
    
    AG_WindowSetPosition(view.playbackWindow, AG_WINDOW_TR, 0);
    AG_WindowShow(view.playbackWindow);
    osdUpdate();
}

void osdInitIntroWindow() {
    AG_Window *w = osdNewWindow("Welcome to Gravit!");
    
    AG_Box *vBox = AG_BoxNewVert(w, 0);
    AG_Box *hBox = AG_BoxNewHoriz(w, 0);
    AG_Label *text;
    AG_BoxSetSpacing(vBox, 10);
    
    text = AG_LabelNew(vBox, 0, 0);
    AG_LabelText(text, "Gravit is a free, visually stunning gravity simulator, where you can spend endless\ntime experimenting with various configurations of simulated universes.\n\nQuick Start:\n\n - Click on RESPAWN to start a new simulation.\n - Click on PLAY to replay a recording\n - Click on RECORD to resume recording\n - Use arrow keys, or hold down a mouse button and move it around to change your perspective.\n - Use the A and Z keys, or the scroll wheel to zoom in and out.\n");
    AG_WidgetSetSize(text, 200, 100);
    
    // AG_Checkbox *showAgain = AG_CheckboxNew(vBox, AG_CHECKBOX_SET, "Show this window on startup");
    AG_ButtonNewFn(hBox, 0, "  OK  ", AG_WindowCloseGenEv, "%p", w);
    
    AG_WindowShow(w);
}

void osdUpdate() {
    
    if (state.mode & SM_PLAY) {
        AG_ButtonTextS(view.playButton, "Pause");
    } else {
        AG_ButtonTextS(view.playButton, "Play");
    }
    
    if (state.mode & SM_RECORD) {
        AG_ButtonTextS(view.recordButton, "Pause");
    } else {
        AG_ButtonTextS(view.recordButton, "Record");
    }
    
}

void osdInitDefaultWindows() {

    osdInitStyle();
    osdInitIntroWindow();
    osdInitPlaybackWindow();
    
}

#endif

#endif
