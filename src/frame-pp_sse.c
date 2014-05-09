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

#ifdef HAVE_SSE

#ifdef _OPENMP
#include <omp.h>
#endif


// __restrict__ tell the compiler that two pointer will not point to the same location
// if your compiler complains, just remove __restrict__

#ifdef _MSC_VER
#define __restrict__ __restrict
//#define __restrict__
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
} acc_vectors;


#include "sse_functions.h"


/*  Optimizations:
    ===============
    * before processing, copy particle data to vector-friendly arrays
    * use SSE to process four particles at once
    * delay multiplication with G
    * after processing, write back results
*/


#define MIN_STEP2 0.05f
static const __v128 vmin_step2 = _mm_init1_ps(MIN_STEP2);

HOT
static void do_processFramePP_SSE(particle_vectors pos, acc_vectors accel,
                                  int start, int amount) {
    int i;

    // apply gravity to every specified velocity
#ifdef _OPENMP
#pragma omp parallel for schedule(dynamic, 256)
#endif
    for (i = start; i < amount; i++) {
        __v128 p1_vpos_x ;
        __v128 p1_vpos_y ;
        __v128 p1_vpos_z ;
        __v128 p1_vmass  ;

        __v128 p1_vaccel_x  = _mm_init1_ps(0.0f);
        __v128 p1_vaccel_y  = _mm_init1_ps(0.0f);
        __v128 p1_vaccel_z  = _mm_init1_ps(0.0f);

        //VectorNew(p1_pos);
        //VectorNew(p1_vel);
        //VectorZero(p1_vel);

        float p1_pos_x;
        float p1_pos_y;
        float p1_pos_z;
        float p1_mass;

        float p1_accel_x = 0.0f;
        float p1_accel_y = 0.0f;
        float p1_accel_z = 0.0f;


        int j;
        int vector_limit;
        vector_limit = (i / VECT_SIZE) * VECT_SIZE;  // round down to value divisible by 4

        p1_vpos_x = _mm_set1_ps(pos.x[i]);
        p1_vpos_y = _mm_set1_ps(pos.y[i]);
        p1_vpos_z = _mm_set1_ps(pos.z[i]);
        p1_vmass  = _mm_set1_ps(pos.mass[i]);


        // SSE loop - four particles at once
#ifdef __INTEL_COMPILER
#pragma vector aligned
#endif
        for (j = 0; j < vector_limit; j += VECT_SIZE) {
            __v128 dv_vx ;
            __v128 dv_vy ;
            __v128 dv_vz ;
            __v128 vInvSqDist;
            __v128 vforce;

            dv_vx = V_SUB( p1_vpos_x, LOAD_V4(pos.x, j));
            dv_vy = V_SUB( p1_vpos_y, LOAD_V4(pos.y, j));
            dv_vz = V_SUB( p1_vpos_z, LOAD_V4(pos.z, j));

            // get distance^2 between the two
            vInvSqDist  = V_MUL( dv_vx, dv_vx);
            V_INCR( vInvSqDist, V_MUL( dv_vy, dv_vy));
            V_INCR( vInvSqDist, V_MUL( dv_vz, dv_vz));
            V_INCR( vInvSqDist, vmin_step2);

            /* compute acceleration */
            vforce = V_MUL( V_MUL( p1_vmass, LOAD_V4(pos.mass, j)), newtonrapson_rcp(vInvSqDist));

            // sum of accelerations for p1
            V_INCR( p1_vaccel_x, V_MUL( dv_vx, vforce));
            V_INCR( p1_vaccel_y, V_MUL( dv_vy, vforce));
            V_INCR( p1_vaccel_z, V_MUL( dv_vz, vforce));

            // add acceleration for p2 (with negative sign, as the direction is inverted)
            SUB_V4(accel.x, j, V_MUL( dv_vx, vforce));
            SUB_V4(accel.y, j, V_MUL( dv_vy, vforce));
            SUB_V4(accel.z, j, V_MUL( dv_vz, vforce));

        }


        // cache p1 data
        p1_pos_x = pos.x[i];
        p1_pos_y = pos.y[i];
        p1_pos_z = pos.z[i];
        p1_mass   = pos.mass[i];

        // copy vector results  to single floats
        p1_accel_x = _vector4_sum(p1_vaccel_x);
        p1_accel_y = _vector4_sum(p1_vaccel_y);
        p1_accel_z = _vector4_sum(p1_vaccel_z);


        // do the remaining particles without SSE
        for (j = vector_limit; j < i; j++) {
            VectorNew(dv);
            float inverseSquareDistance;
            float force;

            dv[0] = p1_pos_x - pos.x[j];
            dv[1] = p1_pos_y - pos.y[j];
            dv[2] = p1_pos_z - pos.z[j];

            // get distance^2 between the two
            inverseSquareDistance  = dv[0] * dv[0];
            inverseSquareDistance += dv[1] * dv[1];
            inverseSquareDistance += dv[2] * dv[2];
            inverseSquareDistance +=  + MIN_STEP2;

            /* compute acceleration */
            force = p1_mass * pos.mass[j] / inverseSquareDistance;

            // sum of accelerations for p1
            p1_accel_x += dv[0] * force;
            p1_accel_y += dv[1] * force;
            p1_accel_z += dv[2] * force;

            // add acceleration for p2 (with negative sign, as the direction is inverted)
            accel.x[j] -= dv[0] * force;
            accel.y[j] -= dv[1] * force;
            accel.z[j] -= dv[2] * force;
        }


        // write back buffered acceleration of p1
        accel.x[i] += p1_accel_x;
        accel.y[i] += p1_accel_y;
        accel.z[i] += p1_accel_z;

    }

}



void processFramePP_SSE(int start, int amount) {
    particle_vectors pos;
    acc_vectors accel;
    particle_t *framebase = state.particleHistory + state.particleCount*state.frame;
    particleDetail_t * __restrict__ framedetail = state.particleDetail;

    int i;
    int particles_max;
    particles_max = state.particleCount;


    // create arrays aligned to 64 bytes  (cache line lize)
    pos.x    = (float*) _mm_malloc(sizeof(float)*(particles_max + 64), 64);
    pos.y    = (float*) _mm_malloc(sizeof(float)*(particles_max + 64), 64);
    pos.z    = (float*) _mm_malloc(sizeof(float)*(particles_max + 64), 64);
    pos.mass = (float*) _mm_malloc(sizeof(float)*(particles_max + 64), 64);

    accel.x = (float*) _mm_malloc(sizeof(float)*(particles_max + 64), 64);
    accel.y = (float*) _mm_malloc(sizeof(float)*(particles_max + 64), 64);
    accel.z = (float*) _mm_malloc(sizeof(float)*(particles_max + 64), 64);

    memset(accel.x, 0, sizeof(float) * (particles_max + 64));
    memset(accel.y, 0, sizeof(float) * (particles_max + 64));
    memset(accel.z, 0, sizeof(float) * (particles_max + 64));


    // copy frame data to vector-friendly arrays
    for (i=0; i<particles_max; i++) {
        pos.x[i] = framebase[i].pos[0];
        pos.y[i] = framebase[i].pos[1];
        pos.z[i] = framebase[i].pos[2];
        pos.mass[i] = state.particleDetail[i].mass;
    }


    // calculate new accelerations
    do_processFramePP_SSE(pos, accel, start, amount);


    // write back results
    for (i=0; i<particles_max; i++) {
        framedetail[i].accel[0] += accel.x[i] * state.g;
        framedetail[i].accel[1] += accel.y[i] * state.g;
        framedetail[i].accel[2] += accel.z[i] * state.g;
    }


    // clean up
    _mm_free(pos.x);
    _mm_free(pos.y);
    _mm_free(pos.z);
    _mm_free(pos.mass);

    _mm_free(accel.x);
    _mm_free(accel.y);
    _mm_free(accel.z);

}

#else
#pragma message( __FILE__ " : warning : define HAVE_SSE  to enable SSE support." )
#endif
