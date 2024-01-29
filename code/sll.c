/*
 *  Written by Cole Gannaway
 *  A generic SLL class. When constructed takes function pointers
 *  to display and free the generic values stored in the SLL.
 */

#include <stdio.h>
#include <stdlib.h>
#include "sll.h"
#include <assert.h>

typedef struct node{
    void * data;
    struct node * next;
}NODE;

struct sll{
    NODE * head;
    NODE * tail;
    int size;
    void (*display)(void *,FILE *);
    void (*free)(void *);
};

// Private Functions //
static NODE * newNode(void * value){
    NODE * p = malloc(sizeof(NODE));
    assert(p != 0);
    p->data = value;
    p->next = NULL;
    return p;
}
static void freeNODE(void * v){
    free((NODE *) v);
}
// Public Functions //
extern SLL *newSLL(void (*d)(void *,FILE *),void (*f)(void *)){
    SLL *items = malloc(sizeof(SLL));
    assert(items != 0);
    items->head = 0;
    items->tail = 0;
    items->size = 0;
    items->display = d;
    items->free = f;
    return items;
}

extern void insertSLL(SLL *items,int index,void *value){
    if ((index < 0) || (index > (items->size))){
        return;
    }
    NODE * createdNode = newNode(value);
    // Inserting at the head
    if (index == 0){
        if ( items->size == 0){
            items->tail = createdNode;
        }
        else {
        }
        createdNode->next = items->head;
        items->head = createdNode;
    }
    // Inserting at the tail
    else if (index == (items->size)){
        if (items->size == 0){
            items->head = createdNode;
        }
        else {
            items->tail->next = createdNode;
        }
        createdNode->next = NULL;
        items->tail = createdNode;
    }
    // Inserting somewhere in the middle
    else {
        NODE * ptr = items->head;
        for (int a = 0; a < (index - 1); a++){
            ptr = ptr->next;
        }
        createdNode->next = ptr->next;
        ptr->next = createdNode;
    }
    items->size = (items->size) + 1;
    return;
}

extern void *removeSLL(SLL *items,int index){
    // the index is out of bounds
    if ((index < 0) || (index > (items->size))){
        return NULL;
    }
    if (items->size == 0){
        return NULL;
    }
    void * pop;
    NODE * deleteNode;
    
    // removing from the head
    if (index == 0){
        deleteNode = items->head;
        pop = items->head->data;
        if (items->head == items->tail){
            items->head = NULL;
            items->tail = NULL;
        }
        else{
            items->head = items->head->next;
        }
        freeNODE(deleteNode);
        items->size = (items->size - 1);
        return pop;
    }
    NODE * ptr = items->head;
    NODE * before = ptr;
    for (int a = 0; a < index; a++){
        before = ptr;
        ptr = ptr->next;
    }
    assert(ptr != 0);
    pop = ptr->data;
    
    before->next = ptr->next;
    
    freeNODE(ptr);
    items->size = (items->size) - 1;
    return pop;
}

extern void unionSLL(SLL *recipient,SLL *donor){
    if (recipient && (recipient->size > 0)){
        if (donor->head){
            recipient->tail->next = donor->head;
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

extern void *getSLL(SLL *items,int index){
    if (items->size == 0){
        return NULL;
    }
    // accessing out of bounds index
    if ((index < 0) || (index > (items->size - 1))){
        return NULL;
    }
    // to make it run constant time from the tail
    if (index == (items->size - 1)){
        return (items->tail->data);
    }
    NODE * ptr = items->head;
    for (int a = 0; a < index; a++){
        if (ptr == NULL){
            break;
        }
        ptr = ptr->next;
    }
    assert(ptr->data != 0);
    return ptr->data;
}

extern void *setSLL(SLL *items,int index,void *value){
    // set or appended to the back of list
    if (index == items->size){
        insertSLL(items,index,value);
        return NULL;
    }
    // set/change the tail value
    if (index == (items->size - 1)){
        void * prevData = items->tail->data;
        items->tail->data = value;
        return prevData;
    }
    NODE * ptr = items->head;
    int a = 0;
    while (ptr && (a < index)){
        ptr = ptr->next;
        a++;
    }
    if (ptr == NULL) return NULL;
    void * prevData = ptr->data;
    ptr->data = value;
    return prevData;
}

extern int sizeSLL(SLL *items){
    return items->size;
}

extern void displaySLL(SLL *items,FILE * fp){
    fprintf(fp,"{");
    NODE * ptr;
    ptr = items->head;
    while (ptr) {
        if (items->display)items->display((ptr->data), fp);
        ptr = ptr->next;
        if (ptr){
            fprintf(fp,",");
        }
    }
    fprintf(fp,"}");
    return;
}

extern void displaySLLdebug(SLL *items,FILE * fp){
    fprintf(fp,"head->");
    fprintf(fp,"{");
    NODE * ptr;
    ptr = items->head;
    while (ptr) {
        if (items->display)items->display((ptr->data), fp);
        ptr = ptr->next;
        if (ptr){
            fprintf(fp,",");
        }
    }
    fprintf(fp,"},");
    fprintf(fp,"tail->{");
    NODE * x = items->tail;
    x = items->tail;
    if (x){
        if (items->display)items->display((x->data), fp);
    }
    fprintf(fp,"}");
    return;
}
extern void freeSLL(SLL *items){
    NODE * ptr = items->head;
    NODE * prev = ptr;
    while (ptr) {
        prev = ptr;
        if (items->free && ptr->data) items->free(ptr->data);
        ptr = ptr->next;
        if (prev) freeNODE(prev);
        prev = NULL;
    }
    free((SLL *) items);
    return;
}
