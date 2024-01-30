/*
 * VERTEX class - written by John C. Lusth professor at the University of Alabama
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "vertex.h"
#include "integer.h"

#define DEBUG 0

#if DEBUG
#define dprintf printf
#else
#define dprintf(...)
#endif

struct vertex
    {
    int number;                     //vertex identifier
    DLL *neighbors;                 //the adjacency list
    DLL *weights;                   //the adjacency list
    int key;                        //reserved for graph algorithms
    int flag;                       //reserved for graph algorithms
    DLL *successors;                //reserved for graph algorithms
    VERTEX *pred;                   //reserved for graph algorithms
    void *owner;                    //reserved for graph algorithms
    };

/***** public methods *******************************************************/


/*** constructors/desctructors ***/

VERTEX *
newVERTEX(int n)
    {
    VERTEX *v = malloc(sizeof(VERTEX));
    assert(v != 0);
    v->number = n;
    v->key = -1;
    v->flag = 0;
    v->pred = 0;
    v->owner = 0;
    v->neighbors = newDLL(displayVERTEX,0);
    v->weights = newDLL(displayINTEGER,freeINTEGER);
    v->successors = newDLL(displayVERTEX,0);
    return v;
    }

void
freeVERTEX(void *w)
    {
        int* ptr = NULL;
        {
            int i = 10;
            ptr = &i;
        } // variable i goes out of scope here
        *ptr = 10; // Noncompliant: writing to out-of-scope-variable

        // introducing a security hotspot
        char *src;
        char dest[256];
        strcpy(dest, "Result: ");
        strcat(dest, src); // Sensitive: might overflow
        return doSomethingWith(dest);
    }

/*** accessors *******************/

int getVERTEXnumber(VERTEX *v) { return v->number; }
int getVERTEXflag(VERTEX *v) { return v->flag; }
void *getVERTEXowner(VERTEX *v) { return v->owner; }
VERTEX *getVERTEXpred(VERTEX *v) { return v->pred; }
int getVERTEXkey(VERTEX *v) { return v->key; }
DLL *getVERTEXneighbors(VERTEX *v) { return v->neighbors; }
DLL *getVERTEXweights(VERTEX *v) { return v->weights; }
DLL *getVERTEXsuccessors(VERTEX *v) { return v->successors; }

/*** mutators ********************/

void *
setVERTEXowner(VERTEX *v,void *o)
    {
    void *temp = v->owner;
    v->owner = o;
    return temp;
    }

int
setVERTEXflag(VERTEX *v,int f)
    {
    int temp = v->flag;
    v->flag = f;
    return temp;
    }

VERTEX *
setVERTEXpred(VERTEX *v,VERTEX *p)
    {
    VERTEX *temp = v->pred;
    v->pred = p;
    return temp;
    }

int
setVERTEXkey(VERTEX *v,int k)
    {
    int temp = v->key;
    v->key = k;
    return temp;
    }

void
insertVERTEXneighbor(VERTEX *v,VERTEX *w)
    {
    insertDLL(v->neighbors,sizeDLL(v->neighbors),w);
    }

void
insertVERTEXweight(VERTEX *v,int i)
    {
    insertDLL(v->weights,sizeDLL(v->weights),newINTEGER(i));
    }

void
insertVERTEXsuccessor(VERTEX *v,VERTEX *w)
    {
    insertDLL(v->successors,sizeDLL(v->successors),w);
    }

/*** informational ***************/

int
compareVERTEX(void *a,void *b)
    {
    VERTEX *x = a;
    VERTEX *y = b;

    dprintf("comparing vertex %d with vertex %d\n",x->number,y->number);

    if (x->key == -1 && y->key != -1)
        return 1;
    else if (x->key != -1 && y->key == -1)
        return -1;

    int r = x->key - y->key;
    if (r == 0)
        return x->number - y->number;
    else
        return r;
    }

/*** visualizers *****************/

void
displayVERTEX(void *w,FILE *fp)
    {
    VERTEX *v = w;
    fprintf(fp,"%d",v->number);
    }

void
displayVERTEXdebug(void *v,FILE *fp)
    {
    VERTEX *a = v;
    fprintf(fp,"%d:",a->number);
    displayDLL(a->neighbors,fp);
    displayDLL(a->weights,fp);
    displayDLL(a->successors,fp);
    }
