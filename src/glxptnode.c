/*
 * The first step towards the creation of the deterministic finite automaton
 * that recognize the language provided by the input regular expression is to
 * construct the Parse Tree. The Parse Tree is a binary tree that reflects the 
 * syntactic structure of the given regular expression. PTNode hold information
 * of Parse Tree nodes.
 *
 * name: glxptnode.h
 * purpose: Provides the means for manipulating nodes in the parse tree
 * author: extradiable
 * date: sáb may  2 19:28:02 CDT 2015
 * update: dom oct 25 17:04:39 CST 2015
 */

#include "glxptnode.h"

void freePTNode(PTNode *N);
void dumpST(FILE *fpLog, PTNode *T, int16_t *node, int16_t *leaf);

PTNode *createPTNode(int8_t token, uint8_t lexeme, uint8_t symbol){
  PTNode *N = (PTNode *) malloc(sizeof(PTNode));
  if(N != NULL){
    N->nullable = 0;
    N->token = token;
    N->lexeme = lexeme;
    N->symbol = symbol;
    N->leaf = -1;
    N->firstpos = rbt.create();
    N->lastpos = rbt.create();
    //TODO: ptnode is no longer a tree but a stack
    //N->left = NULL;
    //N->right = NULL;
  }else{
    fprintf(stderr, "Insuficient memory to create PTNode");  
    exit(EXIT_SUCCESS);
  }
  return N;
}

//TODO: ptnode is no longer a tree but a stack
/*
void preordenPT(PTNode *T, void(*ptrF)(void *)){
  if(T!=NULL){
    (*ptrF)(T);
    preordenPT(T->left, ptrF);
    preordenPT(T->right, ptrF);
  }
}

void inordenPT(PTNode *T, void (*ptrF)(void *)){
  if(T!=NULL){
    inordenPT(T->left, ptrF);
    (*ptrF)(T);
    inordenPT(T->right, ptrF);
  }
}

void postordenPT(PTNode *T, void (*ptrF)(PTNode *)){
  if(T!=NULL){
    postordenPT(T->left, ptrF);
    postordenPT(T->right, ptrF);
    (*ptrF)(T);
  }
}
*/
/* Frees the allocated memory used for this node */
void freePTNode(PTNode *T){
  rbt.destroy(&T->firstpos, NULL);
  rbt.destroy(&T->lastpos, NULL);
  free(T);
}

void freePT(PTNode *T){
  //TODO: ptnode is no longer a tree but a stack of nodes
  //postordenPT(T, (void *) &freePTNode);
}

//TODO: ptnode is no longer a tree but a stack
/*
void dumpST(FILE *fpLog, PTNode *T, int16_t *node, int16_t *leaf){
  if(T){
    int8_t l = T->left || T->right ? 0 : 1;
    dumpST(fpLog, T->left, node, leaf);
    dumpST(fpLog, T->right, node, leaf);
    fprintf(fpLog, "[id: %d leaf: %d lexeme: %c]\n", *node, l ? *leaf : 0, T->lexeme);
    fprintf(fpLog, "\tNullable:%d\n", T->nullable);
    fprintf(fpLog, "\tFirstPos:[ ");
    dumpRBT(fpLog, T->firstpos);
    fprintf(fpLog, "]\n");
    fprintf(fpLog, "\tLastPos:[ ");
    dumpRBT(fpLog, T->lastpos);
    fprintf(fpLog, "]\n");
    *leaf = *leaf + l;
    (*node)++;
  }
}

void dumpSyntaxTree(FILE *fpLog, PTNode *T){
  if(fpLog){
    fprintf(fpLog, "\nDumping nullable, firstpos and lastpos information\n\n");
    int16_t node = 1, leaf = 1;
    dumpST(fpLog, T, &node, &leaf);
  }
}
*/
