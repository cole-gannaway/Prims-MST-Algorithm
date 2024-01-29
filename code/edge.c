/*
 * EDGE class - written by John C. Lusth
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "edge.h"

#define DEBUG 0

#if DEBUG
#define dprintf printf
#else
#define dprintf(...)
#endif

struct edge
    {
    int v1;
    int v2;
    int weight;
    };

/*** constructors/desctructors ***/

EDGE
*newEDGE(int v1,int v2,int w)
    {
    EDGE *e = malloc(sizeof(EDGE));
    e->v1 = v1;
    e->v2 = v2;
    e->weight = w;
    return e;
    }


void
freeEDGE(void *e)
    {
    free((EDGE *) e);
    }

/*** accessors *******************/

int getEDGEv1(EDGE *e) { return e->v1; }
int getEDGEv2(EDGE *e) { return e->v2; }
int getEDGEweight(EDGE *e) { return e->weight; }

/*** mutators ********************/

int
setEDGEv1(EDGE *e,int v)
    {
    int temp = e->v1;
    e->v1 = v;
    return temp;
    }

int
setEDGEv2(EDGE *e,int v)
    {
    int temp = e->v2;
    e->v2 = v;
    return temp;
    }

int
setEDGEweight(EDGE *e,int w)
    {
    int temp = e->weight;
    e->weight = w;
    return temp;
    }

/*** informational ***************/

int
compareEDGE(void *a,void *b)
    {
    EDGE *e = a;
    EDGE *f = b;

    dprintf("comparing edge (%d,%d,%d) with (%d,%d,%d)\n",
        e->v1,e->v2,e->weight,f->v1,f->v2,f->weight);

    int r1 = e->v1 - f->v1;

    if (r1 == 0)
        return e->v2 - f->v2;
    else
        return r1;
    }

/*** visualizers *****************/

void
displayEDGE(void *a,FILE *fp)
    {
    EDGE *e = a;
    fprintf(fp,"(%d,%d,%d)",e->v1,e->v2,e->weight);
    }
