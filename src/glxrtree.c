/*
 * An RTree node is used to construct a recursive Red Black Tree structure.
 * An RTree node contains a set of branches each of which is a Red Black Tree
 * node that contains as data an RTree.
 * 
 * This recursive structure is used as a kind of trie. You can store 
 * (key, value) pairs as in a dictionary. The key is the concatenation of 
 * unsigned byte values. You can navigate the structure using getBranch 
 * function with every unsigned byte value as the key. When you reach the
 * desired branch, you can store your value using setValue function.
 *
 * @name: glxrtree.c
 * @author: extradiable
 * @date: mié dic  2 06:35:54 CST 2015
 * @update:
 *
 * 11/07/15 - X
 */

#include "glxrtree.h"

void destroyBranches(RBNode *T);

RTree *createRTree() {
  RTree *R = (RTree *) malloc(sizeof(RTree));
  if (R) {
    R->value = NULL;
    R->branches = createRBT();
  } else {
    fprintf(stderr, "Insufficient memory to create RTree.");
    exit(EXIT_FAILURE);
  }
  return R;
}

void *getBranch(RBTree *R, int8_t key) {
  if (R) {
    RBNode *N =  insertRBN(&(R->branches), key, NULL);
    if (N->data == NULL) {
      N->data = createRTree();
    }
    return N->data;
  } else {
    fprintf(stderr, "getBranch: invalid RTree call");
    exit(EXIT_FAILURE);
  }
}

void setValue(RTree *R, void *value) {
  if (R) {
    RTree->value = data;
  } else {
    fprintf(stderr, "setValue: invalid RTree call");
    exit(EXIT_FAILURE);
  }
}

void destroyBranches(RBNode *T, void (* fn) (void *data)){
  if(T != NULL && !isEmptyRBT(T)){
    RBNode *branch = T;
    do {
      destroyRTree(branch->data, fn);
      branch->data = NULL;
      branch = branch->next;
    } while (branch != T);
    freeRBT(T);
  }
}

void destroyRTree(RTree *R, void (* fn) (void *data)){
  if(R != NULL) {
    if (fn != NULL) {
      fn(R->value);
    }
    R->value = NULL;
    destroyBranches(R->branches, fn);
    free(R);
  }
}
