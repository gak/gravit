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
// experimental OMP support
#include <omp.h> // VC has to include this header to build the correct manifest to find vcom.dll or vcompd.dll
#endif


// mm_malloc gives back aligned blocks
#if !defined(__INTEL_COMPILER) && !defined(__OPENCC__)
   // intel and amd compilers do not like this ..
#include <malloc.h>
#include <mm_malloc.h>
#endif

#ifdef __OPENCC__
#include <xmmintrin.h>
#endif

// __restrict__ tell the compiler that two pointer will not point to the same location
// if your compiler complains, just remove __restrict__

#ifdef _MSC_VER
//#define __restrict__ __restrict
//#define __restrict__ __declspec(restrict)
// problems with openMP
#define __restrict__
#endif




typedef struct {
  float * __restrict__ x;
  float * __restrict__ y;
  float * __restrict__ z;
  float * __restrict__ mass;
} particle_vectors;

typedef struct {
  float * __restrict__ x;
  float * __restrict__ y;
  float * __restrict__ z;
} vel_vectors;




  /*  Optimizations:
      ===============
      * before processing, copy particle data to vector-friendly arrays
      * delay multiplication with G
      * after processing, write back results
  */


//#define MIN_STEP2 0.05

static void do_processFramePP(particle_vectors pos, vel_vectors vel, 
                              int start, int amount) {
     int i;
     //int particles_max = state.particleCount;

    // apply gravity to every specified velocity
    #ifdef _OPENMP
    #pragma omp parallel for schedule(dynamic, 256)
    #endif
    for (i = start; i < amount; i++) {
        //VectorNew(p1_pos);
        float p1_pos_x;
        float p1_pos_y;
        float p1_pos_z;
        float p1_mass;

        //VectorNew(p1_vel);
	//VectorZero(p1_vel);
        float p1_vel_x = 0.0f;
        float p1_vel_y = 0.0f;
        float p1_vel_z = 0.0f;

        int j;

	p1_pos_x = pos.x[i];
	p1_pos_y = pos.y[i];
	p1_pos_z = pos.z[i];
	p1_mass   = pos.mass[i];


#ifdef __INTEL_COMPILER
#pragma vector aligned
#endif
        for (j = 0; j < i; j++) {
	    //VectorNew(dv);
	    float dv_x;
	    float dv_y;
	    float dv_z;
	    float inverseSquareDistance;
	    float force;

            dv_x = p1_pos_x - pos.x[j];
            dv_y = p1_pos_y - pos.y[j];
            dv_z = p1_pos_z - pos.z[j];

            // get distance^2 between the two
	    inverseSquareDistance  = dv_x * dv_x;
	    inverseSquareDistance += dv_y * dv_y;
	    inverseSquareDistance += dv_z * dv_z;
	    //inverseSquareDistance +=  + MIN_STEP2;

            force = p1_mass * pos.mass[j] / inverseSquareDistance;

            // sum of accelerations for p1
            p1_vel_x += dv_x * force;
            p1_vel_y += dv_y * force;
            p1_vel_z += dv_z * force;

            // add acceleration for p2 (with negative sign, as the direction is inverted)
            vel.x[j] -= dv_x * force;
            vel.y[j] -= dv_y * force;
            vel.z[j] -= dv_z * force;

        }

        // write back buffered acceleration of p1
        vel.x[i] += p1_vel_x;
        vel.y[i] += p1_vel_y;
        vel.z[i] += p1_vel_z;

    }

}



void processFramePP(int start, int amount) {
    particle_vectors pos;
    vel_vectors vel;
    particle_t *framebase = state.particleHistory + state.particleCount*state.frame;

    int i;
    int particles_max;
    particles_max = state.particleCount;


    // create arrays, aligned to 16 bytes

    pos.x = (float*) _mm_malloc(sizeof(float)*(particles_max + 16), 16);
    pos.y = (float*) _mm_malloc(sizeof(float)*(particles_max + 16), 16);
    pos.z = (float*) _mm_malloc(sizeof(float)*(particles_max + 16), 16);
    pos.mass = (float*) _mm_malloc(sizeof(float)*(particles_max + 16), 16);
    //memset(pos.x, 0, sizeof(float) * (particles_max + 16));
    //memset(pos.y, 0, sizeof(float) * (particles_max + 16));
    //memset(pos.z, 0, sizeof(float) * (particles_max + 16));
    //memset(pos.mass, 0, sizeof(float) * (particles_max + 16));
    vel.x = (float*) _mm_malloc(sizeof(float)*(particles_max + 16), 16);
    vel.y = (float*) _mm_malloc(sizeof(float)*(particles_max + 16), 16);
    vel.z = (float*) _mm_malloc(sizeof(float)*(particles_max + 16), 16);
    memset(vel.x, 0, sizeof(float) * (particles_max + 16));
    memset(vel.y, 0, sizeof(float) * (particles_max + 16));
    memset(vel.z, 0, sizeof(float) * (particles_max + 16));


    // copy frame data to vector-friendly arrays
    for (i=0; i<particles_max; i++)
	{
	  pos.x[i] = framebase[i].pos[0];
	  pos.y[i] = framebase[i].pos[1];
	  pos.z[i] = framebase[i].pos[2];
	  pos.mass[i] = state.particleDetail[i].mass;
    }


    // calculate new accelerations
    do_processFramePP(pos, vel, start, amount);


    // write back results
    for (i=0; i<particles_max; i++)
    {
	  framebase[i].vel[0] += vel.x[i] * state.g;
	  framebase[i].vel[1] += vel.y[i] * state.g;
	  framebase[i].vel[2] += vel.z[i] * state.g;
    }


    // clean up
    _mm_free(pos.x);
    _mm_free(pos.y);
    _mm_free(pos.z);
    _mm_free(pos.mass);

    _mm_free(vel.x);
    _mm_free(vel.y);
    _mm_free(vel.z);

}
