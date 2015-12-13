/*
 * This is a header for glxrtree.c
 *
 * @name: glxrtree.h
 * @author: extradiable
 * @date: mié dic  2 06:13:05 CST 2015
 * @updates:
 *  
 * 11/07/15 - X
 */

#ifndef RTREE_H
#define RTREE_H 1
#include "glxrbtree.h"
#include <stdint.h>
#include <stdlib.h>

/* Recursive Red Black Tree Structure */
struct RTree {
  void *value;
  RBNode *branches;
}

typedef struct RTree RTree;

RTree *createRTree(void);

/*
 * You can navigate this recursive structure from branch to branch by using 
 * this function.
 */
RTree *getBranch(RTree *R, int8_t key);

/*
 * Stores a value in the RTree node given.
 */
void setValue(RTree *R, void *value);

/*
 * Realloc all memory used by the tree. If you supply a destroy function, it 
 * will be used to destroy every value stored by this structure. If you 
 * provide a NULL function be careful free your memory before calling this 
 * function as you can lose reference to unused memory blocks.
 */
void destroyRTree(RTree *R, void (* destroyfn) (void *data));

#endif
