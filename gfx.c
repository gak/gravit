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

void drawFrameSet2D() {

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f,conf.screenW,conf.screenH,0,-1.0f,1.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

void drawFrameSet3D() {

	glViewport(0, 0, conf.screenW, conf.screenH);

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	gluPerspective(45.0f,1,0,10000.0f);

}

int gfxInit() {

	int flags;

    if (SDL_Init(SDL_INIT_VIDEO)) {

            dlog(le, "SDL Init failed");
            return 0;

    }

    if (TTF_Init()) {

            dlog(le, "SDL_ttf Init failed");
            return 0;

    }

    SDL_ShowCursor(0);

    dlog(2, "SDL Getting Video Info");
    conf.gfxInfo = (SDL_VideoInfo*) SDL_GetVideoInfo();

    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, conf.screenBPP );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	if (conf.screenAA) {

		SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, 4);

	}

    flags = SDL_OPENGL;

	if (conf.screenFS)
		flags |= SDL_FULLSCREEN;

    if (!SDL_SetVideoMode(conf.screenW, conf.screenH, conf.gfxInfo->vfmt->BitsPerPixel, flags )) {

		dlog(le, "SDL_SetVideoMode failed");
		dlog(le, SDL_GetError());
		return 0;

	}

    glClearColor(0, 0, 0, 0);
    glShadeModel(GL_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);

	if (!loadFonts())
		return 0;

	//if (!loadTexture())
	//	return 0;

	SDL_WM_SetCaption("Gravit", "");

	memset(view.mat1, 0, sizeof(view.mat1));
	memset(view.mat2, 0, sizeof(view.mat2));

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glGetFloatv(GL_MODELVIEW_MATRIX, view.mat1);
	glGetFloatv(GL_MODELVIEW_MATRIX, view.mat2);

	return 1;

}

void drawFrame() {

	particle_t *p;
	particleDetail_t *pd;
	int i,j,k;
	float c;
	float sc[4];

	if (!state.particleHistory)
		return;

	setColors();

	drawFrameSet3D();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
 	gluPerspective(45,(GLfloat)conf.screenW/(GLfloat)conf.screenH,0.1f,10000000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/*
	gluLookAt(

		0, 0, view.zoom,
		0, 0, 0,
		0, 1, 0);
	*/

	glTranslatef(0, 0, -view.zoom);

	glMultMatrixf(view.mat1);
	glMultMatrixf(view.mat2);

	glEnable(GL_BLEND);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	//glBlendFunc( GL_SRC_ALPHA, GL_ONE );
	glDepthMask(GL_TRUE);

	glBindTexture(GL_TEXTURE_2D, 0);

	view.verticies = 0;

	glBegin(GL_POINTS);
	sc[3] = 1;

	for (i = 0; i < state.particleCount; i++) {

		p = state.particleHistory + state.particleCount * state.currentFrame + i;
		pd = state.particleDetail + i;

		glColor4fv(pd->col);

		glVertex3fv(p->pos);

		view.verticies++;

	}
	glEnd();

	if (view.tailLength > 0 || view.tailLength == -1) {

		glLineWidth(view.tailWidth);

		for (i = 0; i < state.particleCount; i++) {

			p = 0;

			glBegin(GL_LINE_STRIP);

			if (view.tailLength == -1)
				k = 0;
			else if (state.currentFrame < (view.tailLength+2))
				k = 0;
			else
				k = state.currentFrame - (view.tailLength+2);

			for (j = k; j < state.currentFrame; j+=view.tailSkip ) {
			//for (j = state.currentFrame; j >= k; j-=view.tailSkip ) {

				if (j >= state.historyFrames)
					continue;

				p = state.particleHistory + state.particleCount * j + i;
				pd = state.particleDetail + i;

//				if (!p->active)
//					continue;

				if (view.tailFaded)
					c = (float)(j-k) / (float)(state.currentFrame-k) * view.tailOpacity;
				else
					c = view.tailOpacity;

				memcpy(sc, pd->col, sizeof(float)*4);
				sc[3] *= c;
				glColor4fv(sc);

				glVertex3fv(p->pos);

				view.verticies++;

			}

			p = state.particleHistory + state.particleCount * state.currentFrame + i;

			glVertex3fv(p->pos);

			glEnd();

		}

	}

}

void drawAxis() {

	drawFrameSet3D();

	glEnable(GL_BLEND);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	glBindTexture(GL_TEXTURE_2D, 0);

	glBegin(GL_QUADS);

		// x plane
		glColor4f(0.8f,0.2f,0.2f,0.1f);
		glVertex3f(0,-100,-100);
		glVertex3f(0,100,-100);
		glVertex3f(0,100,100);
		glVertex3f(0,-100,100);

		// y plane
		glColor4f(0.2f,0.8f,0.2f,0.1f);
		glVertex3f(-100,0,-100);
		glVertex3f(100,0,-100);
		glVertex3f(100,0,100);
		glVertex3f(-100,0,100);

		// z plane
		glColor4f(0.2f,0.2f,0.8f,0.1f);
		glVertex3f(-100,-100,0);
		glVertex3f(100,-100,0);
		glVertex3f(100,100,0);
		glVertex3f(-100,100,0);

	glEnd();

	glBegin(GL_LINES);

		// x plane
		glColor4f(0.8f,0.2f,0.2f,1.0f);
		glVertex3f(-100,0,0);
		glVertex3f(100,0,0);

		// y plane
		glColor4f(0.2f,0.8f,0.2f,1.0f);
		glVertex3f(0,-100,0);
		glVertex3f(0,100,0);

		// z plane
		glColor4f(0.2f,0.2f,0.8f,1.0f);
		glVertex3f(0,0,-100);
		glVertex3f(0,0,100);

	glEnd();

}

void drawRGB() {

	float i;
	float sx = (float)conf.screenW - 60;
	float sy = 10;
	float wx = 50;
	float wy = 200;
	float c[4];
	float step = .01f;

	drawFrameSet2D();

	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glBindTexture(GL_TEXTURE_2D, 0);

	for (i = 0; i <= 1+step; i+=step) {

		gfxNormalToRGB(c, i);

		glBegin(GL_QUADS);
		glColor4fv(c);
		glVertex2f(sx,		sy + wy * i);
		glVertex2f(sx + wx,	sy + wy * i);
		glVertex2f(sx + wx,	sy + wy * (i + step));
		glVertex2f(sx,		sy + wy * (i + step));
		glEnd();

	}


}

void drawAll() {

	glDepthMask(GL_TRUE);

	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawFrame();

	// draws the oct tree
	if (view.drawTree)
		otDrawTree();

//	if (view.drawAxis)
//		drawAxis();

	// drawRGB();

	drawOSD();

	SDL_GL_SwapBuffers();

}

void drawWireCube() {

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);

	glVertex3i(1,1,1); 
	glVertex3i(1,-1,1); 
	glVertex3i(-1,-1,1); 
	glVertex3i(-1,1,1); 

	glVertex3i(1,1,-1);
	glVertex3i(1,-1,-1);
	glVertex3i(-1,-1,-1);
	glVertex3i(-1,1,-1);

	glVertex3i(1,1,1);
	glVertex3i(1,-1,1);
	glVertex3i(1,-1,-1);
	glVertex3i(1,1,-1);

	glVertex3i(-1,1,1);
	glVertex3i(-1,-1,1);
	glVertex3i(-1,-1,-1);
	glVertex3i(-1,1,-1);

	glVertex3i(-1,1,-1);
	glVertex3i(-1,1,1);
	glVertex3i(1,1,1);
	glVertex3i(1,1,-1);

	glVertex3i(-1,-1,-1); 
	glVertex3i(-1,-1,1); 
	glVertex3i(1,-1,1); 
	glVertex3i(1,-1,-1);

	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

#endif
