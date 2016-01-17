/*
 * This is a header for glex.c
 *
 * @name: glex.h
 * @author: extradiable
 * @date: dom ene 17 09:31:13 CST 2016
 * @update: dom ene 17 09:31:23 CST 2016
 */

#ifndef GLEX_H
#define GLEX_H 1

/* This structure is used by main to communicate with parse_opt. */
typedef struct {
  char *input;      /* File to be processed */
  int verbose;      /* Flag -v */
  char *outfile;    /* Argument for -o */
} Config;

/* This is structure is shared across all files to log erros */
typedef struct {
 
  /* Log error messages */
  void (* show) (char *mod, char *fn,  char  *msg);

} err_lib;

extern err_lib const err;

#endif
