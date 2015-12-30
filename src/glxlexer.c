/*
 * Reads a file in chunks and place them into a buffer in order to keep the 
 * disk access low.
 *
 * @name: glxlexer.c
 * @author: extradiable
 * @date: sáb may  2 20:57:16 CDT 2015
 * @update: dom oct 25 16:55:09 CST 2015
 */

#include "glxconstants.h"
#include "glxlexer.h"

#define readEmptyLines() \
  while (ascii == '\n') {\
    ascii = readByte();\
    currentLine++;\
  }
/* Half of the buffer size */

#define HALF_BUFF_LEN 512

/* These constants are used to read the name of expressions */
#define NUM_MASK = 0x30
#define LCL_MASK = 0x60
#define UCL_MASK = 0x40

/* Max number of characters in expression name */
#define MAX_EXPNAME_LENGTH 80

int8_t currentLine = 1;

/* Counter of the number of expressions read so far  */
int8_t numExpressions = 0;

/* Flags whether a new expression name must be read  */
int8_t currentExpression = numExpression;

/* Pointer to regular expressions file. */
FILE *fp;

/* File is buffered here. */
uint8_t buffer[HALF_BUFF_LEN << 1];

/*
 * Alphabet is ascii and all symbols are allowed. There are however, symbols 
 * with a special meaning. In order to check if the current symbol has a
 * special meaning you can check in this data structure.
 */
RBNode *specials = NULL;

/* 
 * In this implementation I'm considering only ascii symbols, nevertheless only
 * a subset of them might be used for a particular automata.
 */
RBNode *symbols;


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

/* reads next character from file */
uint8_t readByte(void);

/* returns true if there are unread characters */
uint8_t hasNextByte(void);


void initLexer(uint8_t *name){
  symbols = createRBT();
  fp = fopen(name, "r");
  if(fp == NULL){
    fprintf(stderr, "Unable to open file: %s\n", name);
    exit(EXIT_FAILURE);
  }
  loadLexer();
  /*
  // CAT does not appear here because it does not have an operator symbol
  specials = createRBT();
  insertRBN(&specials, OR,       NULL); 
  insertRBN(&specials, STAR,     NULL); 
  insertRBN(&specials, PLUS,     NULL); 
  insertRBN(&specials, OPTIONAL, NULL); 
  //insertRBN(&specials, O_GROUP,  NULL); 
  insertRBN(&specials, C_GROUP,  NULL); 
  insertRBN(&specials, END,      NULL); 
  insertRBN(&specials, EOL,      NULL); 
  */
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

uint8_t hasNextByte(){
  if(pos > limit) {
    loadBuffer();
  }
  return pos <= limit;
}

uint8_t readByte(void) {
  return buffer[pos++];  
}

/*PTNode *getNextToken(void){
  uint8_t ascii = readByte();
  readEmptyLines()
  if (currentExpression == 0) {
    readExpressionName();
    consumeBlanks()
    if (ascii == '\n') {
      fprintf(stderr, "GLEX error: Empty expression. Line: %d.\n", currentLine);
      exit(EXIT_FAILURE);
    }
  }
  consumeBlanks()
  // TODO 
}
*/
void freeLexer(){
  fclose(fp);
}
