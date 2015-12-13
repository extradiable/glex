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
struct PTNode{
  int8_t token;
  uint8_t lexeme;
  uint8_t symbol;
  int8_t nullable;
  int16_t leaf;
  RBNode *firstpos;
  RBNode *lastpos;
};

typedef struct PTNode PTNode;

/* Convenient function to create a parse tree node */
PTNode *createPTNode(int8_t token, uint8_t lexeme, uint8_t symbol);

/* Walk the parse tree in preorden executing ptrF */
void preordenPT(PTNode *T, void (*ptrF)(void *));

/* Walk the parse tree in inorden executing ptrF  */
void inordenPT(PTNode *T, void (*ptrF)(void *));

/* Walk the parse tree in postorden executing ptrF */
void postordenPT(PTNode *T, void (*ptrF)(PTNode *));

/* Frees the allocated memory for the parse tree rooted at T */
void freePT(PTNode *T);

/* Convenient function to output syntax tree information */
void dumpSyntaxTree(FILE *fpLog, PTNode *T);

#endif
