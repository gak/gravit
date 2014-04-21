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

#ifdef _OPENMP
#include <omp.h>

#pragma message("OpenMP optimized multithreading enabled.")
#else
#if (!defined(WIN32) || defined(USE_PTHREAD))
#pragma message("pthreads multithreading enabled.")
#else
#pragma message( __FILE__ " : warning : multithreading not availeable.")
#endif
#endif


int initFrame() {

    state.frame = 0;
    state.frameCompression = 1;
    state.totalFrames = 0;
    state.historyNFrame = 1;
    state.currentFrame = 0;
    state.targetFrame = -1;
    view.quit = 0;

    cleanMemory();

//	conAdd(LERR, "Allocating %u bytes", FRAMESIZE * state.historyFrames);
    state.particleHistory = (particle_t *)calloc(FRAMESIZE, state.historyFrames);

    while (!state.particleHistory) {

        conAdd(LLOW, "Could not allocate %lu bytes of memory for %ld frames of particleHistory", (unsigned long)(FRAMESIZE * state.historyFrames), state.historyFrames);
        if (state.historyFrames < 10) {
            conAdd(LERR, "Could not allocate for particleHistory - giving up.");
            //return 0;
            cmdQuit(NULL);
        }
        // reduce requested size by 10%, and try again
        state.historyFrames = (state.historyFrames / 10) * 9;
        state.particleHistory = (particle_t *)calloc(FRAMESIZE, state.historyFrames);
    }

//	conAdd(LERR, "Allocating %u bytes", FRAMEDETAILSIZE);
    state.particleDetail = calloc(sizeof(particleDetail_t),state.particleCount);
    if (!state.particleDetail) {

        conAdd(LERR, "Could not allocate %lu bytes of memory for particleDetail", (unsigned long)(FRAMEDETAILSIZE));
        free(state.particleHistory);
        state.memoryAllocated = 0;
        //return 0;
        cmdQuit(NULL);

    }

    state.memoryAllocated += FRAMESIZE * state.historyFrames;
    state.memoryAllocated += FRAMEDETAILSIZE;

    memset(state.particleHistory, 0, FRAMESIZE);

    view.timed_frames=0;
    view.totalRenderTime=0;
    view.lastRenderTime = 0;

    fpsInit();

    VectorZero(view.lastCenter);

    state.have_old_accel = 0;

    return 1;

}

void processFrameThread(int thread) {

    int sliceStart, sliceEnd, sliceSize;

#if (defined(WIN32) && !defined(USE_PTHREAD)) || defined(_OPENMP)
    sliceSize = state.particleCount;
    sliceStart = 0;
    sliceEnd = sliceStart + sliceSize;

#else
    sliceSize = state.particleCount / state.processFrameThreads;

    sliceStart = sliceSize * thread;
    sliceEnd = sliceSize * thread + sliceSize;

    // make sure the last thread does not miss remaining particle
    if (thread == (state.processFrameThreads - 1))
        sliceEnd = state.particleCount;
#endif


#if NBODY_METHOD == METHOD_OT
    processFrameOT(sliceStart, sliceEnd);
#else
#ifdef HAVE_SSE
    processFramePP_SSE(sliceStart, sliceEnd);
#else
    processFramePP(sliceStart, sliceEnd);
#endif
#endif
    
}

void processMomentum() {

    double sP[3];
    double tmp[3];
    int i;
    particle_t *p;
    particleDetail_t *pd;

    // work out simulation momentum
    VectorZero(tmp);
    VectorZero(sP);
    for (i = 0; i < state.particleCount; i++) {

        p = state.particleHistory + state.particleCount * state.frame + i;
        pd = state.particleDetail + i;

        VectorMultiply(p->vel, pd->mass, tmp);
        VectorAdd(sP, tmp, sP);

    }

    conAdd(LLOW, "Momentum: %.10f, %.10f, %.10f", sP[0], sP[1], sP[2]);

}


/*
 * compute new particle accelerations, based on current positions
 */
static void accelerateParticles() {
#if (!defined(WIN32) || defined(USE_PTHREAD)) && !defined(_OPENMP)
    pthread_t ptt[MAX_THREADS];
#endif
        int i;

#ifdef _OPENMP
    omp_set_num_threads(state.processFrameThreads);
#endif

    // zero accelerations
    for (i = 0; i < state.particleCount; i++) {
        particleDetail_t *pd;
        pd = getParticleDetail(i);
	VectorZero(pd->accel);
    }

#if NBODY_METHOD == METHOD_OT
    otFreeTree();
#endif

#if (defined(WIN32) && !defined(USE_PTHREAD)) || defined(_OPENMP)
    processFrameThread(0);
#else

    if (state.processFrameThreads > 1) {
        for (i = 0; i < state.processFrameThreads; i++) {
            pthread_create(&ptt[i], NULL, (void*)processFrameThread, (void*)i);
        }
        for (i = 0; i < state.processFrameThreads; i++) {
            pthread_join(ptt[i], NULL);
        }

    } else {
        processFrameThread(0);
    }

#endif
}


/*
 * compute and "integrate" new particle velocities, and advances particle postions to next time frame
 *
 * assumption:   one new frame is availeable at (state.particleHistory + state.particleCount * (state.frame+1))
 * side effects: updates state.frame
 */
static void moveParticles() {
    int i;
    particle_t *p;
    particleDetail_t *pd;

    // use leapfrog integration sheme, as it has a much better acuracy,
    // with very low additional computation costs
    // http://einstein.drexel.edu/courses/Comp_Phys/Integrators/leapfrog/
    // http://www.artcompsci.org/vol_1/v1_web/node34.html
    // note: in gravit, the "time step" t is always 1


    // make sure we know the accelerations of the current frame
    if ((state.totalFrames == 0) || (state.have_old_accel == 0)) {
        accelerateParticles();
	state.have_old_accel = 1;
    }

    // copy particles to next frame
    memcpy(
        state.particleHistory + state.particleCount * (state.frame+1),
        state.particleHistory + state.particleCount * state.frame,
        FRAMESIZE
    );
    // use next frame
    state.frame++;


    // advance velocities by 0.5 step, then advance positions by 1 step
    for (i = 0; i < state.particleCount; i++) {
        p = state.particleHistory + state.particleCount * (state.frame) + i;
        pd = getParticleDetail(i);

        VectorMultiplyAdd(pd->accel, 0.5, p->vel);
        VectorAdd(p->pos, p->vel, p->pos);
    }

    // compute new accelerations
    accelerateParticles();

    // Check if the recording frame was cancelled, if so - forget new frame and return;
    if (!(state.mode & SM_RECORD))
    {
        state.frame--;
	state.have_old_accel = 0;  // also, invalidate saved acceleration values
        return;
    }

    // advance velocities by 0.5 step
    for (i = 0; i < state.particleCount; i++) {
        p = state.particleHistory + state.particleCount * (state.frame) + i;
        pd = getParticleDetail(i);
	VectorMultiplyAdd(pd->accel, 0.5, p->vel);
    }


    //	processCollisions();
    //	forceToCenter();

    // simple "Euler" integration - low accuracy
    // advance velocities, then advance particles to final positions
    //for (i = 0; i < state.particleCount; i++) {
    //    p = state.particleHistory + state.particleCount * (state.frame) + i;
    //    pd = getParticleDetail(i);
    //    VectorAdd(p->vel, pd->accel, p->vel);
    //    VectorAdd(p->pos, p->vel, p->pos);
    //}
}



void processFrame() {

    int i;
    Uint32 frameStart = 0;
    Uint32 frameEnd = 0;

//	processMomentum();

    if (state.frame >= state.historyFrames - 1) {

        if (state.frameCompression) {

            state.frame /= 2;
            if (state.targetFrame >0) state.targetFrame /= 2;
            state.currentFrame = state.frame;
            state.historyNFrame *= 2;
            conAdd(LLOW, "historyNFrame: %i", state.historyNFrame);

            for (i = 0; i < state.frame; i++) {

                memcpy(

                    state.particleHistory + state.particleCount * i,
                    state.particleHistory + state.particleCount * i * 2,
                    FRAMESIZE

                );

            }

            state.frame--;

        } else {

	    // no more frames left - stop recording
            state.targetFrame= -1;
            if (state.mode & SM_RECORD) cmdRecord(NULL);
            return;

        }

    }

    frameStart = getMS();

    moveParticles();

    // Check if the recording frame was cancelled, if so just return;
    if (!(state.mode & SM_RECORD))
        return;

    state.totalFrames ++;

    frameEnd = getMS();
    view.lastRenderTime = frameEnd - frameStart;
    view.totalRenderTime += frameEnd - frameStart;
    view.timed_frames ++;


    if (state.frameCompression && (state.totalFrames % state.historyNFrame)) {

        memcpy(

            state.particleHistory + state.particleCount * state.frame,
            state.particleHistory + state.particleCount * (state.frame+1),
            FRAMESIZE

        );

        return;

    }

    state.currentFrame = state.frame;

    if ((state.targetFrame >= 0) && (state.targetFrame <= state.frame) && (state.mode & SM_RECORD))
    {
      conAdd(LNORM, "target frame reached: %i", state.currentFrame);
      cmdRecord(NULL);
      state.targetFrame = -1;
    }
}

#if 0
// slow
void processCollisions() {

    int i,j;
    particle_t *p1;
    particle_t *p2;
    VectorNew(dv);
    VectorNew(dv2);
    float d;

    for (i = 0; i < state.particleCount; i++) {

        p1 = state.particleHistory + state.particleCount*state.frame + i;

        for (j = 0; j < state.particleCount; j++) {

            if (i == j)
                continue;

            p2 = state.particleHistory + state.particleCount*state.frame + j;

            dv[0] = p1->pos[0] - p2->pos[0];
            dv[1] = p1->pos[1] - p2->pos[1];
            dv[2] = p1->pos[2] - p2->pos[2];

            dv2[0] = pow(dv[0], 2);
            dv2[1] = pow(dv[1], 2);
            dv2[2] = pow(dv[2], 2);

            // get distance between the two
            d = sqrt(dv2[0] + dv2[1] + dv2[2]);

            if (d <= 2.f) {

                // p1->size += p2->size;
                p1->mass += p2->mass;
                p1->vel[0] = (p1->vel[0] + p2->vel[0])/2;
                p1->vel[1] = (p1->vel[1] + p2->vel[1])/2;
                p1->vel[2] = (p1->vel[2] + p2->vel[2])/2;

                conAdd(LLOW, "Particle %i and %i collided: newmass=%f", i, j, p1->mass);

            }

        }

    }

}
#endif

#if 0

// slow/unrealistic
void forceToCenter() {

    int i;
    particle_t *p;
    VectorNew(dv);
    float d;
    float force;

    for (i = 0; i < state.particleCount; i++) {

        p = state.particleHistory + state.particleCount * state.frame + i;

        dv[0] = p->pos[0];
        dv[1] = p->pos[1];
        dv[2] = p->pos[2];

        d = pow(sqrt(dv[0] + dv[1] + dv[2]), 1);

        if (d) {
            //force = -0.000001f * p->mass / d;
            force = -0.000000001f * d * p->mass;
            p->vel[0] += dv[0] * force;
            p->vel[1] += dv[1] * force;
            p->vel[2] += dv[2] * force;
        }

    }
}

#endif


