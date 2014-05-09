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

#ifdef _OPENMP
// experimental OMP support
#include <omp.h> // VC has to include this header to build the correct manifest to find vcom.dll or vcompd.dll
#endif


  /*  Optimizations:
      ===============

      1. make use of the fact that the force between p1->p2 and p2->p1 is the same (except sign)
         --> saves 50% of all loop iterations !!

      2. get rid of pow(x,2) function -- its incredibly slow

      3. use buffers for p1->pos, pd1->vel, pd1->mass --> save memory bandwidth 
         (esp. writes) in inner loop

      4. moved declarations into scope of for loops (to help the compiler optimize stuff)
         .. if your compiler does not like this, just move all declarations back to the top of the function.

	 5. experimental: add OMP directives (compile with gcc -fopenmp) to execute the outer loop with parallel threads.
		To control the number of thread, we could add "omp_set_num_threads(state.processFrameThreads);" somewhere
		before "#pragma omp parallel for" 
  */

// optional - min. distance squared -- used as smoothing length, 
//            to avoid the "stars shooting out of the galaxy" effect
//#define MIN_STEP2 0.05


void processFramePP(int start, int amount) {

    int i;
    int particles_max = state.particleCount;

    // apply gravity to every specified velocity

#ifdef _OPENMP
    // experimental: multi-treading with OMP. Do NOT mix with pthread (frame.c processFrameThread)
    #pragma omp parallel for schedule(dynamic, 256)
#endif
    for (i = start; i < amount; i++) {
        VectorNew(p1_pos);
        VectorNew(p1_acc);
        float p1_mass;
        
        particle_t *p1;
        particleDetail_t *pd1;
        int j;
        
        p1 = state.particleHistory + state.particleCount*state.frame + i;
        pd1 = state.particleDetail + i;
        
        p1_mass = pd1->mass;
        VectorCopy(p1->pos, p1_pos);
        VectorZero(p1_acc);
        
        for (j = i + 1; j < particles_max; j++) {
            
            VectorNew(dv);
            float inverseSquareDistance;
            float force;
            
            particle_t *p2;
            particleDetail_t *pd2;
            
            p2 = state.particleHistory + state.particleCount*state.frame + j;
            pd2 = state.particleDetail + j;
            
            dv[0] = p1_pos[0] - p2->pos[0];
            dv[1] = p1_pos[1] - p2->pos[1];
            dv[2] = p1_pos[2] - p2->pos[2];
            
            // get distance^2 between the two
            inverseSquareDistance  = dv[0] * dv[0];
            inverseSquareDistance += dv[1] * dv[1];
            inverseSquareDistance += dv[2] * dv[2];
            //inverseSquareDistance +=  + MIN_STEP2;
            
            force = state.g * p1_mass * pd2->mass / inverseSquareDistance;
            
            // sum of accelerations for p1
            p1_acc[0] += dv[0] * force;
            p1_acc[1] += dv[1] * force;
            p1_acc[2] += dv[2] * force;
            
            // add acceleration for p2 (with negative sign, as the direction is inverted)
            pd2->accel[0] += -dv[0] * force;
            pd2->accel[1] += -dv[1] * force;
            pd2->accel[2] += -dv[2] * force;
            
        }
        
        // write back buffered acceleration of p1
        pd1->accel[0] += p1_acc[0];
        pd1->accel[1] += p1_acc[1];
        pd1->accel[2] += p1_acc[2];
        
    }
    
}

