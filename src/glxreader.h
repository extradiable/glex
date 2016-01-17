/*
 * This is the header file for glxreader.c
 *
 * @name: glxlexer.h
 * @author: extradiable
 * @date: sáb may  2 20:57:16 CDT 2015
 * @update: dom oct 25 16:49:48 CST 2015
 */

#ifndef GLX_READER_H
#define GLX_READER_H
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
 
  /* Initialize the Reader */
  void (* init) (uint8_t *name);
 
  /* Read next byte from file */
  uint8_t (* readByte) (void);

  /* Returns true if EOF has not been reached */
  uint8_t (* hasNextByte) (void);

  /* Returns current line counter */
  int8_t (* getLine) (void);

  /* Free all resources from this reader */
  void (* free) (void);

} reader_lib;

extern reader_lib const reader;

#endif
