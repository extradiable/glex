#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "glxrbtree.h"
#include "glxnestedrbtree.h"

int main(int argc, char **argv){
  srand(time(NULL));
  int i, j, r, n = 5, m = 3;
  printf("[*] Starts unit test for glxstack\n\n");
  printf("[*] Creating array of R.B. trees\n\n");
  RBNode **trees = (RBNode **) malloc(n * sizeof(RBNode));
  for(i = 0; i < n; i++) {
    *(trees + i) = createRBT();
  }
  printf("[*] Initializing with random integers\n");
  for(i = 0; i < n; i++) {
    for(j = 0; j < m; j++) {
      r = rand() % 20;
      printf("[*] insertRBN(trees[%d], %d, NULL);\n", i, r);
      insertRBN(trees + i, r, NULL);
    }
    printf("\n");
  }
  r = rand() % n;
  printf("[*] Printing elements from tree[%d]\n", r);
  RBNode *N = *(trees + r);
  RBNode *next = N;
  do {
    printf("[*] value: %d\n", next->key);
    next = next->next;
  } while(next != N);
  printf("[*] Printing first 3 elements[%d]\n", r);
  printf("[*] N [%d]\n", N->key);
  printf("[*] N(left) [%d]\n", N->left->key);
  printf("[*] N(right) [%d]\n", N->right->key);
  printf("[*] Starts unit test for glxstack\n");
  printf("\n[*] Preparing tree for nested test\n");
  RBNode *nested = createRBT();
  printf("[*] insertNestedRBT(nested, trees[0])\n");
  insertNestedRBT(&nested, *(trees + 0));
  printf("[*] getNumStates(): %d\n", getNumStates());
  printf("[*] insertNestedRBT(nested, trees[1])\n");
  insertNestedRBT(&nested, *(trees + 1));
  printf("[*] getNumStates(): %d\n", getNumStates());
  insertNestedRBT(&nested, *(trees + 0));
  printf("[*] insertNestedRBT(nested, trees[0])\n");
  printf("[*] getNumStates(): %d\n", getNumStates());
  printf("[*] Clean up\n");
  for(i = 0; i < n; i++) {
    freeRBT(*(trees + i));
  }
  printf("[*] Unit test for glcstack is done!\n");
  free(trees);
  return 0;
}
