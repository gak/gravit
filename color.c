#include "graviton.h"

void setColorsByVel() {

	int i;
	particle_t *p;
	particleDetail_t *pd;
	float d;
	float velMax;
	float velSpeed;

	VectorNew(zero);
	VectorZero(zero);

	for (i = 0; i < state.particleCount; i++) {

		p = getParticleCurrentFrame(i);

		pd = getParticleDetail(i);
		distance(zero, p->vel, velSpeed);

		if (velSpeed < 0)
			conAdd(1, "VelSpeed < 0!");

		if (i == 0) {
		
			velMax = velSpeed;

		} else {

			if (velSpeed > velMax)
				velMax = velSpeed;

		}

	}

	for (i = 0; i < state.particleCount; i++) {

		p = getParticleFirstFrame(i);
		pd = getParticleDetail(i);
		
		distance(zero, p->vel, velSpeed);

		d = velSpeed / velMax;
		gfxNormalToRGB(pd->col, (float)fabs((double)d));

	}

}

void setColorsByMass() {

	int i;
	particle_t *p;
	particleDetail_t *pd;
	float d;

	for (i = 0; i < state.particleCount; i++) {

		p = getParticleCurrentFrame(i);
		pd = getParticleDetail(i);

		if (i == 0) {
		
			state.massRange[0] = pd->mass;
			state.massRange[1] = pd->mass;

		} else {

			if (pd->mass < state.massRange[0])
				state.massRange[0] = pd->mass;

			if (pd->mass > state.massRange[1])
				state.massRange[1] = pd->mass;

		}

	}

	for (i = 0; i < state.particleCount; i++) {

		p = getParticleCurrentFrame(i);
		pd = getParticleDetail(i);
		
		d = pd->mass / state.massRange[1];
		gfxNormalToRGB(pd->col, (float)fabs(d));
/*
		if (i == 0) {

			pd->col[0] = pd->col[1] = pd->col[2] = 1;
			pd->col[3] = 0.5f;

		} else {

			pd->col[0] = 1;
			pd->col[1] = 0.5f;
			pd->col[2] = 0;
			pd->col[3] = 0.5f;

		}
*/

		if (d < 0) {

			float s;

			s = pd->col[0];
			pd->col[0] = pd->col[1];
			pd->col[1] = s;

		}

	}

}

void setColors() {

	switch (view.particleColorMode) {
		
		case 1: setColorsByMass(); break;
		default: setColorsByVel(); break;

	}

}
