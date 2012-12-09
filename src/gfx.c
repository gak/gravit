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

// microsoft specific workarounds for missing C99 standard functions
#ifdef _MSC_VER
#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
#include <math.h>
#define fmax max
#define fmin min
#endif
#endif

#ifndef WITHOUT_AGAR
#include "agar/gui/surface.h"
#endif

#ifndef NO_GUI

static GLuint particleTextureID = 0;
static GLuint sprites[SPRITE_LAST+1];

// TODO: Move to view struct
static GLuint skyBoxTextureID = 0;
static GLuint skyBoxTextureIDs[6] = {0,0,0,0,0,0};

static int lastSkyBox = -1;    // the last skybox loaded
static int simpleSkyBox = 0;   // if 1, use single skyBoxTextureID

// toDo: auto-detect list of availeable skyboxes
static const char *skyboxes[] = { "simple.png", "purplenebula/", NULL};

void checkDriverBlacklist();


void drawFrameSet2D() {

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, video.screenW, video.screenH, 0, -1.0f, 1.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

void drawFrameSet3D() {

    glViewport(0, 0, video.screenW, video.screenH);
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    gluPerspective(45.0f, 1, 0, 10000.0f);

}

GLuint loadParticleTexture() {
    int i;
    sprites[SPRITE_GLOW]  = loadTexture(va("%s%s", MISCDIR, "/particle_glow.png"), GL_FALSE);
    sprites[SPRITE_GRAY]  = loadTexture(va("%s%s", MISCDIR, "/particle_gray_glow.png"), GL_FALSE);
    sprites[SPRITE_RED]   = loadTexture(va("%s%s", MISCDIR, "/particle_red_glow.png"), GL_FALSE);
    sprites[SPRITE_GREEN] = loadTexture(va("%s%s", MISCDIR, "/particle_green_glow.png"), GL_FALSE);
    sprites[SPRITE_BLUE]  = loadTexture(va("%s%s", MISCDIR, "/particle_blue_glow.png"), GL_FALSE);
    sprites[SPRITE_GRAY2] = loadTexture(va("%s%s", MISCDIR, "/particle_gray2.png"), GL_FALSE);
    sprites[SPRITE_GLOW2] = loadTexture(va("%s%s", MISCDIR, "/particle_glow2.png"), GL_FALSE);

    particleTextureID = loadTexture(va("%s%s", MISCDIR, "/particle.png"), GL_FALSE);
    sprites[SPRITE_DEFAULT] = particleTextureID;

    for (i=0; i < SPRITE_LAST+1; i++)
        if(sprites[i] == 0) sprites[i] = particleTextureID;

    return particleTextureID;
}

GLuint loadSkyBoxTexture(char *fileName, GLuint *textureID) {
    // free previously bound texture
    if ((*textureID != 0) && glIsTexture(*textureID)) {
        glDeleteTextures(1, textureID);
    }

    *textureID = loadTexture(va("%s/skybox/%s", MISCDIR, fileName), GL_TRUE);

    // catch error
    if ((*textureID == 0) || !glIsTexture(*textureID)) {
        view.drawSky ++;
        if(view.drawSky > SKYBOX_LAST) view.drawSky = 0;
    }

    return *textureID;
}


// load skybox according to view.drawSky
void loadSkyBox() {

    char *skyFile;
    int sky = view.drawSky;
    
    if (sky == 0 || sky > SKYBOX_LAST)
        return;

    lastSkyBox = sky;
    skyFile = (char *) skyboxes[sky - 1];
    
    if (skyFile[strlen(skyFile) - 1] == '/') {
        // if the filename ends with "/", we assume it's a directory
        // use individual texture for each surface
        simpleSkyBox = 0;
        loadSkyBoxTexture(va("%s%s", skyFile, "front.png"), skyBoxTextureIDs);
        loadSkyBoxTexture(va("%s%s", skyFile, "left.png"),  skyBoxTextureIDs + 1);
        loadSkyBoxTexture(va("%s%s", skyFile, "back.png"),  skyBoxTextureIDs + 2);
        loadSkyBoxTexture(va("%s%s", skyFile, "right.png"), skyBoxTextureIDs + 3);
        loadSkyBoxTexture(va("%s%s", skyFile, "top.png"),   skyBoxTextureIDs + 4);
        loadSkyBoxTexture(va("%s%s", skyFile, "bottom.png"),skyBoxTextureIDs + 5);
    } else {        
        // use one texture for all box surfaces
        simpleSkyBox = 1;
        loadSkyBoxTexture(skyFile, &skyBoxTextureID);
    }
}

int gfxSetResolution() {
    SDL_VideoInfo* videoInfo;

    video.screenW = video.screenWtoApply;
    video.screenH = video.screenHtoApply;

    if (video.screenAA) {

        SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 1);
        SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, 4);

    }

    video.flags = SDL_OPENGL;

    if (video.screenFS)
        video.flags |= SDL_FULLSCREEN;
    else
        video.flags |= SDL_RESIZABLE;
    
    videoInfo = (SDL_VideoInfo*) SDL_GetVideoInfo();
    
    if (!video.screenW || !video.screenH || video.screenFS) {
        video.screenW = videoInfo->current_w;
        video.screenH = videoInfo->current_h;
    }

    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1);
    
    video.sdlScreen = SDL_SetVideoMode(video.screenW, video.screenH, video.screenBPP, video.flags );
    if (!video.sdlScreen) {
        conAdd(LERR, "SDL_SetVideoMode failed: %s", SDL_GetError());
        return 1;
    }

    glEnable(GL_TEXTURE_2D);

    // need to (re)load textures
    if (!loadFonts())
        return 2;

    if (!loadParticleTexture())
        return 3;

    loadSkyBox();
    
    // not sure if we need to re-attach to new surface
    //if (video.agarStarted == 1) {
    //    if (AG_SetVideoSurfaceSDL(video.sdlScreen) == -1) {
    //        ( conAdd(LERR, "agar error while attaching to resized window: %s", AG_GetError() );
    //    }
    //}

    return 0;
}

int gfxInit() {

    int detectedBPP;
    SDL_Surface *icon;
    int ret;
    char *fileName;

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

    fileName = findFile(MISCDIR "/gravit.png");
    if (!fileName) {
        return 0;
    }
    icon = IMG_Load(fileName);
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
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glCheck();

    checkPointParameters();
    checkPointSprite();

    checkDriverBlacklist();

    SDL_ShowCursor(view.showCursor);
    SDL_EnableUNICODE(SDL_ENABLE);
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY,SDL_DEFAULT_REPEAT_INTERVAL);

#ifndef WITHOUT_AGAR
    AG_InitCore("gravit", 0);
    //AG_InitGraphics("sdlgl");
    if (AG_InitVideoSDL(video.sdlScreen, AG_VIDEO_OVERLAY | AG_VIDEO_OPENGL_OR_SDL) == -1)
        conAdd(LERR, "agar error while initializing main window: %s", AG_GetError() );

    video.agarStarted = 1;

    if (!view.screenSaver)
        osdInitDefaultWindows();
#endif
    
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
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        break;
    case 2:
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        break;
    case 3:
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_ONE);
        break;
    case 4:
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_ONE_MINUS_SRC_ALPHA);
        break;

    }
    glCheck();

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

        float quadratic_0[] =  { 0.0f, 0.0f, 0.008f };
        float quadratic_1[] =  { 0.0f, 0.0f, 0.00006f };
        float quadratic_2[] =  { 0.0f, 0.0f, 0.000006f, 0.00f };
        float quadratic_3[] =  { 0.0f, 0.0f, 0.0000008f, 0.00f };
        float quadratic_4[] =  { 0.0f, 0.0f, 0.0000001f, 0.00f };
        float quadratic_5[] =  { 0.0f, 0.0f, 0.000008f, 0.00f };
        float quadratic_6[] =  { 0.0f, 0.0f, 0.0000015f, 0.00f };
        float quadratic_7[] =  { 0.0f, 0.0f, 0.0000005f, 0.00f };
        float quadratic_8[] =  { 0.0f, 0.0f, 0.00000006f, 0.00f };
        float *quadratic;

        quadratic = quadratic_0;
        if (view.glow == 1) quadratic = quadratic_1;
        if (view.glow == 2) quadratic = quadratic_2;
        if (view.glow == 3) quadratic = quadratic_3;
        if (view.glow == 4) quadratic = quadratic_4;
        if (view.glow == 5) quadratic = quadratic_5;
        if (view.glow == 6) quadratic = quadratic_6;
        if (view.glow == 7) quadratic = quadratic_7;
        if (view.glow >= 8) quadratic = quadratic_8;

        if (!video.supportPointParameters || !video.supportPointSprite) {

            conAdd(LNORM, "Sorry, Your video card does not support GL_ARB_point_parameters and/or GL_ARB_point_sprite.");
            conAdd(LNORM, "This means you can't have really pretty looking particles.");
            conAdd(LNORM, "Setting particleRenderMode to 2");
            view.particleRenderMode = 2;
            return;

        }

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_POINT_SMOOTH);	// enabling this makes particles dissapear

	// glPointParameter and glPointSprite attributes are global (not per-texture)
        glPointParameterfvARB_ptr( GL_POINT_DISTANCE_ATTENUATION_ARB, quadratic );
        glCheck();

        glPointParameterfARB_ptr( GL_POINT_SIZE_MAX_ARB, view.particleSizeMax );
        glCheck();
        glPointParameterfARB_ptr( GL_POINT_SIZE_MIN_ARB, view.particleSizeMin );
        glCheck();

        glPointSize( view.particleSizeMax );
        glCheck();

        // lets you put textures on the sprite
        // doesn't work on some cards for some reason :(
        // so i had to make textures an option with this mode
        if (view.particleRenderTexture) {
            glTexEnvf( GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE );
            glBindTexture(GL_TEXTURE_2D, sprites[SPRITE_DEFAULT]);
        } else {
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        glEnable( GL_POINT_SPRITE_ARB );
        glCheck();

    } else if (view.particleRenderMode == 2) {

        glDisable(GL_DEPTH_TEST);
        if (view.particleRenderTexture) {
            glBindTexture(GL_TEXTURE_2D, sprites[SPRITE_DEFAULT]);
        } else {
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        glCheck();

    }

    if (view.particleRenderMode == 0 || view.particleRenderMode == 1) {

        unsigned int lastSprite=state.particleDetail[0].particleSprite;
        if (view.particleRenderMode > 0) glBindTexture(GL_TEXTURE_2D, sprites[lastSprite]);
        glCheck();

        // Enabling GL_DEPTH_TEST and setting glDepthMask to GL_FALSE makes the
        // Z-Buffer read-only, which helps remove graphical artifacts generated
        // from  rendering a list of particles that haven't been sorted by
        // distance to the eye.
        glEnable( GL_DEPTH_TEST );
        glDepthMask( GL_FALSE );

        glBegin(GL_POINTS);
        for (i = 0; i < state.particleCount; i++) {

            VectorNew(pos);

            pd = state.particleDetail + i;

            if ((view.particleRenderMode > 0) && (pd->particleSprite != lastSprite)) {
                glEnd();
                glBindTexture(GL_TEXTURE_2D, sprites[pd->particleSprite]);
                //glCheck();
                glBegin(GL_POINTS);
            }
            lastSprite = pd->particleSprite;

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

        glDepthMask( GL_TRUE );
        glDisable( GL_DEPTH_TEST );

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
        if (view.stereoMode == 1) {
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
            int success;
            pos = moo;

            pd = state.particleDetail + i;

            if (view.frameSkip < 0) {
                particleInterpolate(i, ((float)view.frameSkipCounter / view.frameSkip), moo);
            } else {
                p = state.particleHistory + state.particleCount * state.currentFrame + i;
                pos = p->pos;
            }

            success = gluProject(
                pos[0],pos[1],pos[2],
                matModelView, matProject, viewport,
                &screen[0], &screen[1], &screen[2]
            );

            if ((success != GL_TRUE) || (screen[2] > 1.0) || (screen[2] < -1.0))
                continue;

            /* THIS IS A DIRTY HACK, but it works. */
            /* it seems that z is usually very close to 1 (between 0.999982 and 0.999995)
             * -> To achieve an effect similar to "glow" in particlerendermode 1,
             * we multiply the z value with itself several times, which actually "stretches" 
             * the value range towards the lower values.
             */
            if (view.glow > 0) {
	        // basic amplification
                screen[2] *= screen[2] * screen[2];
                screen[2] *= screen[2] * screen[2];
                screen[2] *= screen[2] * screen[2];
                screen[2] *= screen[2] * screen[2];
                screen[2] *= screen[2] * screen[2];
                screen[2] *= screen[2] * screen[2];
                screen[2] *= screen[2] * screen[2];
            }
            if (view.glow >= 1) {
	        // similat to attenuation 0.0001
                screen[2] *= screen[2] * screen[2];
            }
            if (((view.glow >= 2) && (view.glow < 5)) || (view.glow >= 6)) {
	        // similat to attenuation 0.00001
                screen[2] *= screen[2] * screen[2];
            }
            if (((view.glow >= 3) && (view.glow < 5)) || (view.glow >= 7)) {
	        // similat to attenuation 0.000001
                screen[2] *= screen[2] * screen[2];
            }
            if (((view.glow >= 4) && (view.glow < 5)) || (view.glow >= 8)) {
	        // similat to attenuation 0.0000001
                screen[2] *= screen[2] * screen[2];
            }

            if (screen[2] > 1.0) screen[2] = 1.0;
            if (screen[2] < -1.0) screen[2] = -1.0;

            size = view.particleSizeMin + (1.f - (float)screen[2]) * view.particleSizeMax;
            size = fmin(fabs(size), fabs(view.particleSizeMax));
            glBindTexture(GL_TEXTURE_2D, sprites[pd->particleSprite]);

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

            view.vertices += 4;

        }

        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glCheck();

    }

    if (view.particleRenderMode == 1 && video.supportPointParameters && video.supportPointSprite) {

        glDisable( GL_POINT_SPRITE_ARB );
        glCheck();

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
#ifndef WITHOUT_AGAR
    float sy = 70 + margin;
#else
    float sy = margin;
#endif
    float wx = width;
    float wy = 200;
    float c[4];
    float step = .01f;

    if (view.screenSaver)
        sy = margin;

    drawFrameSet2D();

    glEnable(GL_BLEND);
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glBindTexture(GL_TEXTURE_2D, 0);

    // positive
    for (i = 0; i < 1; i += step) {

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
    VectorCopy(pos, view.lastCenter);
}

#ifndef WITHOUT_AGAR
void drawAgar() {
    AG_Window *win;

    if (AG_TIMEOUTS_QUEUED())
		AG_ProcessTimeouts(AG_GetTicks());

    // do not draw windows in screensaver mode
    if (!view.screenSaver)
    {
        AG_LockVFS(&agDrivers);
        AG_BeginRendering(agDriverSw);
        AG_FOREACH_WINDOW(win, agDriverSw) {
            AG_ObjectLock(win);
            AG_WindowDraw(win);
            AG_ObjectUnlock(win);
        }
    
        AG_EndRendering(agDriverSw);
        AG_UnlockVFS(&agDrivers);
    }
    // Agar leaves glTexEnvf env mode to GL_REPLACE :(
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}
#endif

void setupCamera(int shouldTranslate, int bits) {
    
    glViewport(video.screenW / bits * view.stereoModeCurrentBit, 0, video.screenW / bits, video.screenH);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (!view.stereoMode) {
        // narrow field of view when zooming in  (looks good with skybox :)
        // the effect is similar to zooming in with a telescope.
        // the formula below makes sure the field is logarithmicially adjusted between 15 and 55 degrees.
        float fieldOfView = 15.0 + 40.0 * (fmax(0.1, log(fmin(view.zoom, 96000.0)) / log(96000.0f)));

        gluPerspective(fieldOfView, (GLfloat)video.screenW / bits / (GLfloat)video.screenH, 0.01f, fmax(view.zoom * 2.0f, 100000.0f));
    } else {
        // if stereo mode, do not adjust field of view
        gluPerspective(45, (GLfloat)video.screenW / bits / (GLfloat)video.screenH, 0.01f, fmax(view.zoom * 2.0f, 100000.0f));
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
        
    if (shouldTranslate)
        glTranslatef(0, 0, -view.zoom);
    
    glRotatef((float)view.rot[0], 1, 0, 0);
    if (view.stereoMode) {
        glRotatef((float)view.rot[1] + (view.stereoModeCurrentBit - .5) * view.stereoSeparation, 0, 1, 0);
    } else {
        glRotatef((float)view.rot[1], 0, 1, 0);
    }
    glRotatef((float)view.rot[2], 0, 0, 1);

}


void setupStereoCamera(int shouldTranslate) {
    // set up stereo-view camera for red-cyan anaglyph glasses
    //
    // instead of gluPerspective, we use an asymetric frustrum
    // see     : http://quiescentspark.blogspot.com/2011/05/rendering-3d-anaglyph-in-opengl.html
    // see also: http://paulbourke.net/texture_colour/anaglyph/

    float top, bottom, left, right;
    float a, b, c;

    //const int bits = 2;
    const float nearClip = 0.01f;
    const float farClip = fmax(view.zoom*2.0f, 100000.0f);
    const float aspectRatio = (GLfloat)video.screenW / (GLfloat)video.screenH;
    const float distConvergence = view.zoom * 0.95;

    // telescope-zoom
    //const float fieldOfView = 15.0 + 40.0 * (fmax(0.1, log(fmin(view.zoom, 96000.0)) / log(96000.0f)));
    //const float eyeDistance = view.stereoSeparation * fieldOfView;
    // static view
    const float fieldOfView = 45.0;
    const float eyeDistance = view.stereoSeparation*45.0;


    // compute asymetric perspective frustrum

    top = nearClip * tan( (fieldOfView*PI/180.0)/2.0);
    bottom = -top;

    a = aspectRatio * tan( (fieldOfView*PI/180.0)/2.0) * distConvergence;
    b = a - eyeDistance/2.0;
    c = a + eyeDistance/2.0;

    if (view.stereoModeCurrentBit == 0) {
        left = -b * nearClip/distConvergence;
        right=  c * nearClip/distConvergence;
    } else {
        left = -c * nearClip/distConvergence;
        right=  b * nearClip/distConvergence;
    }


    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, video.screenW, video.screenH);

    // Set the Projection Matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glFrustum(left, right, bottom, top, nearClip, farClip);
    //gluPerspective(fieldOfView, aspectRatio, nearClip, farClip);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // translate to left or right eye
    if (view.stereoModeCurrentBit == 0)
         glTranslatef( eyeDistance/2.0, 0.0f, 0.0f);
    else
         glTranslatef(-eyeDistance/2.0, 0.0f, 0.0f);
    // glRotatef((float)(view.stereoModeCurrentBit - .5) * view.stereoSeparation * -0.2 , 0, 1, 0);


    if (shouldTranslate)
        glTranslatef(0, 0, -view.zoom);

    glRotatef((float)view.rot[0], 1, 0, 0);
    glRotatef((float)view.rot[1], 0, 1, 0);
    glRotatef((float)view.rot[2], 0, 0, 1);


    // set filter for anaglyph colors, and
    // render into different color bits
    if (view.stereoModeCurrentBit == 0)
      // red
      glColorMask(GL_TRUE, GL_FALSE, GL_FALSE, GL_TRUE);
    else
      // cyan
      glColorMask(GL_FALSE, GL_TRUE, GL_TRUE, GL_TRUE);
}


void drawSkyBox(int bits) {
    // fade skybox when zooming in
    // (alpha blending with black background)
    float fade = 0.15 + 0.85 * (sqrt(fmin(view.zoom, 48000.0f) / 48000.0f));

    // check for valid texture before drawing the box
    if (  ((simpleSkyBox == 0) && ((skyBoxTextureIDs[0] == 0) || !glIsTexture(skyBoxTextureIDs[0])))
        ||((simpleSkyBox == 1) && ((skyBoxTextureID == 0) || !glIsTexture(skyBoxTextureID)))) {
        conAdd(LERR, "invalid Sky texture #%d", view.drawSky);
        view.drawSky ++;
        if(view.drawSky > SKYBOX_LAST) view.drawSky = 0;
        return;
    }

    if (view.stereoMode > 1) {
        setupStereoCamera(GL_FALSE);
        //fade = 1.0;
    } else {
        setupCamera(GL_FALSE, bits);
    }
    
    glPushAttrib(GL_ENABLE_BIT);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_CULL_FACE);
    glDisable(GL_LIGHTING);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    // Just in case we set all vertices to white.
    glColor4f(1, 1, 1, fade);

    // single texture --> bind now
    if (simpleSkyBox == 1) glBindTexture(GL_TEXTURE_2D, skyBoxTextureID);
    
    // Render the front quad
    if (simpleSkyBox == 0) glBindTexture(GL_TEXTURE_2D, skyBoxTextureIDs[0]);
    glBegin(GL_QUADS);
    glTexCoord2f(1, 1); glVertex3f(  0.5f, -0.5f, -0.5f );
    glTexCoord2f(1, 0); glVertex3f(  0.5f,  0.5f, -0.5f );
    glTexCoord2f(0, 0); glVertex3f( -0.5f,  0.5f, -0.5f );
    glTexCoord2f(0, 1); glVertex3f( -0.5f, -0.5f, -0.5f );
    glEnd();

    // Render the left quad
    if (simpleSkyBox == 0) glBindTexture(GL_TEXTURE_2D, skyBoxTextureIDs[1]);
    glBegin(GL_QUADS);
    glTexCoord2f(1, 0); glVertex3f( -0.5f,  0.5f, -0.5f );
    glTexCoord2f(0, 0); glVertex3f( -0.5f,  0.5f,  0.5f );
    glTexCoord2f(0, 1); glVertex3f( -0.5f, -0.5f,  0.5f );
    glTexCoord2f(1, 1); glVertex3f( -0.5f, -0.5f, -0.5f );
    glEnd();
    
    // Render the back quad
    if (simpleSkyBox == 0) glBindTexture(GL_TEXTURE_2D, skyBoxTextureIDs[2]);
    glBegin(GL_QUADS);
    glTexCoord2f(1, 1); glVertex3f( -0.5f, -0.5f,  0.5f );
    glTexCoord2f(1, 0); glVertex3f( -0.5f,  0.5f,  0.5f );
    glTexCoord2f(0, 0); glVertex3f(  0.5f,  0.5f,  0.5f );
    glTexCoord2f(0, 1); glVertex3f(  0.5f, -0.5f,  0.5f );
    glEnd();    

    // Render the right quad
    if (simpleSkyBox == 0) glBindTexture(GL_TEXTURE_2D, skyBoxTextureIDs[3]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 1); glVertex3f(  0.5f, -0.5f, -0.5f );
    glTexCoord2f(1, 1); glVertex3f(  0.5f, -0.5f,  0.5f );
    glTexCoord2f(1, 0); glVertex3f(  0.5f,  0.5f,  0.5f );
    glTexCoord2f(0, 0); glVertex3f(  0.5f,  0.5f, -0.5f );
    glEnd();
    
    // Render the top quad
    if (simpleSkyBox == 0) glBindTexture(GL_TEXTURE_2D, skyBoxTextureIDs[4]);
    glBegin(GL_QUADS);
    glTexCoord2f(1, 1); glVertex3f(  0.5f,  0.5f, -0.5f );
    glTexCoord2f(1, 0); glVertex3f(  0.5f,  0.5f,  0.5f );
    glTexCoord2f(0, 0); glVertex3f( -0.5f,  0.5f,  0.5f );
    glTexCoord2f(0, 1); glVertex3f( -0.5f,  0.5f, -0.5f );
    glEnd();
    
    // Render the bottom quad
    if (simpleSkyBox == 0) glBindTexture(GL_TEXTURE_2D, skyBoxTextureIDs[5]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f( -0.5f, -0.5f, -0.5f );
    glTexCoord2f(0, 1); glVertex3f( -0.5f, -0.5f,  0.5f );
    glTexCoord2f(1, 1); glVertex3f(  0.5f, -0.5f,  0.5f );
    glTexCoord2f(1, 0); glVertex3f(  0.5f, -0.5f, -0.5f );
    glEnd();

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glBindTexture(GL_TEXTURE_2D, 0);

    glPopAttrib();
}

void drawAll() {

    int bits;
    
    VectorNew(rotateIncrement);
    VectorMultiply(view.autoRotate, view.deltaVideoFrame, rotateIncrement);
    VectorAdd(rotateIncrement, view.rot, view.rot);

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    view.vertices = 0;

    if (view.stereoMode > 0)
        bits = 2;
    else
        bits = 1;

    for (view.stereoModeCurrentBit = 0; view.stereoModeCurrentBit < bits; view.stereoModeCurrentBit++) {

        if ((view.drawSky > 0) && (view.stereoMode < 2)) {
             // load skybox textures if necessary
             if (lastSkyBox != view.drawSky)
                 loadSkyBox();
             drawSkyBox(bits);
        }


        if (view.stereoMode > 1) {
	    // red-cyan anaglyph
            setupStereoCamera(GL_TRUE);
	} else {
	    // normal view or side-by-side
	    setupCamera(GL_TRUE, bits);
	}

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
    // reset color mask
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        
    if (view.vertices > view.maxVertices && view.tailSkip < state.particleCount) {
        view.tailSkip *= 2;
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
    
#ifndef WITHOUT_AGAR
    drawAgar();
#else
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
#endif

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

    char *extList;

    video.supportPointParameters = 0;

    extList = (char *)glGetString(GL_EXTENSIONS);

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

    char *extList;

    video.supportPointSprite = 0;

    extList = (char *)glGetString(GL_EXTENSIONS);

    if (strstr(extList, "GL_ARB_point_sprite") == 0)
        return;

    video.supportPointSprite = 1;

}


void checkDriverBlacklist() {
    char *glVendor;     // the company responsible for this OpenGL implementation 
    char *glRenderer;   // particular driver configuration (i.e. hardware platform)
    // The strings GL_VENDOR and GL_RENDERER together uniquely specify a platform

    char *glVersion;    // OpenGL version number. Vendor specific information may follow the number.
    char *extList;      // list of supported OpenGL extensions

    glVendor   = (char *)glGetString(GL_VENDOR);
    glRenderer = (char *)glGetString(GL_RENDERER);
    glVersion  = (char *)glGetString(GL_VERSION);
    extList    = (char *)glGetString(GL_EXTENSIONS);

    conAdd(LNORM, "OpenGL video driver: vendor=%s; renderer=%s; version=%s", glVendor, glRenderer, glVersion);

    /*
     * Hardware/Driver-specific workarounds
     */

    // Intel Graphics Media Accelerator (GMA) - usually part of mobile core i5/i7 CPUs
    if ((strcmp(glVendor, "Intel") == 0)
        && (  (strcmp(glRenderer, "Intel(R) HD Graphics") == 0)
            ||(strcmp(glRenderer, "Mobile Intel(R) HD Graphics") == 0))
       ) {
       if (strstr(glVersion, " Build 8.15.10.") != NULL) {
          // driver 15.10. causes problems with particlerendermode 1 (GL_ARB_point_sprite)
          // (first observed in Version "2.1.0 - Build 8.15.10.2509" on Windows 7, 64bit)
          // effect: particles properly drawn, but OSD and text in windows is not visible any more
          // other affected driver versions:
          //    2.1.0 - Build 8.15.10.2559 (OSD not visible; but text in agar windows is OK)
          // drivers that seem to work:
          //    2.1.0 - Build 8.15.10.2202

	  if (view.particleRenderMode == 1) {
              conAdd(LERR,  "Sorry, Your video card driver is blacklisted for particleRenderMode 1.");
              conAdd(LERR, "This means you can't have really pretty looking particles.");
              conAdd(LNORM, "Setting particleRenderMode to 2");
              view.particleRenderMode = 2;
              view.particleSizeMax = 63;
	  }
          video.supportPointSprite = 0;
       }
    }

   // linux: MESA openGL on Intel integrated graphics
   // (VMVare is also based on MESA)
    if ((strcmp(glVendor, "Tungsten Graphics, Inc") == 0) || (strcmp(glVendor, "VMware, Inc.") == 0)) {
      if ((  strncmp(glRenderer, "Mesa DRI Intel(R) Ironlake Mobile", strlen("Mesa DRI Intel(R) Ironlake Mobile")) == 0)
          || (strncmp(glRenderer, "Mesa DRI Mobile Intel", strlen("Mesa DRI Mobile Intel")) == 0)
          || (strncmp(glRenderer, "Gallium ", strlen("Gallium ")) == 0)) {
          if (  (strstr(glVersion, "1.4 (2.1 Mesa") != NULL)
                ||(strstr(glVersion, "2.1 Mesa 8.0.") != NULL)
                ||(strstr(glVersion, "2.1 Mesa 7.") != NULL)) {
               // MESA 8.0 claims to have GL_ARB_point_sprite, but it does not work with particlerendermode 1.
               // effect: instead of particle textures, coloured squares are drawn.
                // known configurations that are affected:
                //      mesa 8.0.4-0ubuntu0.2 (with mesa-dri), xorg-server 2:1.11.4-0ubuntu10.8  on Linux 3.2.0-32-generic #51-Ubuntu SMP x86_64 GNU/Linux
                //      GL vendor=Tungsten Graphics, Inc; GL renderer=Mesa DRI Intel(R) Ironlake Mobile ; GL version=2.1 Mesa 8.0.4
                //      GL vendor=Tungsten Graphics, Inc; GL renderer=Mesa DRI Intel(R) Ironlake Mobile ; GL version=1.4 (2.1 Mesa 8.0.4)
                //      GL vendor=VMware, Inc.; GL renderer=Gallium 0.4 on llvmpipe (LLVM 0x300); GL version=1.4 (2.1 Mesa 8.0.4)
 	        if (view.particleRenderMode == 1) {
                    conAdd(LERR,  "Sorry, Your driver is blacklisted for particleRenderMode 1.");
                    conAdd(LERR, "This means you can't have really pretty looking particles.");
                    conAdd(LNORM, "Setting particleRenderMode to 2");
                    view.particleRenderMode = 2;
                    view.particleSizeMax = 63;
	        }
                video.supportPointSprite = 0;
          }
      }
      if (strncmp(glRenderer, "Mesa GLX Indirect", strlen("Mesa GLX Indirect")) == 0) {
          if (view.particleRenderMode == 1) {
              conAdd(LERR,  "Sorry, Your indirect rendering GLX driver is blacklisted for particleRenderMode 1.");
              conAdd(LHELP, "Indirect rendering is too slow for really pretty looking particles.");
              conAdd(LNORM, "Setting particleRenderMode to 0");
              view.particleRenderMode = 0;
              view.particleSizeMax = 63;
	  }
          video.supportPointSprite = 0;
      }
    }


    // software OpenGL driver, Microsoft Windows
    if ((strcmp(glVendor, "Microsoft Corporation") == 0) && (strcmp(glRenderer, "GDI Generic") == 0)) {
       // awfully SLOW. -> prefer particleRenderMode 0
       if (view.particleRenderMode == 1) {
           conAdd(LHELP,  "Sorry, Your software driver is blacklisted for particleRenderMode 1.");
           conAdd(LHELP, "It's too slow for really pretty looking particles.");
           conAdd(LNORM, "Setting particleRenderMode to 0");
           view.particleRenderMode = 0;
           view.particleSizeMax = 63;
       }
       // ToDo: add more tweak to achieve higher framerates ...
    }

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
