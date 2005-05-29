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

spawnVars_t spawnVars;

void spawnDefaults() {

	spawnVars.minGalCount = 3;
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

int pickPositions() {

	int gals;

	VectorNew(galPos[100]);
	VectorNew(galVel[100]);
	VectorNew(shit);

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

	if (gals <= 0) {

		conAdd(LERR, "For some reason galaxies to spawn is 0 or less. Not possible!");
		return 0;

	}

	if (gals >= 100) {

		conAdd(LERR, "Maximum galaxies to spawn is 100");
		return 0;

	}

	spawnRange = frand(spawnVars.minSpawnRange, spawnVars.maxSpawnRange);

	conAdd(LNORM, "Spawning new simulation...");
	conAdd(LLOW, "- %i particles...", state.particleCount);
	conAdd(LLOW, "- %i galaxies...", gals);

	for (g = 0; g < gals; g++) {

		galMassMin[g] = frand(spawnVars.minGalMass, spawnVars.maxGalMass);
		galMassMax[g] = frand(spawnVars.minGalMass, spawnVars.maxGalMass);
		galSize[g] = frand(spawnVars.minGalSize, spawnVars.maxGalSize);

		setRangePosition(galPos[g], spawnRange);
		setRangePosition(galVel[g], frand(0,1) * frand(0,1) * frand(spawnVars.minGalVel, spawnVars.maxGalVel));

	}

	for (i = 0; i < state.particleCount; i++) {

		if (!(i % 100)) {
			view.recordParticlesDone = i;
			doVideoUpdateInSpawn();
		}

		if (state.restartSpawning) {
			return 0;
		}
		
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
		radius = frand(0, galSize[g]);

		VectorZero(p->pos);

		p->pos[0] = cos(angle) * radius;
		p->pos[1] = sin(angle) * radius;
		p->pos[2] = frand(-radius/10, radius/10);

		VectorAdd(galPos[g], p->pos, p->pos);

		angle2 = angle + PI / 2;

		p->vel[0] = cos(angle2) * radius * 0.05f;
		p->vel[1] = sin(angle2) * radius * 0.05f;
		p->vel[2] = 0;

		VectorAdd(galVel[g], p->vel, p->vel);

		if (g & 2) {
			p->vel[0] = -p->vel[0];
			p->vel[1] = -p->vel[1];
			p->vel[2] = -p->vel[2];
		}

	}

	conAdd(LLOW, "- %f total mass...", totalMass);
	conAdd(LLOW, "- %f galaxy mass...", totalMass / gals);
	conAdd(LLOW, "- %f particle mass...", totalMass / state.particleCount);

	return 0;

}

int isSpawning() {

	if (state.currentlySpawning) {
		conAdd(LNORM, "Please wait until all particles are spawned...");
		return 1;
	}

	return 0;

}
