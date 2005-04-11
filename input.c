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

int processKeys() {

	SDL_Event event;

	while( SDL_PollEvent( &event ) ) {

		if (event.type == SDL_QUIT) {
			cmdQuit(NULL);
			return 1;
		}

		if (event.type == SDL_MOUSEBUTTONDOWN) {
		
			if (view.screenSaver) {
				cmdQuit(0);
				return 1;
			}
			
			if (event.button.button == SDL_BUTTON_WHEELDOWN)
				view.zoom *= (1 + (view.deltaVideoFrame * 0.01f));
		
			if (event.button.button == SDL_BUTTON_WHEELUP)
				view.zoom *= (1 + (view.deltaVideoFrame * -0.01f));

		}

		if (event.type == SDL_KEYUP) {
			view.keys[event.key.keysym.sym] = 0;
			if (view.screenSaver) {
				cmdQuit(0);
				return 1;
			}
		}

		if (event.type == SDL_KEYDOWN) {

			view.keys[event.key.keysym.sym] = 1;

			if (view.consoleMode) {
				conInput((char)event.key.keysym.sym);
				return 0;
			}

			switch(event.key.keysym.sym) {

			case SDLK_ESCAPE:
				if (view.consoleMode) {
					view.consoleMode = 0;
					break;

				} else if (view.textMode == TM_HELP1 || view.textMode == TM_HELP2) {
					view.textMode = TM_STANDARD;
					break;
				}

				cmdQuit(NULL);
				return 1;
				break;

			case SDLK_SPACE:
				view.screenSaver = 0;
				view.drawOSD = 1;
				view.showCursor = 1;
				break;

			case SDLK_RETURN:
				if (SDL_GetModState() & KMOD_LALT || SDL_GetModState() & KMOD_RALT) {
					video.screenFS = !video.screenFS;
					cmdVideoRestart(0);
				}
				break;

			case SDLK_F1:
				if (view.textMode == TM_HELP1)
					view.textMode = TM_STANDARD;
				else
					view.textMode = TM_HELP1;
				break;

			case SDLK_F2:
				if (view.textMode == TM_HELP2)
					view.textMode = TM_STANDARD;
				else
					view.textMode = TM_HELP2;
				break;

			case SDLK_F5:
				cmdPlay(NULL);
				break;

			case SDLK_F6:
				cmdRecord(NULL);
				break;

			case SDLK_F7:
				cmdStop(NULL);
				break;

			case SDLK_F8:
				cmdStart(NULL);
				break;

			case SDLK_F9:
				cmdScreenshot(NULL);
				break;

			case SDLK_F10:
				if (view.screenshotLoop) {
					view.screenshotLoop = 0;
					conAdd(1, "Stopped taking many screenshots! Check your screenshots directory.");
				} else {
					view.screenshotLoop = 1;
 					conAdd(1, "Taking a screenshot every frame. Careful how long you run this!");
				}
				break;
				
			case SDLK_s:
				if (SDL_GetModState() & KMOD_LCTRL || SDL_GetModState() & KMOD_RCTRL)
					cmdSaveFrameDump(NULL);
				break;

			case SDLK_BACKQUOTE:
				view.consoleMode = (!view.consoleMode)?1:0;
				break;

			case SDLK_t:
				if (++view.drawTree == 3)
					view.drawTree = 0;
				conAdd(1, "drawTree set to %i", view.drawTree);
				break;

			case SDLK_SLASH:
				view.particleColourMode ++;
				if (view.particleColourMode == CM_LAST)
					view.particleColourMode = 0;
				conAdd(1, "Colour mode set to: %s" , colourModes[view.particleColourMode]);
				break;

			case SDLK_l:
				view.blendMode ++;
				if (view.blendMode == 5)
					view.blendMode = 0;
				conAdd(1, "Blend mode set to: %i", view.blendMode);
				break;

			case SDLK_BACKSLASH:
				view.particleRenderMode++;
				if (view.particleRenderMode == 2)
					view.particleRenderMode = 0;
				conAdd(1, "particleRenderMode set to %i" , view.particleRenderMode);
				break;

			case SDLK_MINUS:
				view.particleSizeMin /= 2;
				if (view.particleSizeMin < 1)
					view.particleSizeMin = 1;
				conAdd(0, "particlesizemin set to %.1f ", view.particleSizeMin);
				break;

			case SDLK_EQUALS:
				if (view.particleSizeMin < 1)
					view.particleSizeMin = 1;
				else
					view.particleSizeMin *= 2;
				conAdd(0, "particlesizemin set to %.1f ", view.particleSizeMin);
				if (view.particleSizeMin > view.particleSizeMax)
					conAdd(1, "Careful! particlesizemin is bigger then particlesizemax!");
				break;

			case SDLK_LEFTBRACKET:
				view.particleSizeMax /= 2;
				if (view.particleSizeMax < 1)
					view.particleSizeMax = 1;
				conAdd(0, "particlesizemax set to %.1f ", view.particleSizeMax);
				if (view.particleSizeMin > view.particleSizeMax)
					conAdd(1, "Careful! particlesizemin is bigger then particlesizemax!");
				break;

			case SDLK_RIGHTBRACKET:
				if (view.particleSizeMax < 1)
					view.particleSizeMax = 1;
				else
					view.particleSizeMax *= 2;
				conAdd(0, "particlesizemax set to %.1f ", view.particleSizeMax);
				break;

			case SDLK_m:
				if (view.tailLength == -1) {
					view.tailLength = 0;
					conAdd(0, "tailLength set to 0");
				} else {
					view.tailLength = -1;
					conAdd(0, "tailLength set to infinite!");
				}
				break;

			case SDLK_n:
				if (view.tailLength <= 0)
					view.tailLength = 1;
				else
					view.tailLength *= 2;
				conAdd(0, "tailLength set to %i", view.tailLength);
				break;

			case SDLK_b:
				if (view.tailLength < 0)
					view.tailLength = 0;

				if (view.tailLength == 1)
					view.tailLength = 0;
				else
					view.tailLength /= 2;

				conAdd(0, "tailLength set to %i", view.tailLength);
				break;

			case SDLK_v:
				view.tailOpacity += 0.1f;
				if (view.tailOpacity > 1)
					view.tailOpacity = 1;
				conAdd(0, "tailOpacity set to %.1f", view.tailOpacity);
				break;

			case SDLK_c:
				view.tailOpacity -= 0.1f;
				if (view.tailOpacity < 0)
					view.tailOpacity = 0;
				conAdd(0, "tailOpacity set to %.1f", view.tailOpacity);
				break;

			case SDLK_x:
				view.tailFaded = (!view.tailFaded)?1:0;
				conAdd(0, "tailFaded set to %i", view.tailFaded);
				break;

			case SDLK_COMMA:
				view.tailSkip /= 2;
				if (view.tailSkip < 1)
					view.tailSkip = 1;
				conAdd(0, "tailSkip set to %i", view.tailSkip);
				break;

			case SDLK_PERIOD:
				view.tailSkip *= 2;
				conAdd(0, "tailSkip set to %i", view.tailSkip);
				break;

			case SDLK_QUOTE:
				view.tailWidth += 1;
				if (view.tailWidth > 10)
					view.tailWidth = 10;
				conAdd(0, "tailWidth set to %.0f", view.tailWidth);
				break;

			case SDLK_SEMICOLON:
				view.tailWidth -= 1;
				if (view.tailWidth < 1)
					view.tailWidth = 1;
				conAdd(0, "tailWidth set to %.0f", view.tailWidth);
				break;

			case SDLK_o:
				view.drawOSD = (!view.drawOSD)?1:0;
				conAdd(0, "drawOSD set to %i", view.drawOSD);
				break;

			default:
				break;

			}

		}

	}

	if (!view.consoleMode) {

		if (view.keys[SDLK_a])
			view.zoom /= (1 + (view.deltaVideoFrame * 0.01f));

		if (view.keys[SDLK_z])
			view.zoom *= (1 + (view.deltaVideoFrame * 0.01f));
/*
		if (view.keys[SDLK_UP])
			view.face[0] -= state.dt * 0.1f;

		if (view.keys[SDLK_DOWN])
			view.face[0] += state.dt * 0.1f;

		if (view.keys[SDLK_LEFT])
			view.face[1] -= state.dt * 0.1f;

		if (view.keys[SDLK_RIGHT])
			view.face[1] += state.dt * 0.1f;
*/

	}

	return 0;

}

void processMouse() {

	int x,y;
	Uint16 wx,wy;

	view.mouseButtons[1] = view.mouseButtons[0];
	memcpy(view.lastMousePosition, view.currentMousePosition, sizeof(view.currentMousePosition));
	view.mouseButtons[0] = SDL_GetMouseState(&view.currentMousePosition[0], &view.currentMousePosition[1]);
	x = view.currentMousePosition[0] - view.lastMousePosition[0];
	y = view.currentMousePosition[1] - view.lastMousePosition[1];

	// if it's a screensaver, and the mouse has moved, and the simulation has been running for a bit...
	if (view.screenSaver && (x || y) && (getMS() - view.firstTimeStamp > 1000)) {
		cmdQuit(0);
		return;
	}

	if (view.mouseButtons[0]) {
	
		wx = video.screenW / 2;
		wy = video.screenH / 2;
		SDL_WarpMouse(wx,wy);
		view.currentMousePosition[0] = wx;
		view.currentMousePosition[1] = wy;

		// turn off cursor only after the 2nd warp mouse, otherwise showcursor does strange things.
		if (view.mouseButtons[1]) {
			SDL_ShowCursor(0);
		}

		view.rot[1] += x;
		view.rot[0] += y;

	} else {

		SDL_ShowCursor(view.showCursor);

	}

}

#else

int processKeys() {

	int key;

#ifdef WIN32

	while (_kbhit()) {
		key = _getche();
		conInput(key);
		if (key == 13 || key == 10) {
			break;
		}
	}

#else

	char numbytes;
	fd_set rfds;
	struct timeval tv;
	int retval;

	FD_ZERO(&rfds);
	FD_SET(0, &rfds);
	tv.tv_sec = 0;
	tv.tv_usec = 1;

	while ((retval = select(1, &rfds, NULL, NULL, &tv))) {
		
		if (retval) {
			
			numbytes = read(0,&key, 1); // = getchar();
			if(numbytes)
				conInput(key);
		}
	
	}
	
#endif

	return 0;

}

#endif
