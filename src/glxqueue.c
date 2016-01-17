/*
 * Implementation of a queue using a simple linked list
 * 
 * @name: glxqueue.c
 * @author: extradiable
 * @date: dom dic  6 18:39:29 CST 2015
 * @update: dom dic  6 18:39:43 CST 2015
 */

#include "glxqueue.h"

static Queue *createQueue() {
  Queue *Q = (Queue *) malloc(sizeof(Queue *));
  if (Q) {
    Q->first = NULL;
    Q->last = NULL;
    Q->size = 0;
  } else {
      err.show("glxqueue", "createQueue", "Unable to create Queue");
  }
  return Q;
}

static Item *createItem(void *data) {
  Item *I = (Item *) malloc(sizeof(Item *));  
  if (I) {
    I->data = data;
    I->next = NULL;
  } else {
      err.show("glxqueue", "createItem", "Unable to create Item");
  }
  return I;
}

static void *dequeue(Queue *Q) {
  if (Q->size) {
    Item *I = Q->first;
    void *data = I->data;
    Q->first = I->next;
    Q->size--;
    free(I);
    return data;
  }
  return NULL;
}

static void enqueue(Queue *Q, void *data) {
  Item *I = createItem(data);
  if (Q->size) {
    Q->last->next = I;
  } else {
    Q->first = I;
  }
  Q->last = I;
  Q->size++;
}

static void *peek(Queue *Q) {
  if (Q != NULL && Q->size) {
    return Q->first->data;
  } else {
   err.show("glxqueue", "peek", "An attempt was made to peek() an empty Queue");
  }
  return NULL;
}

static void destroy(Queue **pQ, void (* destroyfn) (void *data)) {
  if (pQ && *pQ != NULL) {
    Queue *Q = *pQ;
    while (Q->size) {
      void *data = dequeue(Q);
      if (destroyfn) {
        destroyfn(data);
      }
    }
    free(Q);
    *pQ = NULL;
  } else {
    err.show("glxqueue", "destroyQueue", "An attempt was made to destroy an empty Queue");
  }
}

queue_lib const queue = { 
  createQueue,
  dequeue,
  enqueue,
  peek,
  destroy 
};
