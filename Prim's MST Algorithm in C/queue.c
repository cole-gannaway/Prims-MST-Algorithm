/*
 *  Written by Cole Gannaway
 *  A generic QUEUE class. When constructed takes function pointers
 *  to display and free the generic values stored in the QUEUE.
 *  This class uses an underlying SLL to implement functionality.
 */

#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include <assert.h>
#include "sll.h"
struct queue{
    SLL * head;
    void (*display)(void *,FILE *);
    void (*free)(void *);
};
extern QUEUE *newQUEUE(void (*d)(void *,FILE *),void (*f)(void *)){
    QUEUE *items = malloc(sizeof(QUEUE));
    assert(items != 0);
    SLL * list = newSLL(d,f);
    items->head = list;
    items->display = d;
    items->free = f;
    return items;
}
extern void enqueue(QUEUE *items,void *value){
    insertSLL(items->head, sizeSLL(items->head), value);
    return;
}
extern void *dequeue(QUEUE *items){
    void * value = removeSLL(items->head, 0);
    return value;
}
extern void *peekQUEUE(QUEUE *items){
    void * value = getSLL(items->head, 0);
    return value;
}
extern int sizeQUEUE(QUEUE *items){
    return sizeSLL(items->head);
}
extern void displayQUEUE(QUEUE *items,FILE * fp){
    printf("<");
    void * ptr;
    int length = sizeSLL(items->head);
    for (int a = 0; a < length; a++){
        ptr = getSLL((items->head), a);
        if (items->display)items->display(ptr, fp);
        if (a < (length - 1)){
            printf(",");
        }
    }
    printf(">");
    return;
}
extern void displayQUEUEdebug(QUEUE *items,FILE * fp){
    displaySLLdebug(items->head, fp);
}
extern void freeQUEUE(QUEUE *items){
    freeSLL(items->head);
    free((QUEUE *) items);
    return;
}
