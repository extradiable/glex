/*
 * This file includes a series of unit tests for READER component.
 * 
 * @name: test_glxreader.c
 * @author: extradiable
 * @date: jue ene 14 07:52:12 CST 2016
 * @update: jue ene 14 07:52:26 CST 2016
 */

#include <stdio.h>
#include "../../../src/glxreader.h"

int main(int argc, char **argv){
  int i;
  /* Test 1 */
  printf("[*] Test Name: check hasNextByte when no file is loaded.\n");
  printf("hasNextByte(): %d\n", reader.hasNextByte());
  printf("[+] Ok.\n");
  /* Test 2 */
  printf("[*] Test Name: check readByte when no file is loaded.\n");
  printf("readByte(): %c\n", reader.readByte());
  printf("[+] Ok.\n");
  /* Test 3 */
  printf("[*] Test Name: check hasNextByte in an empty file.\n");
  FILE *fp = fopen("empty.dat", "w+");
  fclose(fp);
  reader.init("empty.dat");
  printf("hasNextByte(): %d\n", reader.hasNextByte());
  printf("[+] Ok.\n");
  /* Test 4 */
  printf("[*] Test Name: check readByte in an empty file.\n");
  printf("readByte(): %c\n", reader.readByte());
  printf("[+] Ok.\n");
  /* Test 5 */
  printf("[*] Test Name: free reader.\n");
  reader.free();
  printf("[+] Ok.\n");
  /* Test 6 */
  printf("[*] Test Name: read all bytes in a non empty file.\n");
  fp = fopen("digits.dat", "w+");
  fprintf(fp, "0123456789");
  fclose(fp);
  reader.init("digits.dat");
  for(i=0; i<10; i++){
    printf("%c\n", reader.readByte());
  }
  printf("[+] Ok.\n");
  /* Test 7 */
  printf("[*] Test Name: check hasNextByte when all bytes have been read.\n");
  printf("hasNextByte(): %d\n", reader.hasNextByte());
  printf("[+] Ok.\n");
  /* Test 8 */
  printf("[*] Unit test for glxstack is done!\n");
  return 0;
}
