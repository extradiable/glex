/*
 * This file includes a series of unit tests for QUEUE component.
 * 
 * @name: test_glxqueue.c
 * @author: extradiable
 * @date: sáb dic 12 10:53:00 CST 2015
 * @update: sáb dic 12 10:53:11 CST 2015
 */

#include <stdio.h>
#include "../../../src/glxqueue.h"

int main(int argc, char **argv){
  Queue *Q = NULL;
  int i;
  /* Test 1 */
  printf("[*] Test: destroy a NULL Queue.\n");
  queue.destroy(&Q, &free);
  printf("[+] Ok.\n");
  /* Test 2 */
  printf("[*] Test Name: peek a NULL Queue.\n");
  queue.peek(Q);
  printf("[+] Ok.\n");
  /* Test 3 */
  printf("[*] Test: destroy a Queue with 1 element.\n");
  Q = queue.create();
  queue.enqueue(Q, malloc(sizeof(int)));
  queue.destroy(&Q, &free);
  printf("[+] Ok.\n");
  /* Test 4 */
  printf("[*] Test Name: try to destroy the Queue again.\n");
  queue.destroy(&Q, &free);
  printf("[+] Ok.\n");
  /* Test 5 */
  printf("[*] Test: Enqueue [0, 9] integers.\n");
  Q = queue.create();
  for(i = 0; i < 10; i++){
    int *p = (int *) malloc(sizeof(int));
    *p = i;
    queue.enqueue(Q, p);
  }
  printf("[+] Ok.\n");
  /* Test 6 */
  printf("[*] Test: retrieve size of Queue.\n");
  if (i == Q->size) {
    printf("[+] Ok.\n");
  } else {
    printf("[-] Fail.\n");
  }
  /* Test 7 */
  printf("[*] Test: peek the Queue.\n");
  if (0 == *((int *) queue.peek(Q))) {
    printf("[+] Ok.\n");
  } else {
    printf("[-] Fail.\n");
  }
  /* Test 8 */
  printf("[*] Test: dequeue all integers.\n");
  i = -1;
  while (Q->size) {
    int *p = (int *) queue.dequeue(Q);
    int n = *p;
    free(p);
    if (n != ++i)
      break;
  }
  if (Q->size == 0) {
    printf("[+] Ok.\n");
  } else {
    printf("[-] Fail.\n");
  }
  /* Test 9 */
  printf("[*] Test: Dequeue an empty Queue.\n");
  if (queue.dequeue(Q) == NULL) {
    printf("[+] Ok.\n");
  } else {
    printf("[-] Fail.\n");
  }
  /* Test 10 */
  printf("[*] Test: Peek an empty Queue.\n");
  if (queue.peek(Q) == NULL) {
    printf("[+] Ok.\n");
  } else {
    printf("[-] Fail.\n");
  }
  queue.destroy(&Q, &free);
  printf("[*] Unit test for glxqueue is done!\n");
  return 0;
}
