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

static PTNode *create(int8_t id, uint8_t lexeme, uint8_t symbol) {
  PTNode *N = (PTNode *) malloc(sizeof(PTNode));
  if(N != NULL) {
    N->nullable = 0;
    N->id = id;
    N->lexeme = lexeme;
    N->symbol = symbol;
    N->firstpos = rbt.create();
    N->lastpos = rbt.create();
    //TODO: ptnode is no longer a tree but a stack
    //N->leaf = -1;
    //N->left = NULL;
    //N->right = NULL;
  }else {
    fprintf(stderr, "Insuficient memory to create PTNode");  
    exit(EXIT_SUCCESS);
  }
  return N;
}

/* Destroy this node */
static void destroy(PTNode **ptrN) {
  PTNode *N = *ptrN;
  rbt.destroy(&(N->firstpos), NULL);
  rbt.destroy(&(N->lastpos), NULL);
  free(N);
}

//TODO: ptnode is no longer a tree but a stack
static void print(FILE *fp, PTNode *N) {
  static char separator = '\0';
  if(N) {
    fprintf(fp, "%c{\n", separator);
    fprintf(fp, "  id: %d\n", N->id);
    fprintf(fp, "  lexeme: %c\n", N->lexeme);
    fprintf(fp, "  Nullable:%d\n", N->nullable);
    fprintf(fp, "  FirstPos:[");
    rbt.print(fp, N->firstpos);
    fprintf(fp, "]\n");
    fprintf(fp, "  LastPos:[");
    rbt.print(fp, N->lastpos);
    fprintf(fp, "]\n");
    fprintf(fp, "}\n");
  }
  separator = ',';
}

ptnode_lib const ptn = { 
  create, 
  print,
  destroy
};
