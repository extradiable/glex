/*
 * This is a header file for glxqueue.c
 *
 * @name: glxqueue.h
 * @author: extradiable
 * @date: dom dic  6 18:18:32 CST 2015
 * @update: dom dic  6 18:18:32 CST 2015
 * 
 */

#ifndef QUEUE_H
#define QUEUE_H 1
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "glxerrors.h"

/* The following structures are used to represent a basic queue */

struct Item {
  struct Item *next;
  void *data;
};

typedef struct Item Item;

typedef struct {
  Item *first;
  Item *last;
  uint32_t size;
} Queue;

typedef struct {

  /* Convenient function to create a Queue */
  Queue * (* create) (void);
  
  /* Retrieve the next element from the Queue */
  void * (* dequeue) (Queue *Q);
  
  /* Enqueue an element */
  void (* enqueue) (Queue *Q, void *data);
  
  /* returns element without dequeue it */
  void * (* peek) (Queue *Q);
  
  /* Clear the queue and then free it */
  void (* destroy) (Queue **Q, void (* destroyfn) (void *data));

} queue_lib;

extern queue_lib const queue;

#endif
