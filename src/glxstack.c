/*
 * Implementation of a stack using a simple linked list
 * 
 * @name: glxstack.c
 * @author: extradiable
 * @date: mar may 12 20:49:30 CDT 2015
 * @update: sáb oct 17 07:39:52 CDT 2015
 */

#include "glxstack.h"

void error(char *fn, char *msg);

void error(char *fn, char *msg){
  fprintf(
    #ifndef DEBUG_H
    stderr,
    #else
    stdout, 
    #endif
    "[error]: Source: glxstack:%s Message: %s.\n", fn, msg
  );
  #ifndef DEBUG_H
  exit(EXIT_FAILURE);
  #endif
}

static Stack *createStack() {
  Stack *S = (Stack *) malloc(sizeof(Stack *));
  if (S) {
    S->top = NULL;
    S->size = 0;
  } else {
    error("createStack", "Insufficient memory to create Stack");
  }
  return S;
}

static Item *createItem(void *data) {
  Item *I = (Item *) malloc(sizeof(Item *));  
  if (I) {
    I->data = data;
    I->next = NULL;
  } else {
    error("createItem", "Insuficcient memory to create Item");
  }
  return I;
}

static void *pop(Stack *S) {
  if (S->size) {
    Item *I = S->top;
    void *data = I->data;
    S->top = I->next;
    S->size--;
    free(I);
    return data;
  } else {
    error("pop", "An attempt was made to pop() an empty Stack");
  }
  return NULL;
}

static void push(Stack *S, void *data) {
  Item *I = createItem(data);
  if (S->size) {
    I->next = S->top;
  }
  S->top = I;
  S->size++;
}

static void *peek(Stack *S) {
  if (S != NULL && S->size) {
    return S->top->data;
  } else {
    error("peek", "An attempt was made to peek() an empty Stack");
  }
  return NULL;
}

static void destroy(Stack **pS, void (* destroyfn) (void *data)) {
  if (pS != NULL && *pS != NULL) {
    Stack *S = *pS;
    while (S->size) {  
      void *data = pop(S);
      if (destroyfn) {
        destroyfn(data);
      }
    }
    free(S);
    *pS = NULL;
  } else {
    error("destroyStack", "An attempt was made to destroy an empty Stack");
  }
}

stack_lib const stack = { 
  createStack, 
  pop,
  push,
  peek,
  destroy 
};
