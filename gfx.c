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

GLuint particleTextureID;

void drawFrameSet2D() {

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f,video.screenW,video.screenH,0,-1.0f,1.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

void drawFrameSet3D() {

    glViewport(0, 0, video.screenW, video.screenH);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    gluPerspective(45.0f,1,0,10000.0f);

}

int loadParticleTexture() {

    SDL_Surface *particleSurface;

    if (!fileExists(MISCDIR "/particle.png"))
        return 0;

    particleSurface = IMG_Load(MISCDIR "/particle.png");
//    SDL_SaveBMP(particleSurface, "test.bmp");
//    particleSurface = SDL_LoadBMP("test.bmp");

    glGenTextures(1, &particleTextureID);
    glCheck();

    glBindTexture(GL_TEXTURE_2D, particleTextureID);
    glCheck();

    gluBuild2DMipmaps(GL_TEXTURE_2D, 4, particleSurface->w, particleSurface->h, GL_RGBA, GL_UNSIGNED_BYTE, particleSurface->pixels);
    glCheck();

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
    glCheck();

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glCheck();

    SDL_FreeSurface(particleSurface);

    return 1;

}

int gfxSetResolution() {

    video.screenW = video.screenWtoApply;
    video.screenH = video.screenHtoApply;

    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

    if (video.screenAA) {

        SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 1);
        SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, 4);

    }

    video.flags = SDL_OPENGL;

    if (video.screenFS)
        video.flags |= SDL_FULLSCREEN;

    if (!SDL_SetVideoMode(video.screenW, video.screenH, video.screenBPP, video.flags )) {
        conAdd(LERR, "SDL_SetVideoMode failed: %s", SDL_GetError());
        return 1;
    }

    glEnable(GL_TEXTURE_2D);

    // need to (re)load textures
    if (!loadFonts())
        return 2;

    if (!loadParticleTexture())
        return 3;

    return 0;

}

int gfxInit() {

    int detectedBPP;
    SDL_Surface *icon;
    int ret;

    if (SDL_Init(SDL_INIT_VIDEO)) {

        conAdd(LERR, "SDL Init failed");
        conAdd(LERR, SDL_GetError());
        sdlCheck();
        return 0;

    }

    if (TTF_Init()) {

        conAdd(LERR, "SDL_ttf Init failed");
        conAdd(LERR, SDL_GetError());
        sdlCheck();
        return 0;

    }

    video.sdlStarted = 1;

    icon = IMG_Load(MISCDIR "/gravit.png");
    if (!icon) {
        sdlCheck();
    }
    SDL_WM_SetIcon(icon, NULL);
    SDL_FreeSurface(icon);

    setTitle(0);

    video.gfxInfo = (SDL_VideoInfo*) SDL_GetVideoInfo();
    detectedBPP = video.gfxInfo->vfmt->BitsPerPixel;

    conAdd(LLOW, "Detected %i BPP", detectedBPP);

gfxInitRetry:

    if (video.screenBPP == 0)
        video.screenBPP = detectedBPP;

    ret = gfxSetResolution();
    if (ret) {

        if (ret == 1) {

            if (video.screenAA) {
                conAdd(LERR, "You have videoantialiasing on. I'm turning it off and restarting...");
                video.screenAA = 0;
                goto gfxInitRetry;
            }

            if (detectedBPP != video.screenBPP) {
                conAdd(LERR, "Your BPP setting is different to your desktop BPP. I'm restarting with your desktop BPP...");
                video.screenBPP = detectedBPP;
                goto gfxInitRetry;
            }

        }

        return 0;

    }

    conAdd(LLOW, "Your video mode is %ix%ix%i", video.screenW, video.screenH, video.gfxInfo->vfmt->BitsPerPixel );

    if (!video.screenAA && view.particleRenderMode == 1) {
        conAdd(LERR, "Warning! You don't have videoantialiasing set to 1. From what I've seen so far");
        conAdd(LERR, "this might cause particlerendermode 1 not to work. If you don't see any particles");
        conAdd(LERR, "after spawning, hit the \\ (backslash) key).");
    }

    glClearColor(0, 0, 0, 0);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glDisable(GL_DEPTH_TEST);

    checkPointParameters();
    checkPointSprite();

    SDL_ShowCursor(view.showCursor);
    SDL_EnableUNICODE(1);
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,SDL_DEFAULT_REPEAT_INTERVAL);

    return 1;

}

void particleInterpolate(int i, float t, float *v) {

    particle_t *p1;
    particle_t *p2;
    VectorNew(moo);

    p1 = state.particleHistory + state.particleCount * state.currentFrame + i;

    if (state.currentFrame == state.historyFrames || state.historyFrames == 0 || state.mode & SM_RECORD) {
        VectorCopy(p1->pos, v);
        return;
    }

    p2 = state.particleHistory + state.particleCount * (state.currentFrame+1) + i;

    VectorSub(p1->pos, p2->pos, moo);
    VectorMultiply(moo, t, moo);
    VectorAdd(p1->pos, moo, v);

}

void drawFrame() {

    particle_t *p;
    particleDetail_t *pd;
    int i,j,k;
    float c;
    float sc[4];

    if (!state.particleHistory)
        return;

    switch (view.blendMode) {

    case 0:
        glDisable(GL_BLEND);
        break;
    case 1:
        glEnable(GL_BLEND);
        glBlendFunc( GL_SRC_ALPHA, GL_ONE );
        break;
    case 2:
        glEnable(GL_BLEND);
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        break;
    case 3:
        glEnable(GL_BLEND);
        glBlendFunc( GL_SRC_ALPHA_SATURATE, GL_ONE );
        break;
    case 4:
        glEnable(GL_BLEND);
        glBlendFunc( GL_SRC_ALPHA_SATURATE, GL_ONE_MINUS_SRC_ALPHA );
        break;

    }

    if (view.particleRenderMode == 0) {

        float pointRange[2];

        glDisable(GL_DEPTH_TEST);
        glBindTexture(GL_TEXTURE_2D, 0);
        glEnable(GL_POINT_SMOOTH);

        glGetFloatv(GL_POINT_SIZE_RANGE, pointRange);

        if (view.particleSizeMin < pointRange[0]) {
            view.particleSizeMin = pointRange[0];
            conAdd(LNORM, "Point Size has reached its minimum of %f", view.particleSizeMin);
        }

        if (view.particleSizeMin > pointRange[1]) {
            view.particleSizeMin = pointRange[1];
            conAdd(LNORM, "Point Size has reached its maximum of %f", view.particleSizeMin);
        }

        glPointSize(view.particleSizeMin);

    }

    if (view.particleRenderMode == 1) {

        float quadratic[] =  { 0.0f, 0.0f, 0.01f };

        if (!video.supportPointParameters || !video.supportPointSprite) {

            conAdd(LNORM, "Sorry, Your video card does not support GL_ARB_point_parameters and/or GL_ARB_point_sprite.");
            conAdd(LNORM, "This means you can't have really pretty looking particles.");
            conAdd(LNORM, "Setting particleRenderMode to 2");
            view.particleRenderMode = 2;
            return;

        }

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_POINT_SMOOTH);	// enabling this makes particles dissapear

        glPointParameterfvARB_ptr( GL_POINT_DISTANCE_ATTENUATION_ARB, quadratic );

        glPointParameterfARB_ptr( GL_POINT_SIZE_MAX_ARB, view.particleSizeMax );
        glPointParameterfARB_ptr( GL_POINT_SIZE_MIN_ARB, view.particleSizeMin );

        glPointSize( view.particleSizeMax );

        // lets you put textures on the sprite
        // doesn't work on some cards for some reason :(
        // so i had to make textures an option with this mode
        if (view.particleRenderTexture) {
            glTexEnvf( GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE );
            glBindTexture(GL_TEXTURE_2D, particleTextureID);
        } else {
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        glEnable( GL_POINT_SPRITE_ARB );

    } else if (view.particleRenderMode == 2) {

        glDisable(GL_DEPTH_TEST);
        if (view.particleRenderTexture) {
            glBindTexture(GL_TEXTURE_2D, particleTextureID);
        } else {
            glBindTexture(GL_TEXTURE_2D, 0);
        }

    }

    if (view.particleRenderMode == 0 || view.particleRenderMode == 1) {

        glBegin(GL_POINTS);
        for (i = 0; i < state.particleCount; i++) {

            VectorNew(pos);

            pd = state.particleDetail + i;
            glColor4fv(pd->col);
            if (view.frameSkip < 0) {
                particleInterpolate(i, ((float)view.frameSkipCounter / view.frameSkip), pos);
                glVertex3fv(pos);
            } else {
                p = state.particleHistory + state.particleCount * state.currentFrame + i;
                glVertex3fv(p->pos);
            }
            view.vertices++;

        }
        glEnd();

    } else if (view.particleRenderMode == 2) {

        // my math mojo is not so great, so this may not be the most efficient way of doing this
        // this whole bit is dodgy too, as long as it works :)

        GLdouble matProject[16];
        GLdouble matModelView[16];
        GLint viewport[4];
        GLdouble screen[3];

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glCheck();
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glCheck();

        glGetDoublev(GL_PROJECTION_MATRIX, matProject);
        glCheck();
        glGetDoublev(GL_MODELVIEW_MATRIX, matModelView);
        glCheck();
        glGetIntegerv(GL_VIEWPORT, viewport);
        glCheck();

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        if (view.stereoMode) {
            if (view.stereoModeCurrentBit == 0) {
                glOrtho(0.0,video.screenW/2,0,video.screenH,-1.0,1.0);
            } else {
                glOrtho(video.screenW/2,video.screenW,0,video.screenH,-1.0,1.0);
            }
        } else {
            glOrtho(0.0f,video.screenW,0,video.screenH,-1.0f,1.0f);
        }

        glCheck();

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glCheck();

        for (i = 0; i < state.particleCount; i++) {

            double size;
            VectorNew(moo);
            float *pos;
            pos = moo;

            pd = state.particleDetail + i;

            if (view.frameSkip < 0) {
                particleInterpolate(i, ((float)view.frameSkipCounter / view.frameSkip), moo);
            } else {
                p = state.particleHistory + state.particleCount * state.currentFrame + i;
                pos = p->pos;
            }

            gluProject(
                pos[0],pos[1],pos[2],
                matModelView, matProject, viewport,
                &screen[0], &screen[1], &screen[2]
            );

            if (screen[2] > 1)
                continue;

            size = view.particleSizeMin + (1.f - (float)screen[2]) * view.particleSizeMax;

            glBegin(GL_QUADS);
            glColor4fv(pd->col);
            glTexCoord2i(0,0);
            glVertex2d(screen[0]-size, screen[1]-size);
            glTexCoord2i(1,0);
            glVertex2d(screen[0]+size, screen[1]-size);
            glTexCoord2i(1,1);
            glVertex2d(screen[0]+size, screen[1]+size);
            glTexCoord2i(0,1);
            glVertex2d(screen[0]-size, screen[1]+size);
            glEnd();

            view.vertices+=4;

        }

        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();

    }

    if (view.particleRenderMode == 1 && video.supportPointParameters && video.supportPointSprite) {

        glDisable( GL_POINT_SPRITE_ARB );

    }

    glBindTexture(GL_TEXTURE_2D, 0);
//	sc[3] = 1;

    if (view.tailLength > 0 || view.tailLength == -1) {

        // Not sure why this helps but,
        // it is a fix for one case where only points are drawn instead of lines
        // on radeon 9600 after switching to particlerendermode 0 from 1
        if (view.particleRenderMode == 0)
            glLineWidth(view.tailWidth+1);

        glLineWidth(view.tailWidth);

        for (i = 0; i < state.particleCount; i++) {

            int to;
            p = 0;

            glBegin(GL_LINE_STRIP);

            if (view.tailLength == -1)
                k = 0;
            else if (state.currentFrame < (view.tailLength+2))
                k = 0;
            else
                k = state.currentFrame - (view.tailLength+2);

            if (state.mode & SM_RECORD)
                to = state.currentFrame;
            else
                to = state.currentFrame + 1;

            for (j = k; j <= state.currentFrame; j+=view.tailSkip ) {
                //for (j = state.currentFrame; j >= k; j-=view.tailSkip ) {

                if (j >= state.historyFrames)
                    continue;

                pd = state.particleDetail + i;

                if (view.tailFaded)
                    c = (float)(j-k) / (float)(state.currentFrame-k) * view.tailOpacity;
                else
                    c = view.tailOpacity;

                memcpy(sc, pd->col, sizeof(float)*4);
                sc[3] *= c;
                glColor4fv(sc);

                p = state.particleHistory + state.particleCount * j + i;
                glVertex3fv(p->pos);

                view.vertices++;

            }

            if (view.frameSkip < 0 && !(state.mode & SM_RECORD)) {
                VectorNew(pos);
                particleInterpolate(i, ((float)view.frameSkipCounter / view.frameSkip), pos);
                glVertex3fv(pos);
            } else {
                p = state.particleHistory + state.particleCount * state.currentFrame + i;
                glVertex3fv(p->pos);
            }

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

    float width = 5;
    float margin = 5;
    float i;
    float sx = (float)video.screenW - width - margin;
    float sy = margin;
    float wx = width;
    float wy = 200;
    float c[4];
    float step = .01f;

    drawFrameSet2D();

    glEnable(GL_BLEND);
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glBindTexture(GL_TEXTURE_2D, 0);

    // positive
    for (i = 0; i < 1; i+=step) {

        colourFromNormal(c, i);

        glBegin(GL_QUADS);
        glColor4fv(c);
        glVertex2f(sx,		sy + wy * i);
        glVertex2f(sx + wx,	sy + wy * i);
        glVertex2f(sx + wx,	sy + wy * (i + step));
        glVertex2f(sx,		sy + wy * (i + step));
        glEnd();

    }

    glColor3f(0.5f,0.5f,0.5f);
    glLineWidth(1.0f);
    glDisable(GL_LINE_SMOOTH);
    glBegin(GL_LINE_STRIP);
    glVertex2f(sx-1,		sy-1);
    glVertex2f(sx+1 + wx,	sy-1);
    glVertex2f(sx+1 + wx,	sy+2 + wy);
    glVertex2f(sx-1,		sy+2 + wy);
    glVertex2f(sx-1,		sy-1);
    glEnd();
    glEnable(GL_LINE_SMOOTH);

    // negative
    if (1) {

        sx -= margin * 1.5;

        for (i = 0; i  < 1; i+=step) {

            colourFromNormal(c, i);

            c[0] = 1 - c[0];
            c[1] = 1 - c[1];
            c[2] = 1 - c[2];

            glBegin(GL_QUADS);
            glColor4fv(c);
            glVertex2f(sx,		sy + wy * i);
            glVertex2f(sx + wx,	sy + wy * i);
            glVertex2f(sx + wx,	sy + wy * (i + step));
            glVertex2f(sx,		sy + wy * (i + step));
            glEnd();

        }

        glColor3f(0.5f,0.5f,0.5f);
        glLineWidth(1.0f);
        glDisable(GL_LINE_SMOOTH);
        glBegin(GL_LINE_STRIP);
        glVertex2f(sx-1,		sy-1);
        glVertex2f(sx+1 + wx,	sy-1);
        glVertex2f(sx+1 + wx,	sy+2 + wy);
        glVertex2f(sx-1,		sy+2 + wy);
        glVertex2f(sx-1,		sy-1);
        glEnd();
        glEnable(GL_LINE_SMOOTH);


    }

}

void translateToCenter() {

    int i;
    particle_t *p;
    VectorNew(pos);

    VectorZero(pos);

    for (i = 0; i < state.particleCount; i++) {

        p = getParticleCurrentFrame(i);
        VectorAdd(pos, p->pos, pos);

    }

    VectorDivide(pos, state.particleCount, pos);
    glTranslatef(-pos[0], -pos[1], -pos[2]);

}

void drawAll() {

//	int i;
    int bits;
    VectorNew(rotateIncrement);

    glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    view.vertices = 0;

    if (view.stereoMode)
        bits = 2;
    else
        bits = 1;

    for (view.stereoModeCurrentBit = 0; view.stereoModeCurrentBit < bits; view.stereoModeCurrentBit++) {

        glViewport(video.screenW/bits*view.stereoModeCurrentBit, 0, video.screenW/bits, video.screenH);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45,(GLfloat)video.screenW/bits/(GLfloat)video.screenH,0.1f,10000000.0f);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        VectorMultiply(view.autoRotate, view.deltaVideoFrame, rotateIncrement);
        VectorAdd(rotateIncrement, view.rot, view.rot);

        glTranslatef(0, 0, -view.zoom);

        glRotatef((float)view.rot[0], 1, 0, 0);
        if (view.stereoMode) {
            glRotatef((float)view.rot[1] + (view.stereoModeCurrentBit-.5) * view.stereoSeparation, 0, 1, 0);
        } else {
            glRotatef((float)view.rot[1], 0, 1, 0);
        }
        glRotatef((float)view.rot[2], 0, 0, 1);

        if (view.autoCenter)
            translateToCenter();

        // draws the oct tree
        if (view.drawTree)
            otDrawTree();

        drawFrame();

        if (view.stereoOSD == 1) {
            if (view.drawOSD) {
                drawOSD();
                if (view.drawColourScheme) drawRGB();
            }
            conDraw();
            drawPopupText();
        }

    }

    if (view.vertices > view.maxVertices && view.tailSkip < state.particleCount) {
        view.tailSkip*=2;
        conAdd(LNORM, "Adjusting tailSkip to %i because vertices is bigger then allowed (maxvertices=%i)", view.tailSkip, view.maxVertices);
    }

    glViewport(0, 0, video.screenW, video.screenH);
    if (view.stereoOSD == 0) {
        if (view.drawOSD) {
            drawOSD();
            if (view.drawColourScheme) drawRGB();
        }
        conDraw();
        drawPopupText();
    }

    // just a test...
    // otDrawField();

//	if (view.drawAxis)
//		drawAxis();

//	if (view.drawOSD) {

//		drawOSD();
//		if (view.drawColourScheme) drawRGB();

//	}

//	conDraw();


    if (view.screenshotLoop)
        cmdScreenshot(NULL);

    SDL_GL_SwapBuffers();

}

void drawCube() {

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

}

FPglPointParameterfARB glPointParameterfARB_ptr;
FPglPointParameterfvARB glPointParameterfvARB_ptr;

void checkPointParameters() {

    const char *extList;

    video.supportPointParameters = 0;

    extList = glGetString(GL_EXTENSIONS);

    if (strstr(extList, "GL_ARB_point_parameters") == 0) {
        return;
    }

    glPointParameterfARB_ptr = (FPglPointParameterfARB) SDL_GL_GetProcAddress("glPointParameterfARB");
    glPointParameterfvARB_ptr = (FPglPointParameterfvARB) SDL_GL_GetProcAddress("glPointParameterfvARB");

    if (!glPointParameterfARB_ptr || !glPointParameterfvARB_ptr)
        return;

    video.supportPointParameters = 1;

}

void checkPointSprite() {

    const char *extList;

    video.supportPointSprite = 0;

    extList = glGetString(GL_EXTENSIONS);

    if (strstr(extList, "GL_ARB_point_sprite") == 0)
        return;

    video.supportPointSprite = 1;

}

void drawPopupText() {

    float w;
    float scale = 1.5f;
    float a;
    Uint32 ms;

    ms = getMS();

    if (!view.popupTextMessage[0])
        return;

    if (ms - view.popupTextStart > view.popupTextLength) {
        view.popupTextMessage[0] = 0;
        return;
    }

    drawFrameSet2D();

    if (ms - view.popupTextStart < view.popupTextFadeTime)
        a = (ms - view.popupTextStart) / view.popupTextFadeTime;
    else if (ms - (view.popupTextStart + view.popupTextLength - view.popupTextFadeTime) < view.popupTextFadeTime)
        a = 1.f - (ms - (view.popupTextStart + view.popupTextLength - view.popupTextFadeTime)) / view.popupTextFadeTime;
    else
        a = 1;

    glColor4f(1,1,1,a);

    w = getWordWidth(view.popupTextMessage) * scale;

    glPushMatrix();
    glScalef(scale,scale,scale);
    drawFontWord((float)video.screenW / 2 / scale - w / 2 / scale, (float)video.screenH / 2 / scale - (fontHeight*scale) / 2 / scale, view.popupTextMessage);
    glPopMatrix();

}

#endif
