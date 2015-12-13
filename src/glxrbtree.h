/**
 * This is a header file for glxrbtree.c
 *
 * @name: glxrbnode.h
 * @author: extradiable
 * @date: sáb may  2 19:09:58 CDT 2015
 * @updated: dom oct 25 16:57:08 CST 2015
 */

#ifndef RBTREE_H
#define RBTREE_H 1
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

struct RBT {
  int8_t color;
  uint16_t key;
  void *data;
  struct RBT *left;
  struct RBT *right;
  struct RBT *parent;
  struct RBT *next;
  struct RBT *prev;
};

typedef struct RBT RBT;

typedef struct {

  /** returns node with smallest key */
  RBT * (* findMin) (RBT *T);
  
  /** Creates an empty Red Black Tree */
  RBT * (* create) (void);
  
  /** Returns 0 if T is empty, 1 otherwise */
  int (* isEmpty) (RBT *T);
  
  /** Inserts a new node given the key. Duplicates are not allowed */
  RBT * (* insert) (RBT **T, uint16_t key, void *data);
  
  /** Convenient function for output the content of the tree  */
  void (* dump) (RBT *T, FILE *fp);
  
  /** Inserts all keys from F to T*/
  void (* join) (RBT **T, RBT *F);
  
  /** checks whether given key exists in Tree T */
  char (* containsKey) (RBT *T, uint16_t key);
  
  /** Cleanup */
  void (* destroy) (RBT **pT, void (* destroyfn) (void *data));

} rbt_lib;

extern rbt_lib const rbt;

#endif
