/* EDGE public interface - written by John C. Lusth */

#ifndef __EDGE_INCLUDED__
#define __EDGE_INCLUDED__

#include <stdio.h>

typedef struct edge EDGE;

extern EDGE *newEDGE(int v1,int v2,int weight);
extern int getEDGEv1(EDGE *);
extern int getEDGEv2(EDGE *);
extern int getEDGEweight(EDGE *);
extern int setEDGEv1(EDGE *,int);
extern int setEDGEv2(EDGE *,int);
extern int setEDGEweight(EDGE *,int);
extern void displayEDGE(void *,FILE *);
extern int compareEDGE(void *,void *);
extern void freeEDGE(void *);
#endif
