/*
 * This is a header file for glxptnode.c
 * 
 * name: glxptnode.h
 * author: extradiable
 * date: sáb may  2 19:28:02 CDT 2015
 * update: dom oct 25 17:04:51 CST 2015
 */

#ifndef PTNODE_H
#define PTNODE_H 1
#include <stdint.h>
#include <stdlib.h>
#include "glxrbtree.h"

/* This structure is intended to represent the parse tree. 
 * token: is a character representing a class of elements
 * lexeme: the instance of the token found
 * firstpos: 
 * lastpos:
 */
typedef struct {
  uint8_t id;
  uint8_t lexeme;
  uint8_t symbol;
  uint8_t nullable;
  //int16_t leaf;
  RBT *firstpos;
  RBT *lastpos;
} PTNode;

typedef struct {

  /* Create a parse tree node */
  PTNode * (* create) (int8_t id, uint8_t lexeme, uint8_t symbol);

  /* Convenient function to output syntax tree information */
  void (* print) (FILE *fp, PTNode *N);

  /* Frees the allocated memory for this node */
  void (*destroy) (PTNode **ptrN);

} ptnode_lib;

extern ptnode_lib const ptn;

#endif
