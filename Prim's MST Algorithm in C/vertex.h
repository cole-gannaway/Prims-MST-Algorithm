/* VERTEX public interface - written by John C. Lusth */

#ifndef __VERTEX_INCLUDED__
#define __VERTEX_INCLUDED__

#include <stdio.h>
#include "dll.h"

typedef struct vertex VERTEX;

extern VERTEX *newVERTEX(int);
extern int getVERTEXnumber(VERTEX *);
extern void *getVERTEXowner(VERTEX *);
extern void *setVERTEXowner(VERTEX *,void *);
extern int getVERTEXflag(VERTEX *);
extern int setVERTEXflag(VERTEX *,int);
extern VERTEX *getVERTEXpred(VERTEX *);
extern VERTEX *setVERTEXpred(VERTEX *,VERTEX *);
extern int getVERTEXkey(VERTEX *);
extern int setVERTEXkey(VERTEX *,int);
extern void insertVERTEXneighbor(VERTEX *,VERTEX *);
extern void insertVERTEXweight(VERTEX *,int);
extern void insertVERTEXsuccessor(VERTEX *,VERTEX *);
extern DLL *getVERTEXneighbors(VERTEX *);
extern DLL *getVERTEXweights(VERTEX *);
extern DLL *getVERTEXsuccessors(VERTEX *);
extern void displayVERTEX(void *,FILE *fp);
extern void displayVERTEXdebug(void *,FILE *fp);
extern int compareVERTEX(void *,void *);
extern void freeVERTEX(void *);
#endif
