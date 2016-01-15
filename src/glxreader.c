/*
 * Reads a file in chunks and place them into a buffer in order to keep the 
 * disk access low.
 *
 * @name: glxlexer.c
 * @author: extradiable
 * @date: sáb may  2 20:57:16 CDT 2015
 * @update: dom oct 25 16:55:09 CST 2015
 */

#include <string.h>
#include "glxconstants.h"
#include "glxrbtree.h"
#include "glxreader.h"

//TODO: should this be defined here?
#define readEmptyLines() \
  while (ascii == '\n') {\
    ascii = readByte();\
    currentLine++;\
  }

/* Half of the buffer size */
#define HALF_BUFF_LEN 512

/* Max number of characters in expression name */
#define MAX_EXPNAME_LENGTH 80

// TODO: should this variable be here?
//int8_t currentLine = 1;

/* Pointer to regular expressions file. */
FILE *fp;

/* File is buffered here. */
uint8_t buffer[HALF_BUFF_LEN << 1];

/**
 * position points to the next unread byte in buffer.
 * 0 < position  < 2 * HALF_BUFF_LEN.
 * When position > limit, there are no more unread characters
 */
uint16_t pos = 0;

/* position <= limit as long as there are unread bytes. */
uint16_t limit = 0;

/* Reads from file and stores in Buffer*/
void loadBuffer(void);

static void initLexer(uint8_t *name){
  fp = fopen(name, "r");
  if(fp == NULL){
    char message[] = "Unable to open file: ";
    error("glxreader", "init", strcat(message, name));
  }
  #ifdef DEBUG_H
  printf("File opened: %s\n", name);
  #endif
}

void loadBuffer(void){
  pos = pos % (2 * HALF_BUFF_LEN);
  limit = fread(
    &buffer[pos], 
    sizeof(uint8_t), 
    HALF_BUFF_LEN, 
    fp
  );
  limit += pos;
  pos = pos + (pos == limit);
}

static uint8_t hasNextByte(){
  return pos < limit;
}

static uint8_t readByte(void) {
  if(pos > limit) {
    loadBuffer();
  }
  return buffer[pos++];  
}

static void freeLexer(){
  fclose(fp);
}

reader_lib const reader = { 
  initLexer, 
  readByte,
  hasNextByte,
  freeLexer
};
