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

void setColorsByVel() {

	int i;
	particle_t *p;
	particleDetail_t *pd;
	float d;
	float velMax = 0;
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
