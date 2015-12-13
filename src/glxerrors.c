#include <stdio.h>
#include "glxerrors.h"

void error(char *mod, char *fn, char *msg) {
  fprintf(
    #ifndef DEBUG_H
    stderr,
    #else
    stdout, 
    #endif
    "[error]: Source: %s:%s Message: %s.\n", mod, fn, msg
  );
  #ifndef DEBUG_H
  exit(EXIT_FAILURE);
  #endif
}
