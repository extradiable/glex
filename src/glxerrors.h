#ifndef ERROR_H
#define ERROR_H 1

typedef struct {
 
  /* Log error messages */
  void (* show) (char *mod, char *fn,  char  *msg);

} err_lib;

extern err_lib const err;

#endif
