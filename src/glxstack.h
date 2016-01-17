/*
 * This is a header file for glxstack.c
 *
 * @name: glxstack.h
 * @author: extradiable
 * @date: mar may 12 06:45:44 CDT 2015
 * @update: sáb oct 17 07:40:23 CDT 2015
 * 
 */

#ifndef STACK_H
#define STACK_H 1
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

/* The following structures are used to represent a basic stack */

struct Item {
  struct Item *next;
  void *data;
};

typedef struct Item Item;

typedef struct {
  Item *top;
  uint32_t size;
} Stack;

typedef struct {
 
  /* Convenient function to create a Stack */
  Stack * (* create) (void);
 
  /* Retrieve the next element from the Stack */
  void * (* pop) (Stack *S);

  /* Stacks an element */
  void (* push) (Stack *S, void *data);

  /* returns element without pop it */
  void * (* peek) (Stack *S);

  /* Clear the stack and then free it */
  void (* destroy) (Stack **S, void (* destroyfn) (void *data));

} stack_lib;

extern stack_lib const stack;

#endif
