/* 
 * This is a priority queue implemented as a Red Black Tree.
 *  
 * Notes:
 *  - Duplicates are not allowed.
 *  - It is possible to traverse all the nodes by following next/prev pointers.
 *    If you start the walk using node with smallest key and following next
 *    pointers, then nodes are listed in ascendent order. Likewise, it is
 *    posible list them in descendent order.
 *
 * @name: glxrbtree.c
 * @purpose: Really useful implementation of a Red Black Tree
 * @author: extradiable
 * @references:
 *   Introduction to Algorithms (Cormen, Leiserson, Rivest, Stein)
 * @date: sáb may  2 19:16:21 CDT 2015
 * @updated: dom oct 25 16:57:42 CST 2015
 */

#include "glxrbtree.h"
#define RED 1
#define BLACK 0

static RBT sentinel;

static int isEmpty(RBT *T){
  if (T != NULL) {
    return T == &sentinel;
  } else {
    return 1;
  }  
} 

static RBT *create(void){
  return &sentinel;
}

static RBT *createNode(uint16_t key, void *data){
  RBT *N = (RBT *) malloc(sizeof(RBT));
  if(N){
    N->color = RED;
    N->key = key;
    N->data = data;
    N->left = N->right = create();
    N->next = N->prev = N;
  }else{
    err.show("glxrbtree", "createNode", "Insufficient memory to create R.B. Node");
  }
  return N;
}

static void *left_rotate(RBT **T, RBT *x){
  RBT *y = x->right;
  x->right = y->left;
  y->left->parent = x;
  y->parent = x->parent;
  if(x->parent == &sentinel){
    *T = y;
  }else if(x == x->parent->left){
    x->parent->left = y;
  }else{
    x->parent->right = y;
  }
  y->left = x;
  x->parent = y;
}

static void *right_rotate(RBT **T, RBT *y){
  RBT *x = y->left;
  y->left = x->right;
  x->right->parent = y;
  x->parent = y->parent;
  if(y->parent == &sentinel){
    *T = x;
  }else if(y == y->parent->left){
    y->parent->left = x;
  }else{
    y->parent->right = x;
  }
  x->right = y;
  y->parent = x;
}

static void fix_linked_list(RBT **T, RBT *z){
  if(z != *T){
    if(z->parent->left == z){
      z->parent->prev->next = z;
      z->prev = z->parent->prev;
      z->next = z->parent;
      z->parent->prev = z;
    }else{
      z->next = z->parent->next;
      z->parent->next->prev = z;
      z->prev = z->parent;
      z->parent->next = z;
    }
  }
}

static void insert_fixup(RBT **T, RBT *z){ 
  while(z->parent->color == RED){
    if(z->parent == z->parent->parent->left){
      RBT *y = z->parent->parent->right;
      if(y->color == RED){
        z->parent->color = BLACK;
        y->color = BLACK;
        z->parent->parent->color = RED;
        z = z->parent->parent;
      }else{
        if(z == z->parent->right){
          z = z->parent;
          left_rotate(T, z);
        }
        z->parent->color = BLACK;
        z->parent->parent->color = RED;
        right_rotate(T, z->parent->parent);
      }
    }else{
      RBT *y = z->parent->parent->left;
      if(y->color == RED){
        z->parent->color = BLACK;
        y->color = BLACK;
        z->parent->parent->color = RED;
        z = z->parent->parent;
      }else{
        if(z == z->parent->left){
          z = z->parent;
          right_rotate(T, z); 
        }
        z->parent->color = BLACK;
        z->parent->parent->color = RED;
        left_rotate(T, z->parent->parent);
      }
    }
  }
  (*T)->color = BLACK;
}

static RBT *insert(RBT **T, uint16_t key, void *data){
  RBT *y = &sentinel; 
  RBT *x = *T;
  while(x != &sentinel){
    y = x;
    if(key < x->key){
      x = x->left;
    }else if(key > x->key){
      x = x->right;
    }else{
      //duplicates are not admited
      return x;
    }
  }
  RBT *z = createNode(key, data);
  z->parent = y;
  if(y == &sentinel){
    *T = z;
  }else if(key < y->key){
    y->left = z;
  }else{
    y->right = z;
  }

  fix_linked_list(T, z);
  insert_fixup(T, z);
  return z;
};

/**
 * This function inserts all keys whithin F into T
 */
static void join(RBT **T, RBT *F){
  if(T != NULL && *T != NULL) {
    if (F != &sentinel) {
      RBT *N = F;
      do {
        insert(T, N->key, N->data);
        N = N->next;
      } while(N != F);
    }
  } else {
    err.show("glxrbtree", "join", "An attempt was made to use NULL RBT");
  }
}

/*
 * A convenient function for output the content of the tree
 */
static void dump(RBT *T, FILE *fp) {
  if (!isEmpty(T)) {
    dump(T->left, fp);
    fprintf(fp, "(key: %d, value: %p) ", T->key, T->data);
    dump(T->right, fp);
  }
}

static void _dump(RBT *T, FILE *fp) {
  if (fp != NULL) {
    if (isEmpty(T)) {
      fprintf(fp, "EMPTY RBT\n");
    } else {
      dump(T, fp);
      fprintf(fp, "\n");
    } 
  } else {
    err.show("glxrbtree", "dump", "An attempt was made to write in a NULL file pointer");
  }
}

static char containsKey(RBT *T, uint16_t key){
  if (T != NULL) {
    if (T!=&sentinel) {
      if (key > T->key) {
        return containsKey(T->right, key);
      } else if (key < T->key) {
        return containsKey(T->left, key);
      } else {
        return T->key == key;
      }
    }
    return 0;
  } else {
    err.show("glxrbtree", "containsKey", "An attempt was made to search in a NULL RBT");
  }
}

static RBT *get(RBT *T, uint16_t key){
  if (T != NULL) {
    if (T!=&sentinel) {
      if (key == T->key) {
        return T;
      } else if (key > T->key) {
        return get(T->right, key);
      } else {
        return get(T->left, key);
      }
    }
    return NULL;
  } else {
    err.show("glxrbtree", "get", "An attempt was made to search in a NULL RBT");
  }
}

/*
 * A convenient function for clean up
 */
static void destroy(RBT **pT, void (* destroyfn) (void *data)) {
  if (pT != NULL && !isEmpty(*pT)) {
    RBT *T = *pT;
    destroy(&T->left, destroyfn);
    destroy(&T->right, destroyfn);
    if (destroyfn) {
      destroyfn(T->data);
    }
    free(T);
    *pT = NULL;
  }
}

static RBT *findMin(RBT *T){
  if (T != NULL) {
    while(1) {
      if (isEmpty(T->left)) {
        return T;
      } else { 
        T = T->left;
      }
    } 
  } else {
    err.show("glxrbtree", "findMin", "An attempt was made to search in a NULL RBT");
  }
}

rbt_lib const rbt = { 
  findMin, 
  create,
  isEmpty,
  insert,
  _dump,
  join,
  containsKey,
  get,
  destroy 
};
