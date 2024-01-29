/*
 *  Written by Cole Gannaway
 *  A generic DLL class. When constructed takes function pointers
 *  to display and free the generic values stored in the DLL.
 */

#include <stdio.h>
#include <stdlib.h>
#include "dll.h"
#include <assert.h>

typedef struct node{
    void * data;
    struct node * next;
    struct node * prev;
}NODE;

struct dll{
    NODE * head;
    NODE * tail;
    NODE * currentNode;
    int size;
    void (*display)(void *,FILE *);
    void (*free)(void *);
};

static NODE * newNode(void * value){
    NODE * p = malloc(sizeof(NODE));
    assert(p != 0);
    p->data = value;
    p->next = NULL;
    p->prev = NULL;
    return p;
}

static void freeNODE(void * v){
    if (v != 0) free((NODE *) v);
}

static void setDLLsize(DLL *items, int x){
    assert(items != 0);
    items->size = x;
}
extern DLL *newDLL(void (*d)(void *,FILE *),void (*f)(void *)){
    DLL *items = malloc(sizeof(DLL));
    assert(items != 0);
    items->head = 0;
    items->tail = 0;
    items->currentNode = 0;
    items->size = 0;
    items->display = d;
    items->free = f;
    return items;
}

extern int sizeDLL(DLL *items){
    return items->size;
}

static NODE *findNode(DLL * items, int index){
    if (items == 0) return 0;
    int size = sizeDLL(items);
    NODE * ptr = 0;
    if (index < (size/2)){
        ptr = items->head;
        for (int a = 0; a < (index); a++){
            ptr = ptr->next;
        }
    }
    else{
        ptr = items->tail;
        for (int a = (items->size - 1); a > index; a--){
            ptr = ptr->prev;
        }
    }
    return ptr;
}
extern void* insertDLL(DLL *items,int index,void *value){
    // the index is out of bounds
    assert(!(index < 0));
    assert(!(index > items->size));

    NODE * createdNode = newNode(value);
    /* Inserting at the head */
    if (index == 0){
        /* Empty List */
        if (items->size == 0){
            items->tail = createdNode;
        }
        /* Not empty List */
        else{
            items->head->prev = createdNode;
        }
        createdNode->next = items->head;
        items->head = createdNode;
        createdNode->prev = NULL;
    }
    /* Inserting at the tail */
    else if (index == (items->size)){
        items->tail->next = createdNode;
        createdNode->next = NULL;
        createdNode->prev = items->tail;
        items->tail = createdNode;
    }
    /* Inserting somewhere in the middle*/
    else {
        NODE * after = findNode(items,index);
        NODE * before = after->prev;
        createdNode->next = after;
        createdNode->prev = before;
        before->next = createdNode;
        after->prev = createdNode;
    }
    items->size = (items->size) + 1;
    return createdNode;
}

extern void *removeDLL(DLL *items,int index){
    // the index is out of bounds
    if ((index < 0) || (index > (items->size))){
        return NULL;
    }
    // list is empty
    if (items->size == 0){
        return NULL;
    }
    void * pop;
    NODE * deleteNode;
    //removing from the head
    if (index == 0){
        deleteNode = items->head;
        pop = items->head->data;
        // if its the only one in the list
        if (items->head == items->tail){
            items->head = NULL;
            items->tail = NULL;
        }
        // list is bigger than one
        else{
            items->head = items->head->next;
            items->head->prev = NULL;
        }
        freeNODE(deleteNode);
        items->size = (items->size - 1);
        return pop;
    }
    //removing from the tail
    else if (index == (items->size - 1)){
        deleteNode = items->tail;
        pop = items->tail->data;
        items->tail = items->tail->prev;
        items->tail->next = NULL;
        freeNODE(deleteNode);
        items->size = (items->size - 1);
        return pop;
    }
    
    /* Removing somewhere in the middle*/
    else {
        deleteNode = findNode(items,index);
        NODE * before = deleteNode->prev;
        NODE * after = deleteNode->next;
        // removing from the front half
        pop = deleteNode->data;
        before->next = after;
        (after)->prev = before;
    }
    freeNODE(deleteNode);
    items->size = (items->size - 1);
    return pop;
}

extern void unionDLL(DLL *recipient,DLL *donor){
    if (recipient && (recipient->size > 0)){
        if (donor->head){
            recipient->tail->next = donor->head;
            donor->head->prev = recipient->tail;
            recipient->tail = donor->tail;
            recipient->size = (recipient->size) + (donor->size);
        }
    }
    else{
        recipient->head = donor->head;
        recipient->tail = donor->tail;
        recipient->size = donor->size;
    }
    donor->head = NULL;
    donor->tail = NULL;
    donor->size = 0;
    return;
}

extern void *getDLL(DLL *items,int index){
    // list is empty
    if (items->size == 0){
        return NULL;
    }
    // out of bounds reference
    if ((index < 0) || (index > (items->size - 1))){
        return NULL;
    }
    if (index == 0){
        return (items->head->data);
    }
    // to make it run constant time from the tail
    if (index == (items->size - 1)){
        return (items->tail->data);
    }
    NODE * ptr = findNode(items,index);
    assert(ptr->data != 0);
    return ptr->data;
}

extern void *setDLL(DLL *items,int index,void *value){
    // the index is out of bounds
    if ((index < 0) || (index > (items->size))){
        return NULL;
    }
    // set or appended to the back of list
    if (index == items->size){
        insertDLL(items,index,value);
        return NULL;
    }
    // set/change the tail value
    if (index == (items->size - 1)){
        void * prevData = items->tail->data;
        items->tail->data = value;
        return prevData;
    }
    NODE * ptr = findNode(items,index);
    void * prevData = ptr->data;
    ptr->data = value;
    return prevData;
}

extern void displayDLL(DLL *items,FILE * fp){
    fprintf(fp,"{{");
    NODE * ptr;
    ptr = items->head;
    while (ptr) {
        if (items->display){
            items->display((ptr->data), fp);
        }
        ptr = ptr->next;
        if (ptr){
            fprintf(fp,",");
        }
    }
    fprintf(fp,"}}");
    return;
}

extern void displayDLLdebug(DLL *items,FILE * fp){
    fprintf(fp,"head->");
    fprintf(fp,"{{");
    NODE * ptr;
    ptr = items->head;
    while (ptr) {
        if (items->display)items->display((ptr->data), fp);
        ptr= ptr->next;
        if (ptr){
            fprintf(fp,",");
        }
    }
    fprintf(fp,"}},");
    fprintf(fp,"tail->{{");
    NODE * x = items->tail;
    x = items->tail;
    if (x){
        if (items->display)items->display((x->data), fp);
    }
    fprintf(fp,"}}");
    return;
}

extern void freeDLL(DLL *items){
    NODE * ptr;
    NODE * prev;
    ptr = items->head;
    while (ptr) {
        prev = ptr;
        if (items->free && ptr->data)items->free(ptr->data);
        ptr = ptr->next;
        if (prev) freeNODE(prev);
        prev = NULL;
    }
    free((DLL *) items);
    return;
}

extern void removeDLLall(DLL *items){
    NODE * ptr;
    NODE * prev;
    ptr = items->head;
    while (ptr != 0) {
        prev = ptr;
        ptr = ptr->next;
        freeNODE(prev);
    }
    items->head = 0;
    items->tail = 0;
    setDLLsize(items,0);
}
extern void *removeDLLnode(DLL *items, void * node){
    NODE * found = (NODE *)node;
    assert(found != 0);
    assert(items != 0);
    // it is the only one in the list
    if (sizeDLL(items) == 1){
        items->head = 0;
        items->tail = 0;
    }
    else{
        NODE * before = 0;
        NODE * after = 0;
        // removing the head
        if (items->head == found){
            after = found->next;
            after->prev = 0;
            items->head = after;
        }
        else if (items->tail == found){
            before = found->prev;
            before->next = 0;
            items->tail = before;
        }
        // Removing somewhere in the middle//
        else{
            before = found->prev;
            after = found->next;
            before->next = after;
            after->prev = before;
        }
    }
    void * pop = found->data;
    freeNODE(found);
    items->size = (items->size - 1);
    return pop;
}
extern void firstDLL(DLL *items){
    assert(items != 0);
    items->currentNode = items->head;
}
extern void lastDLL(DLL *items){
    assert(items != 0);
    items->currentNode = items->tail;
}
extern int moreDLL(DLL *items){
    assert(items != 0);
    if (items->currentNode == 0) return 0;
    else return 1;
}
extern void nextDLL(DLL *items){
    assert(items != 0);
    assert(items->currentNode != 0);
    items->currentNode = items->currentNode->next;
}
extern void prevDLL(DLL *items){
    assert(items != 0);
    assert(items->currentNode != 0);
    items->currentNode = items->currentNode->prev;
}
extern void *currentDLL(DLL *items){
    assert(items != 0);
    if (items->currentNode == 0) return 0;
    return items->currentNode->data;
}
