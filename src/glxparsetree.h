/*
 * This is a header for glxparsetree.c
 *
 * @name: glxparsetree.h
 * @author: extradiable
 * @date: sáb may  2 21:11:01 CDT 2015
 * @update: dom oct 25 17:09:39 CST 2015
 */

#ifndef PARSINGTREE_H
#define PARSINGTREE_H 1
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "glxreader.h"
#include "glxrbtree.h"
#include "glxptnode.h"
#include "glxstack.h"

struct Transition {
  int16_t currentState;
  uint8_t symbol;
  int16_t nextState;
};

struct ParseInfo{
  uint16_t numStates;
  int16_t numSymbols;
  uint8_t *alphabet;
  int16_t finalState;
  struct Stack *transitions;
};

typedef struct Transition Transition;

typedef struct ParseInfo ParseInfo;

/* Reads a file containing a regular expression and generates 
 * the parse tree
 */
PTNode *glx_parse(uint8_t *fileName);

/* This function computes all necessary functions before 
 * generating the deterministic finite automaton
 */
void glx_makeAutomaton(uint8_t *fileName);

#endif
