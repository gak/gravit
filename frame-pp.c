#include "graviton.h"

#if 0
void processFramePP(int start, int amount) {

	int i,j;
	particle_t *p1;
	particle_t *p2;

	float dv[3];
	float dv2[3];

	float inverseSquareDistance;
	float force;

	// apply gravity to every specified velocity
	for (i = start; i < amount; i++) {

		p1 = state.particleHistory + state.particleCount*state.frame + i;

		for (j = 0; j < state.particleCount; j++) {

			if (i == j) {

				continue;

			}

			p2 = state.particleHistory + state.particleCount*state.frame + j;
			
			dv[0] = p1->pos[0] - p2->pos[0];
			dv[1] = p1->pos[1] - p2->pos[1];
			dv[2] = p1->pos[2] - p2->pos[2];

			dv2[0] = pow(dv[0], 2);
			dv2[1] = pow(dv[1], 2);
			dv2[2] = pow(dv[2], 2);

			// get distance^2 between the two
			inverseSquareDistance = dv2[0] + dv2[1] + dv2[2];
			
			force = -0.00001f * p1->mass * p2->mass / inverseSquareDistance;

			p1->vel[0] += dv[0] * force;
			p1->vel[1] += dv[1] * force;
			p1->vel[2] += dv[2] * force;

		}

	}

}
#endif