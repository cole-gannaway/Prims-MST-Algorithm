/*
 *  Written by Cole Gannaway
 *  A generic BINOMIAL heap class. When constructed takes function pointers
 *  to display, compare, update, and free the generic value stored in the tree.
 *
 *  Update function would be passed in for graph algorithims, otherwise use NULL.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "binomial.h"
#include "dll.h"
#include "queue.h"

typedef struct binomialnode{
    void * data;
    int degree;
    DLL * children; //DLL that holds the pointers to the child nodes
    struct binomialnode * parent;
    void * owner; // holds a pointer to its DLL node, useful for removing
    void (*display)(void *,FILE *);
    void (*free)(void *);
}BinomialNODE;

struct binomial{
    void (*display)(void *,FILE *);
    int (*compare)(void *,void *);
    void (*update)(void *,void *);
    void (*free)(void *);
    BinomialNODE * extreme;
    DLL * rootList;
    int size;
};

////////////ARRAY used only for consilidation purposes ////////////////

typedef struct array{
    void ** array;
    int size;
    void (*display)(void *,FILE *);
    void (*free)(void *);
    
}ARRAY;
static ARRAY *newARRAY(void (*d)(void *,FILE *),
                       void (*f)(void *),int x){
    ARRAY * created = malloc(sizeof(ARRAY));
    created->array = malloc(sizeof(void*) * x);
    for (int i = 0; i < x; i++){
        created->array[i] = 0;
    }
    created->size = x;
    created->display = d;
    created->free = f;
    return created;
}
static int getSize(ARRAY * a){
    assert(a != 0);
    return a->size;
}
static int validIndex(ARRAY * a, int i){
    assert(a != 0);
    if (i >= 0 && i < getSize(a)){
        return 1;
    }
    return 0;
}
static void setARRAY(ARRAY *a,int index, void *value){
    assert(validIndex(a,index) != 0);
    a->array[index] = value;
}
static void *getARRAY(ARRAY *a,int index){
    assert(validIndex(a,index) != 0);
    return a->array[index];
}
static int isNull (ARRAY * a, int index){
    if (getARRAY(a,index) == 0) return 1;
    return 0;
}
static void freeARRAY(ARRAY *a){
    int s = getSize(a);
    for (int i = 0; i < s; i++){
        if (a->free != 0 && a->array[i] != 0){
            a->free(a->array[i]);
        }
    }
    free(a->array);
    free((ARRAY *) a);
}
///////////////// END of ARRAY Functions ////////////////



/// FUNCITONS TO BE PASSED IN///

void swapperBinomialNODE(void *a,void *b){
    BinomialNODE * nodeA = (BinomialNODE *)a;
    BinomialNODE * nodeB = (BinomialNODE *)b;
    void * temp = nodeA->data;
    nodeA->data = nodeB->data;
    nodeB->data = temp;
}//swapper

void displayBINOMIALnode(void * value,FILE * fp){
    BinomialNODE * x = (BinomialNODE *)value;
    void * generic = x->data;
    x->display(generic,fp);
}//display
void displayBINOMIALnodeDEBUG(void * value,FILE * fp){
    BinomialNODE * x = (BinomialNODE *)value;
    void * generic = x->data;
    x->display(generic,fp);
    fprintf(fp,"[");
    fprintf(fp,"%d",x->degree);
    fprintf(fp,"]");
    generic = x->parent->data;
    fprintf(fp,"\tparent's val is<");
    x->display(generic,fp);
    fprintf(fp,"[");
    fprintf(fp,"%d",x->parent->degree);
    fprintf(fp,"]");
    fprintf(fp,">");
}//displayDebug
void freeBINOMIALnode (void * value){
    BinomialNODE * node = (BinomialNODE *)value;
    void * generic = node->data;
    if (generic != 0) node->free(generic);
    free((BinomialNODE *)node);
}//free

//Constructor
extern BINOMIAL *newBINOMIAL(void (*d)(void *,FILE *), int (*c)(void *,void *),void (*s)(void *,void *),void (*f)(void *)){
    BINOMIAL * createdBinomial = malloc(sizeof(BINOMIAL));
    createdBinomial->display = d;
    createdBinomial->compare = c;
    createdBinomial->free = f;
    createdBinomial->extreme = 0;
    createdBinomial->rootList = newDLL(displayBINOMIALnode,freeBINOMIALnode);
    createdBinomial->update = s;
    createdBinomial->size = 0;
    return createdBinomial;
}
/// Private BINOMIAL HEAP FUNCTIONS //////
static DLL *getBINOMIALrootList(BINOMIAL * b){
    assert(b != 0);
    return b->rootList;
}
static void setBINOMIALsize(BINOMIAL * b, int s){
    assert(b != 0);
    b->size = s;
}
static int getBINOMIALsize(BINOMIAL * b){
    assert(b != 0);
    return b->size;
}
static void setBINOMIALextreme(BINOMIAL * b,BinomialNODE * x){
    assert(b != 0);
    b->extreme = x;
}
static BinomialNODE *getBINOMIALextreme(BINOMIAL * b){
    assert(b != 0);
    return b->extreme;
}

//// Private BinomialNODE FUNCTIONS ////////
static void setBinomialNODEdata(BinomialNODE * n, void * value){
    assert(n != 0);
    n->data = value;
}
static void *getBinomialNODEdata(BinomialNODE * n){
    assert(n != 0);
    return n->data;
}
static void setBinomialNODEdegree(BinomialNODE * n, int d){
    assert(n != 0);
    n->degree = d;
}
static int getBinomialNODEdegree(BinomialNODE * n){
    assert(n != 0);
    return n->degree;
}
static void setBinomialNODEparent(BinomialNODE * n, BinomialNODE * p){
    assert(n != 0);
    n->parent = p;
}
static BinomialNODE *getBinomialNODEparent(BinomialNODE * n){
    assert(n != 0);
    return n->parent;
}
static DLL *getBinomialNODEchildren(BinomialNODE * n){
    assert(n != 0);
    return n->children;
}
static void setBinomialNODEowner(BinomialNODE * n,void * o){
    assert(n != 0);
    n->owner = o;
}
static DLL *getBinomialNODEowner(BinomialNODE * n){
    assert(n != 0);
    return n->owner;
}
static BinomialNODE *newBinomialNODE(BINOMIAL * b,void * value){
    BinomialNODE * createdBinomialNODE = malloc(sizeof(BinomialNODE));
    createdBinomialNODE->data = value;
    createdBinomialNODE->degree = 0;
    createdBinomialNODE->children = newDLL(displayBINOMIALnode,freeBINOMIALnode);
    createdBinomialNODE->parent = createdBinomialNODE;
    createdBinomialNODE->owner = 0;
    createdBinomialNODE->display = b->display;
    createdBinomialNODE->free = b->free;
    return createdBinomialNODE;
}


///// Private helper FUNCTIONS for insertBINOMIAL /////
static BinomialNODE *combine(BINOMIAL * b,BinomialNODE * x,BinomialNODE * y){
    //if x's value is more extreme than y's
    if (b->compare(getBinomialNODEdata(x),getBinomialNODEdata(y)) < 0){
        setBinomialNODEowner(y,insertDLL(x->children,sizeDLL(x->children),y));
        setBinomialNODEparent(y,x);
        setBinomialNODEdegree(x,getBinomialNODEdegree(x) + 1);
        return x;
    }
    else{
        setBinomialNODEowner(x,insertDLL(y->children,sizeDLL(y->children),x));
        setBinomialNODEparent(x,y);
        setBinomialNODEdegree(y,getBinomialNODEdegree(y) + 1);
        return y;
    }
}

static void updateConsolidationArray (BINOMIAL * b,ARRAY * D,BinomialNODE * spot){
    int degree = getBinomialNODEdegree(spot);
    while (!(isNull(D,degree))){
        spot = combine(b,spot,getARRAY(D,degree));
        setARRAY(D,degree,0);
        degree++;
    }
    setARRAY(D,degree,spot);
}
static void consolidate(BINOMIAL * b){
    int size = log(getBINOMIALsize(b))/log(2) + 1;
    ARRAY * D = newARRAY(displayBINOMIALnode,0,size);
    void * spot = 0;
    while (sizeDLL(b->rootList) > 0){
        firstDLL(b->rootList);
        spot = currentDLL(b->rootList);
        assert(getBinomialNODEowner(spot) != 0);
        removeDLLnode(b->rootList,getBinomialNODEowner(spot));
        updateConsolidationArray(b,D,spot);
    }
    //transfer the D array back to the heap, keeping track of the extreme value
    setBINOMIALextreme(b,0);
    BinomialNODE * ptr = 0;
    
    for (int i = 0; i < size; i++){
        if (!(isNull(D,i))){
            ptr = getARRAY(D,i);
            //insert D[i] into b's root list (via the linked-list's insert method)
            setBinomialNODEowner(ptr,insertDLL(b->rootList,sizeDLL(getBINOMIALrootList(b)),ptr));
            //update b's extreme pointer if it's null or b's comparator indicates that D[i] is more extreme
            if (getBINOMIALextreme(b) == 0) setBINOMIALextreme(b,ptr);
            else if (b->compare(getBinomialNODEdata(ptr),getBinomialNODEdata(getBINOMIALextreme(b))) < 0){
                setBINOMIALextreme(b,ptr);
            }
        }
    }
    freeARRAY(D);
}


///// PUBLIC FUNCTIONS ////
extern void *insertBINOMIAL(BINOMIAL *b,void *value){
    BinomialNODE * n = newBinomialNODE(b,value);
    setBinomialNODEowner(n,insertDLL(b->rootList,0,n));
    setBINOMIALsize(b,getBINOMIALsize(b) + 1);
    consolidate(b);
    return n;
}
extern int sizeBINOMIAL(BINOMIAL *b){
    if (b == 0) return 0;
    return b->size;
}
extern void unionBINOMIAL(BINOMIAL * b,BINOMIAL * donor){
    //b and donor are binomial heaps
    unionDLL(b->rootList,donor->rootList);
    setBINOMIALsize(b,getBINOMIALsize(b)+getBINOMIALsize(donor));
    removeDLLall(donor->rootList);
    setBINOMIALsize(donor,0);
    setBINOMIALextreme(b,0);
    consolidate(b);
}
extern void deleteBINOMIAL(BINOMIAL *b,void *node){
    decreaseKeyBINOMIAL(b,node,0);
    extractBINOMIAL(b);
}
// Private Helper Function //
static void * bubbleUp(BINOMIAL * b, BinomialNODE * n){
    BinomialNODE * p = getBinomialNODEparent(n);
    if (n == p) return n;
    assert(n != 0);
    if (b->compare(getBinomialNODEdata(n),getBinomialNODEdata(p)) >= 0) return n;
    if (b->update != 0){
        b->update(getBinomialNODEdata(n),p);
        b->update(getBinomialNODEdata(p),n);
    }
    swapperBinomialNODE(n,p);
    p = getBinomialNODEparent(n);
    return bubbleUp(b,p);
}

// Returns the Binomial node it now points to
extern void decreaseKeyBINOMIAL(BINOMIAL *b,void * node ,void * value){
    assert(node != 0);
    BinomialNODE * n = (BinomialNODE *)node;
    setBinomialNODEdata(n,value);
    BinomialNODE * resultingNode = bubbleUp(b,n);
    // update b's extreme value pointer if necessary)
    if (b->compare(getBinomialNODEdata(resultingNode),getBinomialNODEdata(getBINOMIALextreme(b))) < 0){
        setBINOMIALextreme(b,resultingNode);
    }
    return;
}
extern void *peekBINOMIAL(BINOMIAL *b){
    assert(b != 0);
    BinomialNODE * n = getBINOMIALextreme(b);
    return getBinomialNODEdata(n);
}
extern void *extractBINOMIAL(BINOMIAL *b){
    BinomialNODE * y = getBINOMIALextreme(b);
    removeDLLnode(getBINOMIALrootList(b),getBinomialNODEowner(y));
    // set each of the y's children's parent to point to themselves
    DLL * childrenDLL = getBinomialNODEchildren(y);
    firstDLL(childrenDLL);
    BinomialNODE * c = 0;
    int s = sizeDLL(childrenDLL);
    for (int a = 0; a < s; a++){
        c = currentDLL(childrenDLL);
        setBinomialNODEparent(c,c);
        nextDLL(childrenDLL);
    }
    unionDLL(childrenDLL,getBINOMIALrootList(b));
    unionDLL(getBINOMIALrootList(b),childrenDLL);
    consolidate(b);
    assert(y != 0);
    void * generic = getBinomialNODEdata(y);
    freeDLL(getBinomialNODEchildren(y));
    free((BinomialNODE*)y);
    setBINOMIALsize(b,getBINOMIALsize(b)-1);
    return generic;
}
extern void statisticsBINOMIAL(BINOMIAL *b,FILE *fp){
    fprintf(fp,"size: %d\n",getBINOMIALsize(b));
    fprintf(fp,"rootlist size: %d\n",(sizeDLL(getBINOMIALrootList(b))));
    fprintf(fp,"extreme: ");
    if (getBINOMIALextreme(b) != 0) displayBINOMIALnode(getBINOMIALextreme(b),fp);
    else fprintf(fp,"NULL");
}
extern void displayBINOMIAL(BINOMIAL *b,FILE *fp){
    if (getBINOMIALsize(b) == 0){
        fprintf(fp,"EMPTY\n");
        return;
    }
    fprintf(fp,"rootlist: ");
    DLL * list = getBINOMIALrootList(b);
    BinomialNODE * ptr = 0;
    int degree = 0;
    int count = 0;
    
    firstDLL(list);
    while (1){
        ptr = currentDLL(list);
        if (getBinomialNODEdegree(ptr) == degree){
            displayBINOMIALnode(ptr,fp);
            if (ptr == getBINOMIALextreme(b)){
                fprintf(fp,"*");
            }
            count++;
            if (count == sizeDLL(list)) break;
            nextDLL(list);
        }
        else{
            fprintf(fp,"NULL");
        }
        fprintf(fp," ");
        degree++;
    }
    fprintf(fp,"\n");
}
extern void displayBINOMIALdebug(BINOMIAL *b,FILE *fp){
    /*Base Case*/
    if (getBINOMIALsize(b) < 1) return;
    /*Create an empty QUEUE*/
    QUEUE * linkedList = newQUEUE(0,0);
    /*Enqueue the root and initialize the height*/
    enqueue(linkedList,getBINOMIALrootList(b));
    while (1){
        int nodeCountAtLevel = sizeQUEUE(linkedList);
        if (nodeCountAtLevel == 0) break;
        while (nodeCountAtLevel > 0){
            DLL * list = dequeue(linkedList);
            if (b->display){
                displayDLL(list,fp);
            }
            firstDLL(list);
            for (int a = 0; a < sizeDLL(list); a++){
                BinomialNODE * ptr = currentDLL(list);
                assert(ptr != 0);
                if (sizeDLL(getBinomialNODEchildren(ptr)) > 0){
                    enqueue(linkedList,getBinomialNODEchildren(ptr));
                }
                nextDLL(list);
            }
            nodeCountAtLevel--;
        }
        fprintf(fp,"\n");
    }
    freeQUEUE(linkedList);
    return;
}
extern void freeBINOMIAL(BINOMIAL *b){
    assert(b != 0);
    BinomialNODE * ptr = 0;
    QUEUE * linkedList = newQUEUE(0,0);
    DLL * level = 0;
    enqueue(linkedList,getBINOMIALrootList(b));
    while (sizeQUEUE(linkedList) != 0){
        level = dequeue(linkedList);
        if (sizeDLL(level) > 0){
            firstDLL(level);
            for (int a = 0; a < sizeDLL(level); a++){
                ptr = currentDLL(level);
                enqueue(linkedList,getBinomialNODEchildren(ptr));
                nextDLL(level);
            }
        }
        freeDLL(level);
    }
    freeQUEUE(linkedList);
    free((BINOMIAL *)b);
}
