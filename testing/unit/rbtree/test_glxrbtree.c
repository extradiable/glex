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
  RBT *T2 = NULL;
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
  printf("[+] Ok.\n");
  /* Test 6 */
  printf("[*] Test: join two NULL red black trees.\n");
  rbt.join(&T, NULL);
  printf("[+] Ok.\n");
  /* Test 7 */
  printf("[*] Test: create red black tree.\n");
  T = rbt.create();
  printf("[+] Ok.\n");
  /* Test 8 */
  printf("[*] Test: insert integers [1-2] with NULL values.\n");
  for(i = 1; i <= 2; i++) {
    rbt.insert(&T, i, NULL);
  }
  printf("[+] Ok.\n");
  /* Test 9 */
  printf("[*] Test: dump red black tree to stdout.\n");
  rbt.dump(T, stdout);
  printf("[+] Ok.\n");
  /* Test 10 */
  printf("[*] Test: join two trees.\n");
  T2 = rbt.create();
  for(i = 1; i <= 4; i++) {
    rbt.insert(&T2, i, NULL);
  }
  rbt.join(&T, T2);
  rbt.dump(T, stdout);
  printf("[+] Ok.\n");
  /* Test 11 */
  printf("[*] Test: destroy red black tree.\n");
  rbt.destroy(&T2, NULL);
  printf("[+] Ok.\n");
  /* Test 12 */
  printf("[*] Test: find key min.\n");
  T2 = rbt.findMin(T);
  printf("Min: %d.\n", T2->key);
  printf("[+] Ok.\n");
  /* Test 13 */
  printf("[*] Test: search unexistent key in red black tree.\n");
  printf("%d\n", rbt.containsKey(T, 100));
  printf("[+] Ok.\n");
  /* Test 14 */
  printf("[*] Test: search key in red black tree.\n");
  printf("%d\n", rbt.containsKey(T, 4));
  printf("[+] Ok.\n");
  rbt.destroy(&T, NULL);
  printf("[*] Unit test for glxrbtree is done!\n");
  return 0;
}
