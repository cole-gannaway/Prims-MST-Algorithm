/*
 *  Written by Cole Gannaway
 *  A generic AVL tree class. When constructed takes function pointers
 *  to display, compare, and free the generic value stored in the tree.
 *  
 *  This class acts as a wrapper on the BST tree class.
 */


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bst.h"
#include "avl.h"

//Constructor
struct avl{
    void (*display)(void *,FILE *);
    int (*compare)(void *,void *);
    void (*free)(void *);
    BST * binaryTree;
    int size;
};

//AVLVALUE holds the generic value and is stored in the AVL tree
typedef struct avlvalue{
    void * value;
    int count;
    int balance;
    void (*display)(void *,FILE *);
    int (*compare)(void *,void *);
    void (*free)(void *);
    int leftHeight;
    int rightHeight;
    int height;
}AVLVALUE;

/////////////  AVLVALUE Functions //////////////////////

static AVLVALUE *newAVLVALUE(AVL * t, void * v){
    AVLVALUE * createdAVLVALUE = malloc(sizeof(AVLVALUE));
    createdAVLVALUE->value = v;
    createdAVLVALUE->count = 1;
    createdAVLVALUE->balance = 0;
    createdAVLVALUE->display = t->display;
    createdAVLVALUE->compare = t->compare;
    createdAVLVALUE->free = t->free;
    createdAVLVALUE->leftHeight = 0;
    createdAVLVALUE->rightHeight = 0;
    createdAVLVALUE->height = 1;
    return createdAVLVALUE;
}
static int getAVLVALUEcount(AVLVALUE * aVal){
    assert(aVal != 0);
    return aVal->count;
}
static void setAVLVALUEcount(AVLVALUE * aVal,int x){
    assert(aVal != 0);
    aVal->count = x;
}
static int getAVLVALUEheight(AVLVALUE * aVal){
    if (aVal == 0) return 0;
    return aVal->height;
}
static void *getAVLVALUEval(AVLVALUE * aVal){
    if (aVal == 0) return 0;
    return aVal->value;
}
static int getAVLVALUEbalance(AVLVALUE * aVal){
    if (aVal == 0) return 0;
    return aVal->balance;
}
static void setAVLVALUEheight(AVLVALUE * aVal, int x){
    if (aVal == 0) return;
    aVal->height = x;
}
static int max (int x, int y){
    if (y > x) return y;
    return x;
}
///////////////////////////////////////////

static void setAVLbalance(BSTNODE * node){
    assert(node != 0);
    AVLVALUE * aVal = getBSTNODEvalue(node);
    BSTNODE * leftChild = getBSTNODEleft(node);
    BSTNODE * rightChild = getBSTNODEright(node);
    AVLVALUE * lcVal = 0;
    AVLVALUE * rcVal = 0;
    if (leftChild) lcVal = getBSTNODEvalue(leftChild);
    if (rightChild) rcVal = getBSTNODEvalue(rightChild);
    int leftHeight = getAVLVALUEheight(lcVal);
    int rightHeight = getAVLVALUEheight(rcVal);
    aVal->balance = (leftHeight - rightHeight);
    aVal->height = (max(leftHeight,rightHeight) + 1);
}
/* Functions to be passed in */
void swapperAVLVALUE(BSTNODE *a,BSTNODE *b)
{
    AVLVALUE *ta = getBSTNODEvalue(a);
    AVLVALUE *tb = getBSTNODEvalue(b);
    
    // swap the values stored in the AVL value objects
    void *vtemp = ta->value;
    ta->value = tb->value;
    tb->value = vtemp;
    
    // swap the counts stored in the AVL value objects
    int ctemp = ta->count;
    ta->count = tb->count;
    tb->count = ctemp;
    
    // note: AVL heights and balance factors are NOT swapped
}
void displayAVLVALUE(void * value,FILE * fp){
    AVLVALUE * aVal = (AVLVALUE *)value;
    aVal->display(getAVLVALUEval(aVal),fp);
    int freq = aVal->count;
    if (freq > 1) fprintf(fp, "[%d]",freq);
    if (aVal->balance > 0) fprintf(fp,"+");
    else if (aVal->balance < 0) fprintf(fp,"-");
}// display
int comparatorAVLVALUE(void * valueA,void * valueB){
    AVLVALUE * aValA = (AVLVALUE *)valueA;
    AVLVALUE * aValB = (AVLVALUE *)valueB;
    int num = aValA->compare(getAVLVALUEval(aValA),getAVLVALUEval(aValB));
    return num;
}// compare
void freeAVLVALUE (void * value){
    AVLVALUE * aVal = (AVLVALUE *)value;
    void * generic = getAVLVALUEval(aVal);
    if (generic) aVal->free(generic);
    free((AVL *)aVal);
}//free

/*AVL tree Functions*/
extern AVL *newAVL(void (*d)(void *,FILE *), int (*c)(void *,void *), void (*f)(void *)){
    AVL * createdAVL = malloc(sizeof(AVL));
    createdAVL->binaryTree = newBST(displayAVLVALUE,comparatorAVLVALUE,swapperAVLVALUE,freeAVLVALUE);
    createdAVL->display = d;
    createdAVL->compare = c;
    createdAVL->free = f;
    createdAVL->size = 0;
    return createdAVL;
}

///////////////// Private AVL tree Functions ////////////////
static int getAVLsize (AVL * aTree){
    if (aTree == 0)
        return 0;
    return (aTree->size);
}
static void setAVLsize (AVL * aTree, int x){
    aTree->size = x;
}
static BSTNODE *findNODE(AVL * aTree,void * value){
    AVLVALUE * aVal = newAVLVALUE(aTree,value);
    BSTNODE * found = findBST(aTree->binaryTree,aVal);
    free((AVLVALUE *)aVal);
    return found;
}
static BSTNODE *favorite(BSTNODE * p){
    assert(p != 0);
    AVLVALUE * aVal = getBSTNODEvalue(p);
    if (getAVLVALUEbalance(aVal) == (-1) ){
        return getBSTNODEright(p);
    }
    else if (getAVLVALUEbalance(aVal) == 1){
        return getBSTNODEleft(p);
    }
    return 0;
}
static int linear(BSTNODE * y,BSTNODE * x,BSTNODE * p){
    return ((getBSTNODEleft(x) == y && getBSTNODEleft(p) == x)
            || (getBSTNODEright(x) == y && getBSTNODEright(p) == x));
}
// child is left child
static void RightRotate(BSTNODE * p, BSTNODE * c){
    BSTNODE * gp = getBSTNODEparent(p);
    setBSTNODEparent(c,gp);
    BSTNODE * rightChild = getBSTNODEright(c);
    if (rightChild != 0) setBSTNODEparent(rightChild,p);
    setBSTNODEleft(p,rightChild);
    setBSTNODEparent(p,c);
    setBSTNODEright(c,p);
}
// child is right child
static void LeftRotate(BSTNODE * p, BSTNODE * c){
    BSTNODE * gp = getBSTNODEparent(p);
    setBSTNODEparent(c,gp);
    BSTNODE * leftChild = getBSTNODEleft(c);
    if (leftChild != 0) setBSTNODEparent(leftChild,p);
    setBSTNODEright(p,leftChild);
    setBSTNODEparent(p,c);
    setBSTNODEleft(c,p);
}
static void rotate(BSTNODE * child, BSTNODE * parent, BST * t){
    assert(parent != 0);
    assert(child != 0);
    // if parent was the original root
    if (getBSTroot(t) == parent){
        setBSTroot(t,child);
    }
    // the parent had a grandparent
    else{
        BSTNODE * gp = getBSTNODEparent(parent);
        if (getBSTNODEleft(gp) == parent){
            setBSTNODEleft(gp,child);
        }
        else{
            setBSTNODEright(gp,child);
        }
        setBSTNODEparent(child,gp);
    }
    if (getBSTNODEleft(parent) == child){
        RightRotate(parent,child);
    }
    else{
        LeftRotate(parent,child);
    }
}
static void insertionFixup(BSTNODE * x, BST * t){
    assert(x != 0);
    BSTNODE * p = 0;
    while (1){
        // if it is the root
        if (getBSTroot(t) == x){
            return;
        }
        p = getBSTNODEparent(x);
        // parents favors the sibling
        if ( favorite(p) != 0 && (x != favorite(p)) ){
            setAVLbalance(p);
            return;
        }
        // parent is balanced
        if (getAVLVALUEbalance(getBSTNODEvalue(p)) == 0){
            setAVLbalance(p);
            x = p;
            // continue looping
        }
        else{
            BSTNODE * y = favorite(x);
            // non linear
            if ((y != 0) && (!linear(y,x,p))){
                rotate(y,x,t);
                rotate(y,p,t);
                setAVLbalance(x);
                setAVLbalance(p);
                setAVLbalance(y);
            }
            //linear
            else{
                rotate(x,p,t);
                setAVLbalance(p);
                setAVLbalance(x);
            }
            return;
        }
    }
}

extern void insertAVL(AVL * aTree,void * value){
    BSTNODE * found = findNODE(aTree,value);
    // it is a duplicate
    if (found != 0){
        AVLVALUE * aVal = (AVLVALUE *)getBSTNODEvalue(found);
        setAVLVALUEcount(aVal,getAVLVALUEcount(aVal)+1);
        setAVLsize(aTree, getAVLsize(aTree) + 1);
        return;
    }
    // adding a new value
    AVLVALUE * createdaVal = newAVLVALUE(aTree,value);
    BSTNODE * inserted = insertBST(aTree->binaryTree,(void *)createdaVal);
    assert(inserted != 0);
    if ( inserted == getBSTroot(aTree->binaryTree)){
        setBSTNODEparent(inserted,0);
    }
    insertionFixup(inserted, aTree->binaryTree);
    setAVLsize(aTree, getAVLsize(aTree) + 1);
    return;
}

extern int findAVLcount(AVL * aTree,void * value){
    BSTNODE * found = findNODE(aTree,value);
    if (found == 0) return 0;
    AVLVALUE * foundVal = getBSTNODEvalue(found);
    return getAVLVALUEcount(foundVal);
}
extern void *findAVL(AVL * aTree,void * value){
    BSTNODE * found = findNODE(aTree,value);
    if (found == 0) return 0;
    AVLVALUE * foundVal = getBSTNODEvalue(found);
    return getAVLVALUEval(foundVal);
}
static BSTNODE *getSibling (BSTNODE * node){
    BSTNODE * p = getBSTNODEparent(node);
    if (p == 0) return 0;
    if (getBSTNODEleft(p) == node){
        return getBSTNODEright(p);
    }
    else{
        return getBSTNODEleft(p);
    }
}
static void deleteFixup(BSTNODE * x,BST * t){
    assert(x != 0);
    BSTNODE * p = 0;
    // set the height of x to zero since it will be deleted
    AVLVALUE * aVal = getBSTNODEvalue(x);
    setAVLVALUEheight(aVal,0);
    while (1){
        if (getBSTroot(t) == x){
            return;
        }
        p = getBSTNODEparent(x);
        if ( favorite(p) != 0 && (x == favorite(p)) ){
            setAVLbalance(p);
            x = p;
            // continue looping
        }
        else if (favorite(p) == 0){         //case 2
            setAVLbalance(p);
            return;
        }
        else{
            //p = parent of x
            BSTNODE * z = getSibling(x);
            BSTNODE * y = favorite(z);
            if ((y != 0) && (!linear(y,z,p))) { // case 3
                rotate(y,z,t);
                rotate(y,p,t);
                setAVLbalance(z);
                setAVLbalance(p);
                setAVLbalance(y);
                x = y;
                //continue looping
            }
            else{
                rotate(z,p,t); //case 4
                setAVLbalance(p);
                setAVLbalance(z);
                if (y == 0){
                    return;
                }
                x = z;
                //continue looping
            }
        }
    }
}

//Returns the generic value stored in the AVLVALUE
extern void *deleteAVL(AVL * aTree,void * value){
    BSTNODE * found = findNODE(aTree,value);
    if (found == 0) return 0;
    AVLVALUE * foundVal = getBSTNODEvalue(found);
    if (getAVLVALUEcount(foundVal) > 1){
        setAVLVALUEcount(foundVal,getAVLVALUEcount(foundVal)-1);
        setAVLsize(aTree,getAVLsize(aTree) - 1);
        return 0;
    }
    BSTNODE * leaf = swapToLeafBST(aTree->binaryTree,found);
    deleteFixup(leaf,aTree->binaryTree);
    AVLVALUE * aVal = getBSTNODEvalue(leaf);
    void * returnedVal = getAVLVALUEval(aVal);
    pruneLeafBST(aTree->binaryTree,leaf);
    free((AVLVALUE *)aVal);
    freeBSTNODE(leaf,0);
    setBSTsize(aTree->binaryTree,sizeBST(aTree->binaryTree) - 1);
    setAVLsize(aTree,getAVLsize(aTree) - 1);
    return returnedVal;
}
extern int sizeAVL(AVL * aTree){
    return (sizeBST(aTree->binaryTree));
}
extern int duplicatesAVL(AVL * aTree){
    int count = (getAVLsize(aTree)) - sizeBST(aTree->binaryTree);
    return count;
}
extern void statisticsAVL(AVL * aTree,FILE * fp){
    fprintf(fp,"Duplicates: %d\n", duplicatesAVL(aTree));
    statisticsBST(aTree->binaryTree,fp);
}
extern void displayAVL(AVL * aTree,FILE * fp){
    if (getAVLsize(aTree) == 0){
        fprintf(fp,"EMPTY\n");
    }
    displayBSTdecorated(aTree->binaryTree,fp);
}
extern void displayAVLdebug(AVL * aTree,FILE * fp){
    displayBST(aTree->binaryTree,fp);
}
extern void freeAVL(AVL * aTree){
    freeBST(aTree->binaryTree);
    free((AVL *) aTree);
}
