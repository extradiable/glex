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

/* Init glxlexer */
void initLexer(uint8_t *name);

/* Returns next token found */
PTNode *getNextToken(void);

/* Convenient method to free allocated memory used by glxlexer */
void freeLexer(void);

#endif
