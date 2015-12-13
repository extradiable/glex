/*
 * This file encapsulates the logic for interacting with State structure.
 * A State is represented as a nested Red Black Tree.
 * Recall than given the algorithm used to generate the states of the automaton,
 * a state consists of a set of positions. A node in structure State depicts a
 * single position say p. Hence, a node represents a branch of all states that
 * are prefix of each other up to pos. p. For instance, consider the following
 * states:
 *
 * S1 = {2, 5, 7, 8}
 * S2 = {2, 5, 8}
 * S3 = {2, 5, 1}
 * S4 = {2, 5}
 *
 * All S[1-4] states have positions 2 and 5 as a prefix. A set is unnordered 
 * but we consider order to guarantee that the algorithm works correctly. 
 * Consider States S[1-4] again, in the root of the three exists a node whose
 * key is pos. 2, the data of such node is a tree that contains a node whose
 * key is pos. 5. The data of such node is a tree that contains 3 keys namely
 * 1, 7 and 8. If a node represents a State, (i.e. from the root of the three
 * there is a path that contains all positions for a given state) then the
 * attributes positions, isFinal and Id are set just once.
 *
 * @name: glxstates.c
 * @author: extradiable
 * @date: sáb oct 17 11:00:54 CDT 2015
 * @update: dom oct 25 16:45:40 CST 2015
 *
 */

#include "glxstate.h"

uint16_t numStates = 0;
int8_t found = 0;

State *createState(void) {
  State *S = (State *) malloc(sizeof(State));
  if(S) {
    S->id = 0;
    S->isFinal = 0;
    S->positions = createRBT();
    S->T = createRBT();
  } else {
    fprintf(stderr, "Insufficient memory to create State.");
    exit(EXIT_FAILURE);
  }
  return S; 
}

State *insertState(RBNode **NT, RBNode *T, int16_t finalPos){
  if(NT && *NT && T){
    int8_t isFinal = 0;
    RBNode *N = NULL;
    State *state = createState();
    RBNode *first = findMinRBT(T);
    RBNode *next = first;
    do {
      // duplicates are not allowed, there is no risk to
      // overwrite the RBNode descriptor
      isFinal = isFinal || (next->key == finalPos);
      N = insertRBN(NT, next->key, state);
      State *tmp = N->data;
      NT = &(tmp->T);
      if(N->data == state) {
        // new node was created, descriptor cannot be reused
        state = createState();
      }
      next = next->next;
    } while (next != first);
    state = (State *) N->data;
    if(state->id == 0) {
      // a new state was found
      state->id = ++numStates;
      state->positions = T;
      state->isFinal = isFinal;
      found = 1;
    } else {
      found = 0;
    }
//printf("state is: %d, found is: %d, pointer: %p\n", state->id, found, state);
    return state;
  } else {
    fprintf(stderr, "insertState: invalid state call");
    exit(EXIT_FAILURE);
  }
}

void freeStates(RBNode *T){
  if(!isEmptyRBT(T)){
    RBNode *current = T;
    do {
      freeState(T->data);
      current = current->next;
    } while(current != T);
  }
  freeRBT(T);
}

void freeState(State *S){
  if(S != NULL) {
    freeRBT(S->positions);
    freeStates(S->T);
  }
}
