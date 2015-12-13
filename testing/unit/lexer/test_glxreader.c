#include <stdio.h>
#include "glxreader.h"

int main(int argc, char **argv){
  char c;
  glx_loadFile(argv[1]);
  while((c = glx_consume()) != '\0')
    printf("read: %c\n", c);
  glx_freeBuffer();
  system("pause");
  return 0;
}