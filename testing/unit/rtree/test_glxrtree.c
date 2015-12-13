#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "../../../src/glxrbtree.h"
#include "../../../src/glxrtree.h"

int main(int argc, char **argv){
  RTree *RT = NULL;
  int i;
  /* Test 1 */
  printf("[*] Test Name: destroy a NULL Recursive Tree.\n");
  rtree.destroy(&RT, &free);
  printf("[+] Ok.\n");
  /* Test 2 */
  printf("[*] Unit test for glxrtree is done!\n");
  return 0;
}
