#include "graviton.h"

spawnVars_t spawnVars;

void spawnDefaults() {

	spawnVars.minGalCount = 1;
	spawnVars.maxGalCount = 10;

	spawnVars.minGalMass = 50;
	spawnVars.maxGalMass = 500;

	spawnVars.minGalSize = 500;
	spawnVars.maxGalSize = 500;

	spawnVars.minGalVel = 0;
	spawnVars.maxGalVel = 500;

	spawnVars.minSpawnRange = 0;
	spawnVars.maxSpawnRange = 10000;

}



#ifdef GRAV_SSE
void setRangePosition(__m128 *org, float range) {
#else
void setRangePosition(float *org, float range) {
#endif

	float ranged;

	ranged = range * 4;

	while (1) {

#ifdef GRAV_SSE

		org->m128_f32[0] = frand(-ranged,ranged);
		org->m128_f32[1] = frand(-ranged,ranged);
		org->m128_f32[2] = frand(-ranged,ranged);

		if (sqrt(pow(org->m128_f32[0], 2) + pow(org->m128_f32[1], 2) + pow(org->m128_f32[2], 2)) <= range)
			break;

#else
		org[0] = frand(-ranged,ranged);
		org[1] = frand(-ranged,ranged);
		org[2] = frand(-ranged,ranged);

		if (sqrt(pow(org[0], 2) + pow(org[1], 2) + pow(org[2], 2)) <= range)
			break;
#endif

	}

}

void pickPositions() {

	int gals;

#ifdef GRAV_SSE

	__m128 galPos[100];
	__m128 galVel[100];
	__m128 shit;

#else

	float galPos[100][3];
	float galVel[100][3];
	float shit[3];

#endif

	float galSize[100];
	float galMassMin[100];
	float galMassMax[100];
	float spawnRange;
	int i;
	int g;
	particle_t *p;
	particleDetail_t *pd;
	float totalMass = 0;

	gals = (rand() % (1 + spawnVars.maxGalCount-spawnVars.minGalCount)) + spawnVars.minGalCount;
	spawnRange = frand(spawnVars.minSpawnRange, spawnVars.maxSpawnRange);

	conAdd(1, "Spawning new simulation...");
	conAdd(0, "- %i particles...", state.particleCount);
	conAdd(0, "- %i galaxies...", gals);

	for (g = 0; g < gals; g++) {

		galMassMin[g] = frand(spawnVars.minGalMass, spawnVars.maxGalMass);
		galMassMax[g] = frand(spawnVars.minGalMass, spawnVars.maxGalMass);
		galSize[g] = frand(spawnVars.minGalSize, spawnVars.maxGalSize);
		
#ifdef GRAV_SSE
		setRangePosition(&galPos[g], spawnRange);
		setRangePosition(&galVel[g], frand(0,1) * frand(0,1) * frand(spawnVars.minGalVel, spawnVars.maxGalVel));
#else
		setRangePosition(galPos[g], spawnRange);
		setRangePosition(galVel[g], frand(0,1) * frand(0,1) * frand(spawnVars.minGalVel, spawnVars.maxGalVel));
#endif

	}

	for (i = 0; i < state.particleCount; i++) {

		p = getParticleFirstFrame(i);
		pd = getParticleDetail(i);
		g = rand() % gals;

		pd->mass = frand(galMassMin[g], galMassMax[g]);
		totalMass += pd->mass;

		// position
		VectorCopy(galPos[g], p->pos);
#ifdef GRAV_SSE
		setRangePosition(&shit, galSize[g]);
#else
		setRangePosition((float *)&shit, galSize[g]);
#endif
		VectorAdd(p->pos, shit, p->pos);

		// velocity
//		VectorCopy(galVel[g], p->vel);
//		setRangePosition(p->vel, frand(0, 100) );
/*
		if (i == 0) {

			pd->mass = 100000;
			VectorZero(p->pos);
			VectorZero(p->vel);

		}
*/
		
	}

	conAdd(0, "- %f total mass...", totalMass);
	conAdd(0, "- %f galaxy mass...", totalMass / gals);
	conAdd(0, "- %f particle mass...", totalMass / state.particleCount);

}
