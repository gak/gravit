/*

Graviton - A gravity simulator
Copyright 2003-2005 Gerald Kaszuba

Graviton is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

Graviton is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Graviton; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/

#include "graviton.h"

spawnVars_t spawnVars;

void spawnDefaults() {

	spawnVars.minGalCount = 2;
	spawnVars.maxGalCount = 10;

	spawnVars.minGalMass = 0;
	spawnVars.maxGalMass = 500;

	spawnVars.minGalSize = 0;
	spawnVars.maxGalSize = 500;

	spawnVars.minGalVel = 0;
	spawnVars.maxGalVel = 50;

	spawnVars.minSpawnRange = 0;
	spawnVars.maxSpawnRange = 10000;

}



void setRangePosition(float *org, float range) {

	float ranged;

	ranged = range * 4;

	while (1) {

		org[0] = frand(-ranged,ranged);
		org[1] = frand(-ranged,ranged);
		org[2] = frand(-ranged,ranged);

		if (sqrt(pow(org[0], 2) + pow(org[1], 2) + pow(org[2], 2)) <= range)
			break;

	}

}

void pickPositions_() {

	particle_t *p;
	particleDetail_t *pd;
	int i;
	float angle;
	float radius;
	float angle2;

	angle = 0;

	for (i = 0; i < state.particleCount; i++) {

		p = getParticleFirstFrame(i);
		pd = getParticleDetail(i);

		// pd->mass = frand(spawnVars.minGalMass, spawnVars.maxGalMass);
		pd->mass = 100;

		angle = frand(0, PI*2);
		radius = frand(spawnVars.minGalSize, spawnVars.maxGalSize);
//		radius = 100;
//		angle += (PI / 2);

		p->pos[0] = cos(angle) * radius;
		p->pos[1] = sin(angle) * radius;
		p->pos[2] = frand(spawnVars.minGalSize, spawnVars.maxGalSize / 2);
		p->pos[2] = 0;

		angle2 = angle + PI / 2;
		p->vel[0] = cos(angle2) * radius * 0.1f;
		p->vel[1] = sin(angle2) * radius * 0.1f;
		p->vel[2] = 0;

		if (i == 0) {

			//p->pos[1] += 1;

		}

//		VectorZero(p->vel);

	}


}

void pickPositions() {

	int gals;

	float galPos[100][3];
	float galVel[100][3];
	float shit[3];

	float galSize[100];
	float galMassMin[100];
	float galMassMax[100];
	float spawnRange;
	int i;
	int g;
	particle_t *p;
	particleDetail_t *pd;
	float totalMass = 0;

	float angle;
	float angle2;
	float radius;

	gals = (rand() % (1 + spawnVars.maxGalCount-spawnVars.minGalCount)) + spawnVars.minGalCount;
	spawnRange = frand(spawnVars.minSpawnRange, spawnVars.maxSpawnRange);

	conAdd(1, "Spawning new simulation...");
	conAdd(0, "- %i particles...", state.particleCount);
	conAdd(0, "- %i galaxies...", gals);

	for (g = 0; g < gals; g++) {

		galMassMin[g] = frand(spawnVars.minGalMass, spawnVars.maxGalMass);
		galMassMax[g] = frand(spawnVars.minGalMass, spawnVars.maxGalMass);
		galSize[g] = frand(spawnVars.minGalSize, spawnVars.maxGalSize);

		setRangePosition(galPos[g], spawnRange);
		setRangePosition(galVel[g], frand(0,1) * frand(0,1) * frand(spawnVars.minGalVel, spawnVars.maxGalVel));

	}

	for (i = 0; i < state.particleCount; i++) {

		p = getParticleFirstFrame(i);
		pd = getParticleDetail(i);

		g = rand() % gals;

		pd->mass = frand(galMassMin[g], galMassMax[g]);

//		if (g % 2 == 0)
//			pd->mass = -pd->mass;

		totalMass += pd->mass;

		// position
		VectorCopy(galPos[g], p->pos);
		setRangePosition((float *)&shit, galSize[g]);
		VectorAdd(p->pos, shit, p->pos);


		// galaxy structured position
		angle = frand(0, PI*2);
		radius = frand(spawnVars.minGalSize, spawnVars.maxGalSize);

		VectorZero(p->pos);

		p->pos[0] = cos(angle) * radius;
		p->pos[1] = sin(angle) * radius;

		VectorAdd(galPos[g], p->pos, p->pos);

		angle2 = angle + PI / 2;

		p->vel[0] = cos(angle2) * radius * 0.02f;
		p->vel[1] = sin(angle2) * radius * 0.02f;
		p->vel[2] = 0;

		VectorAdd(galVel[g], p->vel, p->vel);

		if (g & 2) {
			p->vel[0] = -p->vel[0];
			p->vel[1] = -p->vel[1];
			p->vel[2] = -p->vel[2];
		}

		// velocity
//		VectorCopy(galVel[g], p->vel);
//		setRangePosition(p->vel, frand(0, 100) );
		/*
		if (i == 0) {

			pd->mass = 10000;
			p->pos[0] = 0;
			p->pos[1] = 0;
			p->pos[2] = 0;

			VectorZero(p->vel);

		}
*/

	}

	conAdd(0, "- %f total mass...", totalMass);
	conAdd(0, "- %f galaxy mass...", totalMass / gals);
	conAdd(0, "- %f particle mass...", totalMass / state.particleCount);

}
