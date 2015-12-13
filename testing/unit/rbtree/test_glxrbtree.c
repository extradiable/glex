/*
 * This file includes a series of unit tests for RBTREE component.
 * 
 * @name: test_glxrbtree.c
 * @author: extradiable
 * @date: mar may 12 20:49:30 CDT 2015
 * @update: sáb dic 12 19:16:38 CST 2015
 */

#include <stdio.h>
#include "../../../src/glxrbtree.h"

int main(int argc, char **argv){
  RBT *T = NULL;
  int i;
  /* Test 1 */
  printf("[*] Test: destroy a NULL red black tree.\n");
  rbt.destroy(&T, &free);
  printf("[+] Ok.\n");
  /* Test 2 */
  printf("[*] Test: search key in NULL red black tree.\n");
  rbt.containsKey(T, 10);
  printf("[+] Ok.\n");
  /* Test 3 */
  printf("[*] Test: find min-key element in NULL red black tree.\n");
  rbt.findMin(T);
  printf("[+] Ok.\n");
  /* Test 4 */
  printf("[*] Test: dump NULL red black tree with NULL file pointer.\n");
  rbt.dump(T, NULL);
  printf("[+] Ok.\n");
  /* Test 5 */
  printf("[*] Test: dump NULL red black tree to stdout.\n");
  rbt.dump(T, stdout);
  printf("\n[+] Ok.\n");
  /* Test 6 */
  printf("[*] Test: join two NULL red black trees.\n");
  rbt.join(&T, NULL);
  printf("[+] Ok.\n");
  return 0;
}
