/*
 *  Written by Cole Gannaway
 *  A generic BST (Binary Search Tree) class. When constructed takes function pointers
 *  to display, compare, swap, and free the generic value stored in the tree.
 *
 *  Swap function would be passed in for graph algorithims, otherwise use NULL.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "bst.h"
#include "queue.h"

//// BSTNODE ////////////
struct bstnode{
    void * data;
    struct bstnode * right;
    struct bstnode * left;
    struct bstnode * parent;
};

extern BSTNODE *newBSTNODE(void *v){
    BSTNODE * created = malloc(sizeof(BSTNODE));
    created->data = v;
    created->right = 0;
    created->left = 0;
    created->parent = 0;
    return created;
}
extern void *getBSTNODEvalue(BSTNODE *n){
    if (n) return n->data;
    return 0;
}
extern void setBSTNODEvalue(BSTNODE *n,void *value){
    if (n) n->data = value;
}
extern BSTNODE *getBSTNODEleft(BSTNODE *n){
    if (n->left) return n->left;
    return NULL;
}
extern void setBSTNODEleft(BSTNODE *n,BSTNODE *replacement){
    if (n) n->left = replacement;
}
extern BSTNODE *getBSTNODEright(BSTNODE *n){
    if (n->right) return n->right;
    return NULL;
}
extern void setBSTNODEright(BSTNODE *n,BSTNODE *replacement){
    if (n) n->right = replacement;
}
extern BSTNODE *getBSTNODEparent(BSTNODE *n){
    if (n->parent) return n->parent;
    return NULL;
}
extern void setBSTNODEparent(BSTNODE * child,BSTNODE *adult){
    if (child) child->parent = adult;
}
extern void freeBSTNODE(BSTNODE *n,void (*freeFunct)(void *)){
    if (n == 0) return;
    if (freeFunct && n->data) {freeFunct(n->data);}
    free((BSTNODE *) n);
}
struct bst {
    BSTNODE * root;
    void (*display)(void *,FILE *);
    int (*comparator)(void *,void *);
    void (*swapper)(BSTNODE *,BSTNODE *);
    void (*free)(void *);
    int size;
};
///////////////// Private BST Functions ////////////////
static void swapFunct (BSTNODE * node, BSTNODE * next){
    void * temp = ((void*)getBSTNODEvalue(node));
    setBSTNODEvalue(node,getBSTNODEvalue(next));
    setBSTNODEvalue(next,temp);
}
extern BST *newBST( void (*d)(void *,FILE *), int (*c)(void *,void *),void (*s)(BSTNODE *,BSTNODE *),void (*f)(void *)){
    BST * created = malloc (sizeof(BST));
    created->root = 0;
    created->display = d;
    created->comparator = c;
    if (s) created->swapper = s;
    else{
        created->swapper = swapFunct;
    }
    created->free = f;
    created->size = 0;
    return created;
}
static int getBSTsize(BST * t){
    return t->size;
}
///////////////// Public BST Functions ////////////////
extern void setBSTsize(BST *t,int s){
    t->size = s;
}
extern BSTNODE *getBSTroot(BST *t){
    return t->root;
}
extern void setBSTroot(BST *t,BSTNODE *replacement){
    t->root = replacement;
    if (replacement != 0) replacement->parent = 0;
    return;
}
extern BSTNODE *insertBST(BST *t,void *value){
    BSTNODE * created = newBSTNODE(value);
    /*if the list is empty*/
    if (getBSTroot(t) == NULL){
        setBSTsize(t,getBSTsize(t)+1);
        t->root = created;
        return created;
    }
    BSTNODE * ptr = getBSTroot(t);
    BSTNODE * before = ptr;
    while (ptr) {
        before = ptr;
        /*if value is bigger than current ptr*/
        if ( (t->comparator(value, ptr->data)) > 0 ){
            ptr = ptr->right;
        }
        /*if value is samller than current ptr*/
        else{
            ptr = ptr->left;
        }
    }
    //ptr should now point to NULL
    /*if value is bigger than current ptr*/
    if ( (t->comparator(value, before->data)) > 0 ){
        before->right = created;
    }
    /*if value is smaller than current ptr*/
    else{
        before->left = created;
    }
    created->parent = before;
    t->size = (t->size) + 1;
    return created;
}
extern BSTNODE *findBST(BST *t,void *value){
    BSTNODE * ptr = t->root;
    while (ptr) {
        if ( (t->comparator(value, ptr->data) == 0) ){
            return ptr;
        }
        else{
            /*if value is bigger than current ptr*/
            if ( (t->comparator(value, ptr->data)) > 0 ){
                ptr = ptr->right;
            }
            /*if value is samller than current ptr*/
            else{
                ptr = ptr->left;
            }
        }
    }
    return NULL;
}
// Private Helper Function //
static int isLeaf(BSTNODE * node){
    if (node == 0) return 0;
    if (getBSTNODEleft(node) || getBSTNODEright(node)){
        return 0;
    }
    return 1;
}
extern BSTNODE * findSuccessor(BSTNODE * node){
    if (node == 0) return node;
    if (isLeaf(node)) return node;
    // find successor
    if (getBSTNODEright(node)){
        BSTNODE * successor = getBSTNODEright(node);
        while (successor){
            if (getBSTNODEleft(successor)){
                successor = getBSTNODEleft(successor);
            }
            else{return successor;}
        }
    }
    // no successor? find predecssor
    else if (getBSTNODEleft(node)){
        BSTNODE * predecessor = getBSTNODEleft(node);
        while (predecessor) {
            if(getBSTNODEright(predecessor)){
                predecessor = getBSTNODEright(predecessor);
            }
            else{return predecessor;}
        }
    }
    return 0;
}
extern BSTNODE *swapToLeafBST(BST *t,BSTNODE *node){
    if (node == 0 || t == 0) return NULL;
    if (isLeaf(node)) {return node;}
    BSTNODE * next = findSuccessor(node);
    // hoping this doesn't happen
    if (next == 0) {return 0;}
    t->swapper(node,next);
    // if there is a child on the leaf swap one more time
    if (isLeaf(next) == 0){
        next = swapToLeafBST(t,next);
    }
    return next;
}
extern void pruneLeafBST(BST *t,BSTNODE *leaf){
    if (leaf == 0) return;
    assert(isLeaf(leaf));
    BSTNODE * parent = getBSTNODEparent(leaf);
    // if we are pruning the root
    if (parent == 0){
        setBSTroot(t,0);
        return;
    }
    assert(sizeBST(t) > 1);
    // the leaf is a left child
    if ( getBSTNODEleft(parent) == leaf ){
        setBSTNODEleft(parent,0);
    }
    // the leaf is a right child
    else{
        assert(leaf->parent->right == leaf);
        setBSTNODEright(parent,0);
    }
}
extern BSTNODE *deleteBST(BST *t,void *value){
    BSTNODE * foundNode = findBST(t,value);
    assert(foundNode != 0);
    BSTNODE * leaf = swapToLeafBST(t,foundNode);
    pruneLeafBST(t,leaf);
    t->size = (t->size - 1);
    return leaf;
}
extern int sizeBST(BST *t){
    return t->size;
}
// Private Helper Functions //
static int max(int x, int y){
    if (x > y) return x;
    return y;
}
static int maxDepth(BSTNODE * node){
    // Corner case. Should never be hit unless the code is
    // called on node = NULL
    if (node == 0)
        return 0;
    
    // Base case : Leaf Node. This accounts for height = 1.
    if (isLeaf(node))
        return 1;
    
    return max(maxDepth(getBSTNODEleft(node)), maxDepth(getBSTNODEright(node))) + 1;
}

static int min(int x, int y){
    if (x < y) return x;
    return y;
}
static int minDepth(BSTNODE * node){
    // Corner case. Should never be hit unless the code is
    // called on node = NULL
    if (node == 0){
        return 0;
    }
    // Base case : Leaf Node. This accounts for height = 1.
    if (isLeaf(node))
        return 1;

    return min(minDepth(getBSTNODEleft(node)), minDepth(getBSTNODEright(node))) + 1;
}
extern void statisticsBST(BST *t,FILE *fp){
    fprintf(fp,"Nodes: %d\n", t->size);
    fprintf(fp,"Minimum depth: %d\n", minDepth(getBSTroot(t)) - 1);
    fprintf(fp,"Maximum depth: %d\n", maxDepth(getBSTroot(t)) - 1);

}
// Private Helper Function //
static void preoder(BST * t, BSTNODE * ptr, FILE * fp){
    if (ptr == NULL) return;
    fprintf(fp,"[");
    if (t->display) t->display(ptr->data,fp);
    if (ptr->left) {
        fprintf(fp, " ");
        preoder(t,ptr->left,fp);
    }
    if (ptr->right) {
        fprintf(fp, " ");
        preoder(t,ptr->right,fp);
    }
    fprintf(fp,"]");
}
extern void displayBST(BST *t,FILE *fp){
    BSTNODE * ptr = t->root;
    if (ptr == 0) fprintf(fp,"[]");
    preoder(t,ptr,fp);
    return;
}
extern void displayBSTdebug(BST *t,FILE *fp){
    /*Base Case*/
    if (getBSTsize(t) < 1) return;
    /*Create an empty QUEUE*/
    QUEUE * ptrList = newQUEUE(t->display,t->free);
    /*Enqueue the root and initialize the height*/
    enqueue(ptrList,getBSTroot(t));
    while (1){
        int nodeCountAtLevel = sizeQUEUE(ptrList);
        if (nodeCountAtLevel == 0) break;
        while (nodeCountAtLevel > 0){
            BSTNODE * node = dequeue(ptrList);
            if (t->display){
                t->display(getBSTNODEvalue(node),fp);
                if (nodeCountAtLevel > 1) fprintf(fp," ");
            }
            if (getBSTNODEleft(node)) enqueue(ptrList,getBSTNODEleft(node));
            if (getBSTNODEright(node)) enqueue(ptrList,getBSTNODEright(node));
            nodeCountAtLevel--;
        }
        fprintf(fp,"\n");
    }
    freeQUEUE(ptrList);
    return;
}
// Private Helper Function used for AVL tree //
static void decorated(BST * t, BSTNODE * node, FILE * fp){
    if (isLeaf(node)) fprintf(fp, "=");
    t->display(getBSTNODEvalue(node),fp);
    fprintf(fp,"(");
    BSTNODE * parent = getBSTNODEparent(node);
    if (parent) t->display(getBSTNODEvalue(parent),fp);
    else{ t->display(getBSTNODEvalue(node),fp);}
    fprintf(fp,")");
    if (getBSTroot(t) == node){
        fprintf(fp,"X");
    }
    else if (getBSTNODEright(parent) == node){
        fprintf(fp,"R");
    }
    else if (getBSTNODEleft(parent) == node){
        fprintf(fp,"L");
    }
}
extern void displayBSTdecorated(BST *t,FILE *fp){
    /*Base Case*/
    if (getBSTsize(t) < 1) return;
    /*Create an empty QUEUE*/
    QUEUE * ptrList = newQUEUE(t->display,t->free);
    /*Enqueue the root and initialize the height*/
    enqueue(ptrList,getBSTroot(t));
    int levelNum = 0;
    while (1){
        int nodeCountAtLevel = sizeQUEUE(ptrList);
        if (nodeCountAtLevel == 0) break;
        fprintf(fp,"%d: ", levelNum);
        while (nodeCountAtLevel > 0){
            BSTNODE * node = dequeue(ptrList);
            if (t->display){
                decorated(t, node, fp);
                if (nodeCountAtLevel > 1) fprintf(fp," ");
            }
            if (getBSTNODEleft(node)) enqueue(ptrList,getBSTNODEleft(node));
            if (getBSTNODEright(node)) enqueue(ptrList,getBSTNODEright(node));
            nodeCountAtLevel--;
        }
        fprintf(fp,"\n");
        levelNum++;
    }
    freeQUEUE(ptrList);
    return;
}
static void recurssiveFree(BST * t, BSTNODE * node){
    if (node == 0) return;
    recurssiveFree(t,getBSTNODEleft(node));
    recurssiveFree(t,getBSTNODEright(node));
    freeBSTNODE(node,t->free);
}
extern void freeBST(BST *t){
    BSTNODE * ptr = t->root;
    recurssiveFree(t,ptr);
    free((BST *) t);
    return;
}
