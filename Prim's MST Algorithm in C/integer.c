/*
 * INTEGER class - written by John C. Lusth
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include "integer.h"

struct INTEGER
    {
    int value;
    };

INTEGER *
newINTEGER(int x)
    {
    INTEGER *p = malloc(sizeof(INTEGER));
    assert(p != 0);
    p->value = x;
    return p;
    }

int 
getINTEGER(INTEGER *v)
    {
    return v->value;
    }

int 
setINTEGER(INTEGER *v,int x)
    {
    int old = v->value;
    v->value = x;
    return old;
    }

void 
displayINTEGER(void *v,FILE *fp)
    {
    fprintf(fp,"%d",getINTEGER((INTEGER *) v));
    }

int 
compareINTEGER(void *v,void *w)
    {
    if (v == 0) return -1;
    if (w == 0) return 1;
    return getINTEGER(v) - getINTEGER(w);
    }

int
rcompareINTEGER(void *v,void *w)
{
    if (v == 0) return 1;
    if (w == 0) return -1;
    return getINTEGER(w) - getINTEGER(v);
}

void
freeINTEGER(void *v)
    {
    free((INTEGER *) v);
    }
