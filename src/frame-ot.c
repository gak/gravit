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


void otBranchNode(node_t *n);

static node_t *r = NULL;

#ifdef _OPENMP
static int master_thread_id = 0;
#endif

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

    VectorNew(vec);

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

    VectorNew(cm);

    c = otGetParticleInBox(min, max, &p, &mass, (float*)&cm);

    if (c == 0)
        return;

    view.recordNodes++;
    n->b[br] = malloc(sizeof(node_t));
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
    distance2(b->max, b->min, b->length2);

    b->mass = mass;

    if (c == 1) {

        b->p = p;
        VectorCopy(p->pos, b->cm);
        view.recordParticlesDone++;


    } else {

        otBranchNode(b);

    }

}

void otBranchNode(node_t *n) {

    VectorNew(min);
    VectorNew(max);

#ifdef _OPENMP
    if(omp_get_thread_num() == master_thread_id) {doVideoUpdate2();}

    if (!(state.mode & SM_RECORD)) return;
#else
    doVideoUpdate();
#endif

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

// copy of otBranchNode(). spawns 8 threads, one for each sub-tree
void otBranchNode_top(node_t *n) {

    VectorNew(min[8]);
    VectorNew(max[8]);
    int i;

    doVideoUpdate();

    // b[0]: top left front
    min[0][0] = n->min[0];
    max[0][0] = n->c[0];
    min[0][1] = n->min[1];
    max[0][1] = n->c[1];
    min[0][2] = n->min[2];
    max[0][2] = n->c[2];

    // b[1]: top right front
    VectorCopy(min[0], min[1]);
    VectorCopy(max[0], max[1]);
    min[1][0] = n->c[0];
    max[1][0] = n->max[0];

    // b[2]: bottom right front
    VectorCopy(min[1], min[2]);
    VectorCopy(max[1], max[2]);
    min[2][1] = n->c[1];
    max[2][1] = n->max[1];

    // b[3]: bottom left front
    VectorCopy(min[2], min[3]);
    VectorCopy(max[2], max[3]);
    min[3][0] = n->min[0];
    max[3][0] = n->c[0];

    // b[4]: top left back
    VectorCopy(min[3], min[4]);
    VectorCopy(max[3], max[4]);
    min[4][1] = n->min[1];
    max[4][1] = n->c[1];
    min[4][2] = n->c[2];
    max[4][2] = n->max[2];

    // b[5]: top right back
    VectorCopy(min[4], min[5]);
    VectorCopy(max[4], max[5]);
    min[5][0] = n->c[0];
    max[5][0] = n->max[0];

    // b[6]: bottom right back
    VectorCopy(min[5], min[6]);
    VectorCopy(max[5], max[6]);
    min[6][1] = n->c[1];
    max[6][1] = n->max[1];

    // b[7]: bottom left back
    VectorCopy(min[6], min[7]);
    VectorCopy(max[6], max[7]);
    min[7][0] = n->min[0];
    max[7][0] = n->c[0];


#ifdef _OPENMP
    master_thread_id = omp_get_thread_num();
    #pragma omp parallel for schedule(dynamic, 1)
#endif
    for (i=0; i<8; i++)
      otBranchNodeCorner(n, i, (float*)&min[i], (float*)&max[i]);

}

void otMakeTree() {

    particle_t *p;
    node_t *n;

    // make root node
    r = malloc(sizeof(node_t));
    memset(r, 0, sizeof(node_t));
    view.recordNodes = 1;

    n = r;

    otGetBoundingBox((float*)&n->min, (float*)&n->max);
    otGetParticleInBox((float*)&n->min, (float*)&n->max, &p, &n->mass, (float*)&n->cm);

    // Gets Center of min/max
    VectorSub(n->max, n->min, n->c);
    VectorDivide(n->c, 2, n->c);
    VectorAdd(n->c, n->min, n->c);

    // get length
    distance2(n->max, n->min, n->length2);

    otBranchNode_top(n);

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

    free(n);
    view.recordNodes--;

}

void otFreeTree() {

    view.recordStatus = 2;
    otFreeTreeRecursive(r);
    r = NULL;
    view.recordStatus = 0;

}

#ifndef NO_GUI

void otDrawTreeRecursive(node_t *n) {

    int i;

    if (!n)
        return;

    switch (view.drawTree) {

        // line mode
    case 1:
    default:
        if (n->p)
            glColor4f(0, 0, 1.0f, 1);
        else
            glColor4f(0, 0, 0.25f, 1);
        break;

        // fill mode
    case 2:
        if (n->p)
            glColor4f(0, 0, 1.0f, 0.1f);
        else
            glColor4f(0, 0, 1.0f, 0.05f);
        break;


    }

    glPushMatrix();
    glTranslatef(n->c[0], n->c[1], n->c[2]);
    glScalef(n->max[0] - n->min[0], n->max[1] - n->min[1], n->max[2] - n->min[2]);
    glScalef(0.5f, 0.5f, 0.5f);
    drawCube();
    glPopMatrix();

    for (i = 0; i < 8; i++)
        if (n->b[i])
            otDrawTreeRecursive((node_t *)n->b[i]);

}

void otDrawTree() {

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    switch (view.drawTree) {

        // line mode
    case 1:
    default:
        glLineWidth(1.0f);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;

        // fill mode
    case 2:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;

    }

    otDrawTreeRecursive(r);	// doit

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	// cleanup

}

#endif

void otComputeParticleToTreeRecursive(pttr_t *info) {

    int i;
    node_t *b;
    particle_t *p2;

    float d;
    float poo2;

    if (info->n->p == info->p)
        return;

    if (info->n->p) {

        p2 = info->n->p;

        distance2(p2->pos, info->p->pos, d);

// 		frDoGravity(p,n,d); was

        { // now
            VectorNew(dv);
            float force;
            dv[0] = info->n->cm[0] - info->p->pos[0];
            dv[1] = info->n->cm[1] - info->p->pos[1];
            dv[2] = info->n->cm[2] - info->p->pos[2];

            if (d) {
                force = state.g * info->pd->mass * info->n->mass / d;
                info->pd->accel[0] += dv[0] * force;
                info->pd->accel[1] += dv[1] * force;
                info->pd->accel[2] += dv[2] * force;
            }
        }

    } else {

        for (i = 0; i < 8; i++) {

            if (!info->n->b[i])
                continue;

            b = (node_t *)info->n->b[i];

            distance2(b->cm, info->p->pos, d);

            if (!d)
                continue;

	    poo2 = b->length2 / d ;

	    if ( poo2 > 0.25f ) {
                pttr_t info2;
                info2.n = b;
                info2.p = info->p;
                info2.pd = info->pd;
                otComputeParticleToTreeRecursive(&info2);

            } else {

                // frDoGravity(p,b,d);

                { // now
                    VectorNew(dv);
                    float force;
                    dv[0] = b->cm[0] - info->p->pos[0];
                    dv[1] = b->cm[1] - info->p->pos[1];
                    dv[2] = b->cm[2] - info->p->pos[2];

                    if (d) {
                        force = state.g * info->pd->mass * b->mass / d;
                        info->pd->accel[0] += dv[0] * force;
                        info->pd->accel[1] += dv[1] * force;
                        info->pd->accel[2] += dv[2] * force;
                    }
                }

            }

        }

    }

}

void processFrameOT(int start, int amount) {

    int i;

    view.recordStatus = 1;
    view.recordParticlesDone = 0;

    otMakeTree();

    view.recordStatus = 2;
    view.recordParticlesDone = 0;
    doVideoUpdate();

#ifdef _OPENMP
    master_thread_id = omp_get_thread_num();
#endif

    #pragma omp parallel for schedule(dynamic, 256)
    for (i = start; i < amount; i++) {
        particle_t *p;
        particleDetail_t *pd;
        pttr_t info;

        view.recordParticlesDone = i;

        p = state.particleHistory + state.particleCount * state.frame + i;
        pd = state.particleDetail + i;

        if (!p)
            continue;

        info.p = p;
        info.n = r;
        info.pd = pd;

#ifndef _OPENMP
        doVideoUpdate();
#else
	// only main thread may do this
        //#pragma omp single nowait
	if(omp_get_thread_num() == master_thread_id) {doVideoUpdate2();}
#endif

	if (state.mode & SM_RECORD)
	     otComputeParticleToTreeRecursive(&info);

    }

    view.recordStatus = 0;

}

void otDrawField() {

    VectorNew(pos);
    VectorNew(force);

    VectorZero(pos);
    VectorZero(force);

    if (!r)
        return;

    otDrawFieldRecursive(pos, r, force);

    glColor3f(1,1,1);
    glBegin(GL_LINES);
    VectorMultiply(force, 1000, force);
    glVertex3fv(pos);
    glVertex3fv(force);

    glEnd();


}

void otDrawFieldRecursive(float *pos, node_t *node, float *force) {

    int i;
    node_t *b;

    float d;
    float poo2;

    float f;

    VectorNew(newForce);
    VectorZero(newForce);

    for (i = 0; i < 8; i++) {

        if (!node->b[i])
            continue;

        b = (node_t *)node->b[i];

        distance2(b->cm, pos, d);

        if (!d)
            continue;

        poo2 = b->length2 / d;

        if ( poo2 > 0.25f ) {

            otDrawFieldRecursive(pos, b, newForce);

            VectorAdd(force, newForce, force);

        } else {

            {
                VectorNew(dv);
                dv[0] = b->cm[0] - pos[0];
                dv[1] = b->cm[1] - pos[1];
                dv[2] = b->cm[2] - pos[2];

                if (d) {
                    f = state.g * b->mass / d;
                    force[0] += dv[0] * f;
                    force[1] += dv[1] * f;
                    force[2] += dv[2] * f;
                }
            }

        }

    }

}

