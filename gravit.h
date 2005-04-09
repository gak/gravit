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

#ifndef GRAVIT_H_
#define GRAVIT_H_

#define GRAVIT_VERSION "Gravit 0.3.0"
#define GRAVIT_COPYRIGHT "Copyright 2003-2005 Gerald Kaszuba"

// #define NO_GUI
// #define WIN32SCREENSAVER

#define MAX_THREADS 1

#ifdef WIN32

	#include <windows.h>
	#include <conio.h>
	#include <stdio.h>

#ifdef WIN32SCREENSAVER

	#include <scrnsave.h>
	#include <SDL_syswm.h>

#endif

	#include <vfw.h>

	// stupid hack to fix linking errors with isspace
	#undef isspace
	#define isspace(x) (x == 32)

	// stop conversion from 'double ' to 'float ', possible loss of data
	#pragma warning ( disable : 4244 ) 

#else

	#include <stdarg.h>
	#include <stdlib.h>
	#include <string.h>
	#include <errno.h>
	#include <termios.h>
    #include <sys/select.h>
    #include <sys/time.h>
    #include <sys/types.h>
    #include <unistd.h>
	#include <sys/timeb.h>
	#include <pthread.h>
	#include <dirent.h>
	#include <sys/stat.h>

	// used for the opengl extensions
	#ifndef APIENTRY
	#define APIENTRY
	#endif

#endif

#ifndef NO_GUI

	// this removes the header requirement for glext.h in gl.h
	#define GL_GLEXT_LEGACY

	#include <SDL.h>
	#include <SDL_ttf.h>
	#include <SDL_opengl.h>
	#include <SDL_image.h>

	#define glCheck() { GLuint er = glGetError(); if (er) { conAdd(1, "glError: %s:%i %i %s", __FILE__, __LINE__, er, gluErrorString(er)); } }

#else

	// sdlkeys mappings
	#include "sdlk.h"

#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>

#define FILE_CHUNK_SIZE (1024*1024)
#define FILE_CHUNK_SIZE_SMALL 1024

#define frand(min,max) ((min) + ((float)rand() / RAND_MAX) * ((max) - (min)))
#define FRAMESIZE (sizeof(particle_t)*state.particleCount)
#define FRAMEDETAILSIZE (sizeof(particleDetail_t) * state.particleCount)

#define getParticleCurrentFrame(i) state.particleHistory + state.particleCount * state.currentFrame + (i)
#define getParticleFirstFrame(i) state.particleHistory + (i)
#define getParticleDetail(i) state.particleDetail + (i)

#define PI 3.14159265358979f

#define SCREENSHOT_PATH "screenshots"
#define SAVE_PATH "save"

#define STRING_RECORD "Recording"
#define STRING_PLAY "Playing"

#define CM_MASS 0
#define CM_VEL 1
#define CM_ACC 2
#define CM_LAST 3

#define METHOD_PP 0
#define METHOD_OT 1

#define NBODY_METHOD METHOD_OT

#define VectorNew(a) float a[3];

#define VectorCopy(a,b) { b[0] = a[0]; b[1] = a[1]; b[2] = a[2]; }
#define VectorAdd(a,b,c) { c[0] = a[0] + b[0]; c[1] = a[1] + b[1]; c[2] = a[2] + b[2]; }
#define VectorSub(a,b,c) { c[0] = a[0] - b[0]; c[1] = a[1] - b[1]; c[2] = a[2] - b[2]; }
#define VectorMultiply(a, b, c) { c[0] = a[0] * b; c[1] = a[1] * b; c[2] = a[2] * b; }
#define VectorDivide(a, b, c) { c[0] = a[0] / b; c[1] = a[1] / b; c[2] = a[2] / b; }
#define VectorZero(x) { x[0] = 0; x[1] = 0; x[2] = 0; }

// #define distance2(a,b,c) c = ((float)pow((double)a[0] - b[0], 2) + (float)pow((double)a[1] - b[1], 2) + (float)pow((double)a[2] - b[2], 2));

#define distance2(a,b,c) {\
  float tmp;\
  c = a[0] - b[0];\
  c = c * c;\
  tmp = a[1] - b[1];\
  c += tmp * tmp;\
  tmp = a[2] - b[2];\
  c += tmp * tmp;\
}


#define distance(a,b,c) { float d; distance2(a,b,d); c = (float)sqrt((double)d); }

#ifndef Uint32
	#define Uint32 unsigned int
#endif

#ifndef WIN32
#define _aligned_malloc(a,b) malloc(a)
#define _aligned_free(a) free(a)
#endif

#define _aligned_malloc(a,b) malloc(a)
#define _aligned_free(a) free(a)

#define CONSOLE_HISTORY 10
#define CONSOLE_LENGTH 255
#define CONSOLE_BLINK_TIME 500

#define TM_STANDARD 0
#define TM_HELP1 1
#define TM_HELP2 2

#define SM_RECORD 2
#define SM_PLAY 4

#define MAX_FONT_LENGTH 255

// this define is to render video in the middle of a recording
#define doVideoUpdate() \
	if (view.lastVideoFrame) { \
		if (!(state.mode & SM_RECORD) ) return; \
		if (view.lastVideoFrame + view.recordingVideoRefreshTime < getMS()) { \
			runInput(); \
			runVideo(); \
			if (!(state.mode & SM_RECORD)) return; \
		} \
	}

// this define is to render video in the middle of a spawn
#define doVideoUpdateInSpawn() \
	if (view.lastVideoFrame) { \
		if (view.lastVideoFrame + view.recordingVideoRefreshTime < getMS()) { \
			runInput(); \
			runVideo(); \
		} \
		if (view.quit) return; \
	}

#include "command.h"

#ifndef NO_GUI
#include "font.h"
#endif

#ifndef NO_GUI

// function pointers for gl extensions
typedef void (APIENTRY *FPglPointParameterfARB)(GLenum, GLfloat);
typedef void (APIENTRY *FPglPointParameterfvARB)(GLenum, GLfloat*);

extern FPglPointParameterfARB glPointParameterfARB_ptr;
extern FPglPointParameterfvARB glPointParameterfvARB_ptr;

// gl defines
#define GL_POINT_SIZE_MIN_ARB 0x8126
#define GL_POINT_SIZE_MAX_ARB 0x8127
#define GL_POINT_FADE_THRESHOLD_SIZE_ARB 0x8128
#define GL_POINT_DISTANCE_ATTENUATION_ARB 0x8129

#define GL_POINT_SPRITE_ARB 0x8861
#define GL_COORD_REPLACE_ARB 0x8862

typedef struct conf_s {

	int screenBPP;
	int screenW;
	int screenH;
	int screenFS;
	int screenAA;

	int supportPointSprite;
	int supportPointParameters;

	char fontFile[MAX_FONT_LENGTH];
	int fontSize;

	SDL_VideoInfo* gfxInfo;

	int sdlStarted;

} conf_t;

#endif

typedef struct col_s {

	float r;
	float g;
	float b;

} col_t;

// things that change per frame
typedef struct particle_s {

	float pos[3];
	float vel[3];

} particle_t;

// things that change less often
typedef struct particleDetail_s {

	float mass;
	float col[4];

} particleDetail_t;

typedef struct state_s {

	particle_t *particleHistory;
	particleDetail_t *particleDetail;

	int memoryAvailable;	// MB
	
	int particleCount;
	int frame;
	int totalFrames;
	int currentFrame;
	int historyFrames;
	int historyNFrame;

	int gbase;
	float g;

	int frameCompression;

	float center[3];
	int mode;
	float massRange[2];

	int processFrameThreads;

	int particlesToSpawn;

	unsigned int memoryAllocated;

	int incSave;	// save last frame + info after every frame
	int autoSave;	// auto save every n frames. 0 for off.
	char *fileName; // if null dont autosave or incsave.

	int currentlySpawning;
	int restartSpawning;

	int dontExecuteDefaultScript;
	
} state_t;

typedef struct saveInfo_s {

	int particleCount;
	int historyFrames;
	int totalFrames;
	int frame;
	int historyNFrame;
//	float g;

} saveInfo_t;

typedef struct view_s {

	Uint32 lastVideoFrame;
	Uint32 deltaVideoFrame;

	Uint32 lastRecordFrame;
	Uint32 deltaRecordFrame;

	// recordingVideoRefreshTime
	// Set this to 0 to never refresh video while rendering
	// Set this to > 0 to refresh video every Nms while rendering
	Uint32 recordingVideoRefreshTime;

	int quit;

	Uint8 mouseButtons[2];	// 0 now, 1 last
	int currentMousePosition[2];
	int lastMousePosition[2];

	float rot[3];
	float zoom;

	float mat1[16];
	float mat2[16];

#ifndef NO_GUI
	int keys[SDLK_LAST];
#endif

	float pos[3];	// todo, position of camera
	float face[3];

	float autoRotate[3];

	float tailWidth;
	int tailLength;
	float tailOpacity;
	int tailFaded;
	int tailSkip;

	int drawAxis;

	int textMode;
	int consoleMode;

	int drawTree;
	int frameSkip;

	int drawOSD;
	int drawColourScheme;

	float fps;
	int verticies;

	int verboseMode;

	int blendMode;

	int screenshotIndex;	// the next available screenshot file (eg screenshot/gravit00001.bmp)
	int screenshotLoop;	// will do a screenshot every frame

	int particleColourMode;	// 0 for colour based on mass, 1 for colour based on velocity
	int particleRenderMode;	// 0 for standard GL_POINT, 1 for GL_ARB_point_sprite
	
	// in particleRenderMode 1, you can render a texture onto the point
	// this doesnt work in some implementations...
	int particleRenderTexture;
	
	float particleSizeMin;	// can be anything 0 or higher
	float particleSizeMax;	// -1 for the maximum supported. if its bigger then supported, it will simply use the supported value.

	float *colourSpectrum;		// determines what colour to draw a particle with
	int colourSpectrumSteps;

} view_t;

typedef struct spawnVars_s {

	int minGalCount;
	int maxGalCount;
	float minGalMass;
	float maxGalMass;
	float minGalSize;
	float maxGalSize;
	float minGalVel;
	float maxGalVel;
	float minSpawnRange;
	float maxSpawnRange;

} spawnVars_t;

typedef struct con_s {

	col_t c;
	char s[255];

} con_t;

// for processframeot
typedef struct otinfo_s {

	particle_t *p;
	particleDetail_t *pd;
	struct node_s *n;

} otinfo_t;

// for otComputeParticleToTreeRecursive
typedef struct pttr_s {

	particle_t *p;
	particleDetail_t *pd;
	struct node_s *n;

} pttr_t;

typedef struct pib_s {

	float *min;
	float *max;
	particle_t *gp;
	float m;
	float *cm;

} pib_t;

// main.c
#ifdef WIN32

#ifndef NO_GUI
extern conf_t conf;
#endif
extern state_t state;
extern view_t view;

#else

#ifndef NO_GUI
conf_t conf;
#endif
state_t state;
view_t view;

#endif

void cleanMemory();
void runVideo();
void runInput();
void viewInit();
void spawnDefaults();
void stateInit();
int commandLineRead(int argc, char *argv[]);

// tool.c
char * va( char *format, ... );
int gfxPowerOfTwo(int input);
int LoadMemoryDump(char *fileName, unsigned char *d, unsigned int size);
int SaveMemoryDump(char *FileName, unsigned char *d, unsigned int total);
Uint32 getMS();
void setTitle(char *state);
int mymkdir(char *path);
void setFileName(char *name);

// spawn.c
extern spawnVars_t spawnVars;
void pickPositions();
void spawnDefaults();
int isSpawning();

// console.c
extern con_t con[CONSOLE_HISTORY];
void conAdd(int mode, char *f, ... );
void conInit();
void conDraw();
void conInput(char c);

#ifndef NO_GUI

// osd.c
void drawOSD();

// input.c
int processKeys();
void processMouse();

// gfx.c
void drawAll();
void drawFrame();
void drawFrameSet2D();
void drawFrameSet3D();
int gfxInit();
void checkPointParameters();
void checkPointSprite();

// color.c
void setColours();
void setColoursByMass();
void colourSpectrumClear();
void colourFromNormal(float *c, float n);

#else

int processKeys();

#endif

// frame.c
int initFrame();
void processFrame();
void forceToCenter();
void processCollisions();

// frame-pp.c
void processFramePP(int s, int n);

// frame-ot.c
typedef struct node_s {

	float min[3];
	float max[3];
	float c[3];
	float cm[3];

	particle_t *p;
    struct node_t *b[8];
	float mass;
	float length;

} node_t;

void otDrawTree();
void otFreeTree();
void processFrameOT(int,int);

void frDoGravity(particle_t *p, node_t *n, float d);

extern float fpsCurrentAverageFPS;
extern float fpsCurrentAverageFT;
void fpsInit();
void fpsUpdate(float);

int configRead(char *filename);

void drawCube();

extern const char *colourModes[];

#endif

