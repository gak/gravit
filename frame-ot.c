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

void otBranchNode(node_t *n);

node_t *r = NULL;
int nodes;
int particles;

void otGetBoundingBox(float *otMin, float *otMax) {

	particle_t *p;
	int i;
	int j;

	p = state.particleHistory + state.particleCount * state.frame;

	VectorCopy(p->pos, otMin);
	VectorCopy(p->pos, otMax);

	for (i = 1; i < state.particleCount; i++) {

		p = state.particleHistory + state.particleCount * state.frame + i;

		for (j = 0; j < 3; j++) {

			if (p->pos[j] < otMin[j])
				otMin[j] = p->pos[j];
			if (p->pos[j] > otMax[j])
				otMax[j] = p->pos[j];

		}

	}

}

int otGetParticleInBox(float *min, float *max, particle_t **gp, float *m, float *cm) {

	int c = 0;
	particle_t *p;
	particleDetail_t *pd;
	int i;

	float vec[3];

	memset(cm, 0, sizeof(float) * 3);

	*gp = NULL;
	*m = 0;

 	for (i = 0; i < state.particleCount; i++) {

		p = state.particleHistory + state.particleCount * state.frame + i;
		pd = state.particleDetail + i;

		if (p->pos[0] < min[0])
			continue;
		if (p->pos[0] > max[0])
			continue;
		if (p->pos[1] < min[1])
			continue;
		if (p->pos[1] > max[1])
			continue;
		if (p->pos[2] < min[2])
			continue;
		if (p->pos[2] > max[2])
			continue;

		*m += pd->mass;
        *gp = p;
		VectorCopy(p->pos, vec);
		VectorMultiply(vec, pd->mass, vec);
		VectorAdd(cm, vec, cm);

		c++;

	}

	VectorDivide(cm, *m, cm);

	return c;

}

void otBranchNodeCorner(node_t *n, int br, float *min, float *max) {

	float mass;
	particle_t *p;

	node_t *b;
	int c;

	float cm[3];

	c = otGetParticleInBox(min, max, &p, &mass, (float*)&cm);

	if (c == 0)
		return;

	nodes ++;
	n->b[br] = _aligned_malloc(sizeof(node_t), 16);
	b = (node_t *)n->b[br];

	memset(b, 0, sizeof(node_t));

	VectorCopy(min, b->min);
	VectorCopy(max, b->max);
	VectorCopy(cm, b->cm);

	// Gets Center of min/max
	VectorSub(b->max, b->min, b->c);
	VectorDivide(b->c, 2, b->c);
	VectorAdd(b->c, b->min, b->c);

	// Get Length of node
	distance(b->min, b->max, b->length);

	b->mass = mass;

	if (c == 1) {

		particles ++;
		b->p = p;
		VectorCopy(p->pos, b->cm);

	} else {

		otBranchNode(b);

	}

}

void otBranchNode(node_t *n) {

	float min[3];
	float max[3];

	// b[0]: top left front
	min[0] = n->min[0];
	max[0] = n->c[0];
	min[1] = n->min[1];
	max[1] = n->c[1];
	min[2] = n->min[2];
	max[2] = n->c[2];
	otBranchNodeCorner(n, 0, (float*)&min, (float*)&max);

	// b[1]: top right front
	min[0] = n->c[0];
	max[0] = n->max[0];
	otBranchNodeCorner(n, 1, (float*)&min, (float*)&max);

	// b[2]: bottom right front
	min[1] = n->c[1];
	max[1] = n->max[1];
	otBranchNodeCorner(n, 2, (float*)&min, (float*)&max);

	// b[3]: bottom left front
	min[0] = n->min[0];
	max[0] = n->c[0];
	otBranchNodeCorner(n, 3, (float*)&min, (float*)&max);

	// b[4]: top left back
	min[1] = n->min[1];
	max[1] = n->c[1];
	min[2] = n->c[2];
	max[2] = n->max[2];
	otBranchNodeCorner(n, 4, (float*)&min, (float*)&max);

	// b[5]: top right back
	min[0] = n->c[0];
	max[0] = n->max[0];
	otBranchNodeCorner(n, 5, (float*)&min, (float*)&max);

	// b[6]: bottom right back
	min[1] = n->c[1];
	max[1] = n->max[1];
	otBranchNodeCorner(n, 6, (float*)&min, (float*)&max);

	// b[7]: bottom left back
	min[0] = n->min[0];
	max[0] = n->c[0];
	otBranchNodeCorner(n, 7, (float*)&min, (float*)&max);

}

void otMakeTree() {

	particle_t *p;
	node_t *n;

	// make root node
    r = _aligned_malloc(sizeof(node_t), 16);
	memset(r, 0, sizeof(node_t));
	nodes = 1;
	particles = 0;

	n = r;

	otGetBoundingBox((float*)&n->min, (float*)&n->max);
	otGetParticleInBox((float*)&n->min, (float*)&n->max, &p, &n->mass, (float*)&n->cm);

	// Gets Center of min/max
	VectorSub(n->max, n->min, n->c);
	VectorDivide(n->c, 2, n->c);
	VectorAdd(n->c, n->min, n->c);

	// get length
    distance(n->min, n->max, n->length);

	otBranchNode(n);

}

void otFreeTreeRecursive(node_t *n) {

	int i;

	if (!n)
		return;

	for (i = 0; i < 8; i++) {

		if (n->b[i]) {
			otFreeTreeRecursive((node_t *)n->b[i]);
			n->b[i] = 0;
		}

	}

	_aligned_free(n);
	nodes--;

}

void otFreeTree() {

	otFreeTreeRecursive(r);
	r = NULL;

}

#ifndef NO_GUI

void otDrawTreeRecursive(node_t *n) {

	int i;

	if (!n)
		return;

	if (n->p) {
		glColor3f(1.0f, 0, 0);
	} else {
		glColor3f(0.5f, 0, 0);
	}

	glPushMatrix();
	glTranslatef(n->c[0], n->c[1], n->c[2]);
	glScalef(n->max[0] - n->min[0], n->max[1] - n->min[1], n->max[2] - n->min[2]);
	
	glutWireCube(1);
	glPopMatrix();

	for (i = 0; i < 8; i++)
		if (n->b[i])
			otDrawTreeRecursive((node_t *)n->b[i]);

}

void otDrawTree() {

	glDepthMask(GL_FALSE);
	glDisable(GL_BLEND);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	otDrawTreeRecursive(r);

}

#endif

void otComputeParticleToTreeRecursive(pttr_t *info) {

	int i;
	node_t *b;
	particle_t *p2;

	float d;
	float poo;

	if (info->n->p == info->p)
		return;

	if (info->n->p) {

		p2 = info->n->p;

		distance2(info->p->pos, p2->pos, d);

// 		frDoGravity(p,n,d); was

		{ // now
			float dv[3];
			float force;
			dv[0] = info->p->pos[0] - info->n->cm[0];
			dv[1] = info->p->pos[1] - info->n->cm[1];
			dv[2] = info->p->pos[2] - info->n->cm[2];

			if (d) {
				force = -0.00001f * info->pd->mass * info->n->mass / d;
				info->p->vel[0] += dv[0] * force;
				info->p->vel[1] += dv[1] * force;
				info->p->vel[2] += dv[2] * force;
			}
		}

	} else {

		for (i = 0; i < 8; i++) {

			if (!info->n->b[i])
				continue;

			b = (node_t *)info->n->b[i];

			distance2(info->p->pos, b->cm, d);

			if (!d)
				continue;

			poo = b->length / (float)sqrt(d);

			if ( poo > 0.5f ) {
				pttr_t info2;
				info2.n = b;
				info2.p = info->p;
				info2.pd = info->pd;
				otComputeParticleToTreeRecursive(&info2);

			} else {

				// frDoGravity(p,b,d);

				{ // now
					float dv[3];
					float force;
					dv[0] = info->p->pos[0] - b->cm[0];
					dv[1] = info->p->pos[1] - b->cm[1];
					dv[2] = info->p->pos[2] - b->cm[2];

					if (d) {
						force = -0.00001f * info->pd->mass * b->mass / d;
						info->p->vel[0] += dv[0] * force;
						info->p->vel[1] += dv[1] * force;
						info->p->vel[2] += dv[2] * force;
					}
				}

			}

		}

	}

}

void processFrameOT(int start, int amount) {

	int i;
	particle_t *p;
	particleDetail_t *pd;
	pttr_t info;
	
	otMakeTree();

	for (i = start; i < amount; i++) {

		p = state.particleHistory + state.particleCount * state.frame + i;
		pd = state.particleDetail + i;

		if (!p)
			continue;

		info.p = p;
		info.n = r;
		info.pd = pd;

		otComputeParticleToTreeRecursive(&info);

	}

}
