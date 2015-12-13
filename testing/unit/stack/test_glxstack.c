/*
 * This file includes a series of unit tests for STACK component.
 * 
 * @name: test_glxstack.c
 * @author: extradiable
 * @date: mar may 12 20:49:30 CDT 2015
 * @update: sáb dic 12 09:57:07 CST 2015
 */

#include <stdio.h>
#include "../../../src/glxstack.h"

int main(int argc, char **argv){
  Stack *S = NULL;
  int i;
  /* Test 1 */
  printf("[*] Test Name: destroy a NULL stack.\n");
  stack.destroy(&S, &free);
  printf("[+] Ok.\n");
  /* Test 2 */
  printf("[*] Test Name: peek NULL stack.\n");
  stack.peek(S);
  printf("[+] Ok.\n");
  /* Test 3 */
  printf("[*] Test Name: destroy a stack with 1 element.\n");
  S = stack.create();
  stack.push(S, malloc(sizeof(int)));
  stack.destroy(&S, &free);
  printf("[+] Ok.\n");
  /* Test 4 */
  printf("[*] Test Name: try to destroy the stack again.\n");
  stack.destroy(&S, &free);
  printf("[+] Ok.\n");
  /* Test 5 */
  printf("[*] Test Name: Push [0, 9] integers.\n");
  S = stack.create();
  for(i = 0; i < 10; i++){
    int *p = (int *) malloc(sizeof(int));
    *p = i;
    stack.push(S, p);
  }
  printf("[+] Ok.\n");
  /* Test 6 */
  printf("[*] Test Name: retrieve size of stack.\n");
  if (i == S->size) {
    printf("[+] Ok.\n");
  } else {
    printf("[-] Fail.\n");
  }
  /* Test 7 */
  printf("[*] Test Name: peek the stack.\n");
  if ((i-1) == *((int *) stack.peek(S))) {
    printf("[+] Ok.\n");
  } else {
    printf("[-] Fail.\n");
  }
  /* Test 8 */
  printf("[*] Test Name: pop all integers.\n");
  while (S->size) {
    int *p = (int *) stack.pop(S);
    int n = *p;
    free(p);
    if (n != --i)
      break;
  }
  if (S->size == 0) {
    printf("[+] Ok.\n");
  } else {
    printf("[-] Fail.\n");
  }
  /* Test 9 */
  printf("[*] Test Name: Pop an empty stack.\n");
  if (stack.pop(S) == NULL) {
    printf("[+] Ok.\n");
  } else {
    printf("[-] Fail.\n");
  }
  /* Test 10 */
  printf("[*] Test Name: Peek an empty stack.\n");
  if (stack.peek(S) == NULL) {
    printf("[+] Ok.\n");
  } else {
    printf("[-] Fail.\n");
  }
  stack.destroy(&S, &free);
  printf("[*] Unit test for glxstack is done!\n");
  return 0;
}
