 #include "graviton.h"


void otBranchNode(node_t *n);

node_t *r = NULL;
int nodes;
int particles;

int ncalcs;
int scalcs;
int pcalcs;

#ifdef GRAV_SSE
void otGetBoundingBox(__m128 *otMin, __m128 *otMax) {
#else
void otGetBoundingBox(float *otMin, float *otMax) {
#endif

	particle_t *p;
	int i;
	int j;

	p = state.particleHistory + state.particleCount * state.frame;
	/*
	otMin[0] = p->pos[0];
	otMax[0] = p->pos[0];
	otMin[1] = p->pos[1];
	otMax[1] = p->pos[1];
	otMin[2] = p->pos[2];
	otMax[2] = p->pos[2];
	*/

#ifdef GRAV_SSE

	VectorCopy(p->pos, *otMin);
	VectorCopy(p->pos, *otMax);

#else

	VectorCopy(p->pos, otMin);
	VectorCopy(p->pos, otMax);

#endif

	for (i = 1; i < state.particleCount; i++) {

#ifdef GRAV_SSE
		__m128 m1,m2;
#endif

		p = state.particleHistory + state.particleCount * state.frame + i;

#ifdef GRAV_SSE

		m1 = _mm_cmplt_ps(p->pos, *otMin);
		m2 = _mm_cmpgt_ps(p->pos, *otMax);

		for (j = 0; j < 3; j++) {

			if (m1.m128_f32[j])
				otMin->m128_f32[j] = p->pos.m128_f32[j];
			if (m2.m128_f32[j])
				otMax->m128_f32[j] = p->pos.m128_f32[j];

		}

#else

		for (j = 0; j < 3; j++) {

			if (p->pos[j] < otMin[j])
				otMin[j] = p->pos[j];
			if (p->pos[j] > otMax[j])
				otMax[j] = p->pos[j];

		}

#endif

	}

}
#ifdef GRAV_SSE
int otGetParticleInBox(__m128 *min, __m128 *max, particle_t **gp, float *m, __m128 *cm) {
#else
int otGetParticleInBox(float *min, float *max, particle_t **gp, float *m, float *cm) {
#endif

	int c = 0;
	particle_t *p;
	particleDetail_t *pd;
	int i;

#ifdef GRAV_SSE
	__m128 vec;
#else
	float vec[3];
#endif

	memset(cm, 0, sizeof(float) * 3);

	*gp = NULL;
	*m = 0;

 	for (i = 0; i < state.particleCount; i++) {

		p = state.particleHistory + state.particleCount * state.frame + i;
		pd = state.particleDetail + i;

#ifdef GRAV_SSE

		if (p->pos.m128_f32[0] < min->m128_f32[0])
			continue;
		if (p->pos.m128_f32[0] > max->m128_f32[0])
			continue;
		if (p->pos.m128_f32[1] < min->m128_f32[1])
			continue;
		if (p->pos.m128_f32[1] > max->m128_f32[1])
			continue;
		if (p->pos.m128_f32[2] < min->m128_f32[2])
			continue;
		if (p->pos.m128_f32[2] > max->m128_f32[2])
			continue;

#else

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

#endif

		*m += pd->mass;
        *gp = p;
		VectorCopy(p->pos, vec);
#ifdef GRAV_SSE
		VectorMultiply(vec, pd->mass, vec);
		VectorAdd(*cm, vec, *cm);
#else
		VectorMultiply(vec, pd->mass, vec);
		VectorAdd(cm, vec, cm);
#endif
		c++;

	}
#ifdef GRAV_SSE
	VectorDivide(*cm, *m, *cm);
#else
	VectorDivide(cm, *m, cm);
#endif
	return c;

}

#ifdef GRAV_SSE
void otBranchNodeCorner(node_t *n, int br, __m128 *min, __m128 *max) {
#else
void otBranchNodeCorner(node_t *n, int br, float *min, float *max) {
#endif

	float mass;
	particle_t *p;

	node_t *b;
	int c;

#ifdef GRAV_SSE
	__m128 cm;
#else
	float cm[3];
#endif

#ifdef GRAV_SSE
	c = otGetParticleInBox(min, max, &p, &mass, &cm);
#else
	c = otGetParticleInBox(min, max, &p, &mass, (float*)&cm);
#endif

	if (c == 0)
		return;

	nodes ++;
	n->b[br] = _aligned_malloc(sizeof(node_t), 16);
	b = (node_t *)n->b[br];

	memset(b, 0, sizeof(node_t));
//	memcpy(b->min, min, sizeof(b->min));
//	memcpy(b->max, max, sizeof(b->max));
//	memcpy(b->cm, cm, sizeof(b->cm));

#ifdef GRAV_SSE

	VectorCopy(*min, b->min);
	VectorCopy(*max, b->max);
	VectorCopy(cm, b->cm);

#else

	VectorCopy(min, b->min);
	VectorCopy(max, b->max);
	VectorCopy(cm, b->cm);

#endif

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

#ifdef GRAV_SSE

void otBranchNode(node_t *n) {

	__m128 min;
	__m128 max;

//	n->depth ++;

	// b[0]: top left front
	min.m128_f32[0] = n->min.m128_f32[0];
	max.m128_f32[0] = n->c.m128_f32[0];
	min.m128_f32[1] = n->min.m128_f32[1];
	max.m128_f32[1] = n->c.m128_f32[1];
	min.m128_f32[2] = n->min.m128_f32[2];
	max.m128_f32[2] = n->c.m128_f32[2];
	otBranchNodeCorner(n, 0, &min, &max);

	// b[1]: top right front
	min.m128_f32[0] = n->c.m128_f32[0];
	max.m128_f32[0] = n->max.m128_f32[0];
	otBranchNodeCorner(n, 1, &min, &max);

	// b[2]: bottom right front
	min.m128_f32[1] = n->c.m128_f32[1];
	max.m128_f32[1] = n->max.m128_f32[1];
	otBranchNodeCorner(n, 2, &min, &max);

	// b[3]: bottom left front
	min.m128_f32[0] = n->min.m128_f32[0];
	max.m128_f32[0] = n->c.m128_f32[0];
	otBranchNodeCorner(n, 3, &min, &max);

	// b[4]: top left back
	min.m128_f32[1] = n->min.m128_f32[1];
	max.m128_f32[1] = n->c.m128_f32[1];
	min.m128_f32[2] = n->c.m128_f32[2];
	max.m128_f32[2] = n->max.m128_f32[2];
	otBranchNodeCorner(n, 4, &min, &max);

	// b[5]: top right back
	min.m128_f32[0] = n->c.m128_f32[0];
	max.m128_f32[0] = n->max.m128_f32[0];
	otBranchNodeCorner(n, 5, &min, &max);

	// b[6]: bottom right back
	min.m128_f32[1] = n->c.m128_f32[1];
	max.m128_f32[1] = n->max.m128_f32[1];
	otBranchNodeCorner(n, 6, &min, &max);

	// b[7]: bottom left back
	min.m128_f32[0] = n->min.m128_f32[0];
	max.m128_f32[0] = n->c.m128_f32[0];
	otBranchNodeCorner(n, 7, &min, &max);

}

#else

void otBranchNode(node_t *n) {

	float min[3];
	float max[3];

//	n->depth ++;

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

#endif

void otMakeTree() {

	particle_t *p;
	node_t *n;

	// make root node
    r = _aligned_malloc(sizeof(node_t), 16);
	memset(r, 0, sizeof(node_t));
	nodes = 1;
	particles = 0;

	n = r;

#ifdef GRAV_SSE
	otGetBoundingBox(&n->min, &n->max);
	otGetParticleInBox(&n->min, &n->max, &p, &n->mass, &n->cm);
#else
	otGetBoundingBox((float*)&n->min, (float*)&n->max);
	otGetParticleInBox((float*)&n->min, (float*)&n->max, &p, &n->mass, (float*)&n->cm);
#endif


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
#ifdef GRAV_SSE
	glTranslatef(n->c.m128_f32[0], n->c.m128_f32[1], n->c.m128_f32[2]);
	glScalef(n->max.m128_f32[0] - n->min.m128_f32[0], n->max.m128_f32[1] - n->min.m128_f32[1], n->max.m128_f32[2] - n->min.m128_f32[2]);
#else
	glTranslatef(n->c[0], n->c[1], n->c[2]);
	glScalef(n->max[0] - n->min[0], n->max[1] - n->min[1], n->max[2] - n->min[2]);
#endif
	
	glutWireCube(1);
	//glutWireSphere(1, 8, 8);
	glPopMatrix();

/*
	if (n->p) {
		glColor3f(0, 1.0f, 0);
	} else {
		glColor3f(0, 0.5f, 0);
	}

	glPushMatrix();
	glTranslatef(n->cm[0], n->cm[1], n->cm[2]);
	glScalef(n->max[0] - n->min[0], n->max[1] - n->min[1], n->max[2] - n->min[2]);
	// glScalef(0.5f, 0.5f, 0.5f);
	glutWireCube(1);
	//glutWireSphere(1, 8, 8);
	glPopMatrix();
*/

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
// particle_t *p, node_t *n, particleDetail_t *pd) {

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

		ncalcs++;
		pcalcs++;

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
//				otComputeParticleToTreeRecursive(info->p, b, info->pd);
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

				ncalcs++;
				scalcs++;

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

	ncalcs = pcalcs = scalcs = 0;

	for (i = start; i < amount; i++) {

		p = state.particleHistory + state.particleCount * state.frame + i;
		pd = state.particleDetail + i;

		if (!p)
			continue;

		info.p = p;
		info.n = r;
		info.pd = pd;
		//otComputeParticleToTreeRecursive(p, r, pd);

		otComputeParticleToTreeRecursive(&info);

	}

}

#ifndef NO_GUI

void otDrawGravityLinesRecursive(particle_t *p, node_t *n) {

	int i;
	node_t *b;
	particle_t *p2;

	float d;
	float poo;

	if (n->p == p)
		return;

	if (n->p) {

		p2 = n->p;

		glColor3f(1,0,0);
		glBegin(GL_LINES);
//		glVertex3fv(p->pos);
//		glVertex3fv(n->cm);
		glEnd();

		/*
		glPushMatrix();
		glTranslatef(n->c[0], n->c[1], n->c[2]);
		glScalef(n->max[0] - n->min[0], n->max[1] - n->min[1], n->max[2] - n->min[2]);
		glutWireCube(1);
		glPopMatrix();
		*/

	} else {

		for (i = 0; i < 8; i++) {

			if (!n->b[i])
				continue;

			b = (node_t *)n->b[i];

			distance2(p->pos, b->cm, d);

			if (!d)
				continue;

			poo = b->length / sqrt(d);

			if ( poo > 0.5f ) {

				otDrawGravityLinesRecursive(p, b);

			} else {


				glColor3f(0.5f,0,0);

				/*glBegin(GL_LINES);
				glVertex3fv(p->pos);
				glVertex3fv(b->cm);
				glEnd();
				*/

				glPushMatrix();
#ifdef GRAV_SSE
				glTranslatef(b->c.m128_f32[0], b->c.m128_f32[1], b->c.m128_f32[2]);
				glScalef(b->max.m128_f32[0] - b->min.m128_f32[0], b->max.m128_f32[1] - b->min.m128_f32[1], b->max.m128_f32[2] - b->min.m128_f32[2]);
#else
				glTranslatef(b->c[0], b->c[1], b->c[2]);
				glScalef(b->max[0] - b->min[0], b->max[1] - b->min[1], b->max[2] - b->min[2]);

#endif
				//glutWireCube(1);
				glPopMatrix();

			}

		}

	}
}

void otDrawGravityLines() {

	particle_t *p;
	p = state.particleHistory + state.particleCount * state.frame;

	if (!p)
		return;
	if (!r)
		return;

    otDrawGravityLinesRecursive(p, r);

}

#endif

int otGetNCalcs() {
	return ncalcs;
}

int otGetSCalcs() {
	return scalcs;
}

int otGetPCalcs() {
	return pcalcs;
}
