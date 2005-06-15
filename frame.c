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

int initFrame() {

	state.frame = 0;
	state.frameCompression = 1;
	state.totalFrames = 0;
	state.historyNFrame = 1;
	state.currentFrame = 0;
	view.quit = 0;

	cleanMemory();

//	conAdd(LERR, "Allocating %i bytes", FRAMESIZE * state.historyFrames);
	state.particleHistory = _aligned_malloc(FRAMESIZE * state.historyFrames, 16);

	if (!state.particleHistory) {

		conAdd(LNORM, "Could not allocate %i bytes of memory for particleHistory", FRAMESIZE * state.historyFrames);
		return 0;

	}

	state.memoryAllocated += FRAMESIZE * state.historyFrames;

//	conAdd(LERR, "Allocating %i bytes", FRAMEDETAILSIZE);
	state.particleDetail = calloc(sizeof(particleDetail_t),state.particleCount);
	if (!state.particleDetail) {

		conAdd(LNORM, "Could not allocate %i bytes of memory for particleDetail", FRAMEDETAILSIZE);
		_aligned_free(state.particleHistory);
		state.memoryAllocated = 0;
		return 0;

	}

	state.memoryAllocated += FRAMEDETAILSIZE;

	memset(state.particleHistory, 0, FRAMESIZE);

	return 1;

}

void processFrameOTthread(int thread) {

	int sliceStart, sliceEnd, sliceSize;

	sliceSize = state.particleCount / state.processFrameThreads;

	sliceStart = sliceSize * thread;
	sliceEnd = sliceSize * thread + sliceSize - 1;

	processFrameOT(sliceStart, sliceEnd);

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

void processFrame() {

	int i;
	particle_t *p;

//	processMomentum();

#ifndef WIN32
	pthread_t ptt[MAX_THREADS];
#endif

	if (state.frame >= state.historyFrames - 1) {
	
		if (state.frameCompression) {

            state.frame /= 2;
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
		
			return;

		}

	}

#if NBODY_METHOD == METHOD_PP

	processFramePP(0, state.particleCount);

#elif NBODY_METHOD == METHOD_OT

	otFreeTree();

#ifdef WIN32	
	processFrameOT(0, state.particleCount);
#else
	if (state.processFrameThreads > 1) {
		
		int i;
		for (i = 0; i < state.processFrameThreads; i++) {
			pthread_create(&ptt[i], NULL, (void*)processFrameOTthread, (void*)i);
		}
		
		for (i = 0; i < state.processFrameThreads; i++) {
			pthread_join(ptt[i], NULL);
		}

	} else {
		processFrameOT(0, state.particleCount);
	}
#endif

#endif

	// Check if the recording frame was cancelled, if so just return;
	if (!(state.mode & SM_RECORD))
		return;

	//	processCollisions();

	//	forceToCenter();

	// move particles to next page
	memcpy(
		
		state.particleHistory + state.particleCount * (state.frame+1),
		state.particleHistory + state.particleCount * state.frame,
		FRAMESIZE
		
	);

	for (i = 0; i < state.particleCount; i++) {
		p = state.particleHistory + state.particleCount * (state.frame+1) + i;
		VectorAdd(p->pos, p->vel, p->pos);
	}

	state.totalFrames ++;

	if (state.frameCompression && (state.totalFrames % state.historyNFrame)) {
	
		memcpy(
			
			state.particleHistory + state.particleCount * state.frame,
			state.particleHistory + state.particleCount * (state.frame+1),
			FRAMESIZE
			
		);
		
		return;

	}

	state.frame ++;
	state.currentFrame = state.frame;

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


