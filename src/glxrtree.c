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
 * @updates:
 *
 * 2015/12/13 - Use of struct to simulate namespace. 
 */

#include "glxrtree.h"

static void destroyBranches(RBT *T, void (* destroyfn) (void *data));

static RTree *create() {
  RTree *R = (RTree *) malloc(sizeof(RTree));
  if (R != NULL) {
    R->value = NULL;
    R->branches = rbt.create();
  } else {
    err.show("glxrtree", "create", "Insufficient memory to create RTree");
  }
  return R;
}

static RTree *getBranch(RTree *R, int8_t key) {
  if (R != NULL) {
    RBT *N =  rbt.insert(&(R->branches), key, NULL);
    if (N->data == NULL) {
      N->data = create();
    }
    return N->data;
  } else {
    err.show("glxrtree", "getBranch", "An attempt was made to use a NULL RBT");
  }
}

static void setValue(RTree *R, void *value) {
  if (R != NULL) {
    R->value = value;
  } else {
    err.show("glxrtree", "setValue", "An attempt was made to set value in a NULL RTree");
  }
}

static void destroy(RTree **pR, void (* destroyfn) (void *data)) {
  if (pR != NULL && *pR != NULL) {
    RTree *R = *pR;
    if (destroyfn != NULL) {
      destroyfn(R->value);
    }
    R->value = NULL;
    destroyBranches(R->branches, destroyfn);
    free(R);
    *pR = NULL;
  }
}

static void destroyBranches(RBT *T, void (* destroyfn) (void *data)) {
  if(T != NULL && !rbt.isEmpty(T)) {
    RBT *branch = T;
    do {
      destroy(branch->data, destroyfn);
      branch->data = NULL;
      branch = branch->next;
    } while (branch != T);
    rbt.destroy(&T, NULL);
  }
}

rtree_lib const rtree = { 
  create, 
  getBranch,
  setValue,
  destroy 
};
