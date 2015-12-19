#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "../../../src/glxrbtree.h"
#include "../../../src/glxrtree.h"

int main(int argc, char **argv){
  RTree *RT = NULL;
  RTree *RTC = NULL;
  int i;
  int *value;
  /* Test 1 */
  printf("[*] Test: destroy a NULL Recursive Tree.\n");
  rtree.destroy(&RT, &free);
  printf("[+] Ok.\n");
  /* Test 2 */
  printf("[*] Test: branch a NULL Recursive Tree.\n");
  rtree.getBranch(RT, 1);
  printf("[+] Ok.\n");
  /* Test 3 */
  printf("[*] Test: create recursive tree.\n");
  RT = rtree.create();
  printf("[+] Ok.\n");
  /* Test 4 */
  printf("[*] Test: add branches.\n");
  RTC = rtree.getBranch(RT, 'h');
  RTC = rtree.getBranch(RTC, 'e');
  RTC = rtree.getBranch(RTC, 'l');
  printf("[+] Ok.\n");
  /* Test 5 */
  printf("[*] Test: set value.\n");
  value = (int *) malloc(sizeof(int));
  *value = 3;
  rtree.setValue(RTC, value);
  printf("[+] Ok.\n");
  /* Test 7 */
  printf("[*] Test: add branches.\n");
  RTC = rtree.getBranch(RTC, 'l');
  RTC = rtree.getBranch(RTC, 'o');
  printf("[+] Ok.\n");
  /* Test 8 */
  printf("[*] Test: destroy Recursive Tree.\n");
  rtree.destroy(&RT, &free);
  printf("[+] Ok.\n");
  printf("[*] Unit test for glxrtree is done!\n");
  return 0;
}
