#ifndef __BINOMIAL_INCLUDED__
#define __BINOMIAL_INCLUDED__

#include <stdio.h>

typedef struct binomial BINOMIAL;

extern BINOMIAL *newBINOMIAL(
        void (*display)(void *,FILE *),
        int (*compare)(void *,void *),
        void (*update)(void *,void *),
        void (*free)(void *));
void *insertBINOMIAL(BINOMIAL *b,void *value);
extern int sizeBINOMIAL(BINOMIAL *b);
extern void unionBINOMIAL(BINOMIAL *,BINOMIAL *);
extern void deleteBINOMIAL(BINOMIAL *b,void *node);
extern void decreaseKeyBINOMIAL(BINOMIAL *b,void *node,void *value);
extern void *peekBINOMIAL(BINOMIAL *b);
extern void *extractBINOMIAL(BINOMIAL *b);
extern void statisticsBINOMIAL(BINOMIAL *b,FILE *fp);
extern void displayBINOMIAL(BINOMIAL *b,FILE *fp);
extern void displayBINOMIALdebug(BINOMIAL *b,FILE *fp);
extern void freeBINOMIAL(BINOMIAL *b);

#endif
