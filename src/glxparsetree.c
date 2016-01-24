/*
 * Reading and parsing of regular expression is done here. As a result a FDA is
 * constructed. The hole file is interpreted as a regular expression in which 
 * each line is joined as an or expression. 
 *
 * @name: glxparsetree.c
 * @references: 
 *   Compilers Priciples, Techniques, and Tools
 * @purpose: construction of FDA
 * @author: extradiable
 * @date: sáb may  2 21:11:01 CDT 2015
 * @update: dom oct 25 17:21:56 CST 2015
 */

/** SECTION: HEADER FILES */

#include <string.h>
#include <stdio.h>
#include "glxreader.h"
#include "glxrtree.h"
#include "glxstack.h"
#include "glxptnode.h"
#include "glxparsetree.h"

/** SECTION: DEFINE */

#define consumeLineSpaces() \
  while (ascii == ' ' || ascii == '\t') {\
    ascii = reader.readByte();\
  }

#define consumeAllSpaces() \
  while(reader.hasNextByte()) {\
    ascii = reader.readByte();\
    if(ascii != '\n' || ascii != '\t' || ascii != ' ')\
      break;\
  }

/* Max number of characters in expression name */
#define MAX_EXPNAME_LENGTH 80

#define READ_EXPRNAME_ERR_1 0
#define READ_EXPRNAME_ERR_2 1
#define READ_EXPRNAME_ERR_3 2
#define OPEN_LOG_ERR        3
#define READ_EXPR_ERR_1     4

// Special tokens
#define AUGMENTED   '#'
#define SYM         'S'

// Tokens for specials
#define BRANCH     '|'
#define CONCAT     '.'
#define ATOM       'a'
#define STAR       '*'
#define PLUS       '+'
#define OPTIONAL   '?'
#define START_EXPR '('
#define END_EXPR    ')'
#define END        '\0'
#define EOL        '\n'

/** SECTION: GLOBALS */

typedef struct {
  char min;
  char max;
} Range;

/* Parse tree */
Stack *parseTree = NULL;

/* Current character being analized */
uint8_t ascii = '\0';

/* Counter of the number of expressions read so far */
int8_t numExpressions = 0;

/* Flags whether a new expression name must be read */
int8_t currentExpression = 0;

/* This pointer is maintained for logging purposes. */
FILE *fpLog = NULL;

uint8_t *currentName = NULL;

/*
 * Alphabet is ascii and all symbols are allowed, there are however, symbols 
 * with an special meaning. In order to check if the current symbol has an
 * special meaning you can check in this data structure.
 */
RBT *specials = NULL;

/* Current symbol read from file. */
uint8_t current;

/* Counter for the number of positions in the syntax tree. */
int16_t numPositions = 0;

/* 
 * In this implementation I'm considering only ascii symbols, nevertheless only
 * a subset of them might be used for a particular automata.
 */
RBT *symbols;

/*
 * Function followpos, it is used so frequently that I decided to make it global.
 * Given a position p, there is a set of positions Q such that followpos(p) = Q.
 * Fore more details on this fuction please refer to "Compilers: Principles, 
 * Techniques & Tools.
 */
RBT **followpos = NULL;

/* Automata and auxiliary information will be constructed here */
ParseInfo *info;

/* positionToSymbol[i] is the alphabet symbol corresponding to position i. */
uint8_t *positionToSymbol = NULL;

/* 
 * Non blank input lines should contain the following format:
 * <name of token> <separator> <regular expression>
 * where
 * <name of token>: is any word in the following language: [a-zA-Z0-9]{1,80}
 * <separator>: is the character ':'
 * <regular expression>: any non empty string representing a regular expression
 * Every name is recorded in a recursive tree that is used kind of a hash and
 * assigned a unique numeric identifier.
 */
RTree *dictionary;

/* High order nibbles of letters and digits */
RBT *ranges = NULL;

/** SECTION: FUNCTION DEFINITIONS */

/* Every non empty line should be of the form <name>:<expression> */
void readExpressionName(void);

/* Initialization */
void init(Config *);

/* Syntax tree */

PTNode *token(int8_t);
void concat(void);
void branch(void);
void clousure(void);
void atom(void);

/** SECTION: LOGIC */

PTNode *parse(uint8_t *fileName) {
  parseTree = stack.create();
  reader.init(fileName);
  do {
    consumeAllSpaces()
    readExpressionName();
    ascii = reader.readByte();
    consumeLineSpaces();
    branch();
    while(reader.hasNextByte()) {
      ascii = reader.readByte();      
      if(ascii == '\n')
        break;
    }
    consumeAllSpaces()
  } while(reader.hasNextByte());

  //TODO: fix this function by adding the call to re()
  /*
  while((current = reader.readByte()) != '\0') {
    if(!containsKeyRBT(whitespaces, current)) {
      if(T) {
        T = join(T, &re, BRANCH);
      }else{
        T = re();
      }
    }
  }
  // we are done with the reader
  freeBuffer();
  if(current == '\0') {
    PTNode *R =  createPTNode(CAT, CAT, 0);
    R->left = T;
    R->right = createPTNode(AUGMENTED, AUGMENTED, 0);
    T = R;
    return T;
  }else{ // an error has occurred
    freePT(T);
    fprintf(stderr, "Bad input format.");
    exit(EXIT_FAILURE);   
  }
  */
}

int8_t valid() {
  uint8_t hnibble = ascii & 0xF0;
  uint8_t lnibble = ascii & 0x0F;
  RBT *T = rbt.get(ranges, hnibble);
  if (T != NULL) {
    Range *r = T->data;
    if (lnibble >= r->min && lnibble <= r->max) {
      return 1;
    } else {
      return ascii == '_';
    }  
  }
  return 0;
}

void showError(int8_t code) {
  char msg[50 + MAX_EXPNAME_LENGTH];
  int8_t line = reader.getLine();
  switch(code) {
    case READ_EXPRNAME_ERR_1:
      sprintf(msg, "Expected ':' but found '%c'. Line: %d", ascii, line);
      err.show("glxparser", "readExpressionName", msg);
      break;
    case READ_EXPRNAME_ERR_2:
      sprintf(msg, "No expression name was given. Line: %d", line);
      err.show("glxparser", "readExpressionName", msg);
      break;
    case READ_EXPRNAME_ERR_3:
      sprintf(msg, "Duplicated name found '%s'. Line: %d", currentName, line);
      err.show("glxparser", "readExpressionName", msg);
      break;
    case OPEN_LOG_ERR:
      sprintf(msg, "Unable to open file: glex.log");
      err.show("glxparser", "init", msg);
      break;
    case READ_EXPR_ERR_1:
      sprintf(msg, "Expected ')' but found '%c'. Line: %d", ascii, line);
      err.show("glxparser", "atom", msg);
      break;
  }
}

void readExpressionName() {
  RTree *branch = dictionary;
  ascii = reader.readByte();
  consumeLineSpaces();
  uint8_t i;
  for(i = 0; i < MAX_EXPNAME_LENGTH; i++) {
    if(!valid(ascii))
       break;
    currentName[i] = ascii;
    branch =  rtree.getBranch(branch, ascii);
    ascii = reader.readByte();
  }
  currentName[i] = '\0'; 
  consumeLineSpaces()
  if (ascii != ':') {
    showError(READ_EXPRNAME_ERR_1);
  } else if (i == 0) {
    showError(READ_EXPRNAME_ERR_2);
  } else if (branch->value != NULL) {
    showError(READ_EXPRNAME_ERR_3);
  } else {
    char cpy[i];
    strcpy(cpy, currentName);
    uint8_t *value = (uint8_t *) malloc(sizeof(uint8_t));
    currentExpression = ++numExpressions;
    *value = currentExpression;
    rtree.setValue(branch, value);
  }
}

void init(Config *config) {
  currentName = malloc((MAX_EXPNAME_LENGTH + 1) * sizeof(uint8_t));
  ranges = rbt.create();
  // number ranges
  Range *r = (Range *) malloc(sizeof(Range));
  r->min = 0x00;
  r->max = 0x09;
  rbt.insert(&ranges, 0x30, r);
  // upper case letters ranges
  r = (Range *) malloc(sizeof(Range));
  r->min = 0x01;
  r->max = 0x0F;
  rbt.insert(&ranges, 0x40, r);
  r = (Range *) malloc(sizeof(Range));
  r->min = 0x00;
  r->max = 0x0A;
  rbt.insert(&ranges, 0x50, r);
  // lower case letters ranges
  r = (Range *) malloc(sizeof(Range));
  r->min = 0x01;
  r->max = 0x0F;
  rbt.insert(&ranges, 0x60, r);
  r = (Range *) malloc(sizeof(Range));
  r->min = 0x00;
  r->max = 0x0A;
  rbt.insert(&ranges, 0x70, r);
  dictionary = rtree.create();
  if(config->verbose) { 
    fpLog = fopen("glex.log", "w");
    if(!fpLog) {
      showError(OPEN_LOG_ERR);
    } 
  }
  specials = rbt.create();
  rbt.insert(&specials, BRANCH,   NULL); 
  rbt.insert(&specials, OPTIONAL, NULL); 
  rbt.insert(&specials, STAR,     NULL); 
  rbt.insert(&specials, END_EXPR, NULL); 
  rbt.insert(&specials, EOL, NULL); 
  /*
  // CAT does not appear here because it does not have an operator symbol
  rbt.insert&specials, PLUS,     NULL); 
  //rbt.insert(&specials, START_EXPR,  NULL); 
  rbt.insert&specials, END,      NULL); 
  rbt.insert(&specials, EOL,      NULL); 
  symbols = rbt.create();

  info = (ParseInfo *) malloc(sizeof(ParseInfo));
  if(info) {
    info->numStates = 0;
    info->numSymbols = 0;
    info->finalState = 0;
    info->alphabet = NULL;
    info->transitions = createStack();
  }else{
    fprintf(stderr, "Insufficient memory to create ParseInfo.");
    exit(EXIT_FAILURE);
  }*/
}







































/* Logging */

void dumpFP(RBT **followpos, uint8_t *positionToSymbol, int16_t size);


/* DFA Generation */

void followPos(PTNode *T, RBT **followpos, int16_t *leaf);
void preprocessSyntaxTree(PTNode *T);
void computeAuxiliaryFunctions(PTNode *T);
void computeFollowPos(PTNode *T);
void generateStates(RBT *firstState);
Transition *createTransition(int16_t currentState, uint8_t symbol, int16_t nextState);

/* 
 * This function directs the flow from input file to DFA. If you want to know
 * the flow of this file, this is the function you want to take a look.
 */
void makeAutomaton(Config *config);





/** SECTION: CONSTANTS */

PTNode *token(int8_t type) {
  PTNode *N = ptn.create(type, ascii, 0);
  switch(type){
    case CONCAT:
      N->lexeme = '.';
      break;
    case BRANCH:
      N->lexeme = '|';
      break;
  }
  return N;
}

void branch() {
  concat();
  while(ascii == BRANCH) {
    stack.push(parseTree, token(BRANCH));
    concat();
  }
}

void concat() {
  clousure();
  while(!rbt.containsKey(specials, current)) { // no special meaning for current
    stack.push(parseTree, token(CONCAT));
    exit(1); //TODO: remove this
    clousure();
  }
}

void clousure() {
  atom();
  while(ascii == STAR || ascii == PLUS || ascii == OPTIONAL) { //TODO: check optional case
    stack.push(parseTree, token(ascii));
  }
}

void atom() {
  if(ascii == START_EXPR) { 
    ascii = reader.readByte();
    branch();
    if(ascii != END_EXPR) {
      showError(READ_EXPR_ERR_1);
    }
    ascii = reader.readByte();
  }else{
    //handle scape
    if(ascii == '\\') {
    
    }
    stack.push(parseTree, token(ATOM));
    stack.print(stdout, parseTree, (void (*) (FILE *, void *)) ptn.print);
    ascii = reader.readByte();
  }
}

PTNode *createSymbol(void) {
  /*uint8_t *s = malloc(sizeof(uint8_t));
  *s = 0; // try to insert symbol for first time
  if(s != NULL) {
    RBT *N = rbt.insert(&symbols, current, s); 
    if(s == N->data) { // new symbol
      *s = info->numSymbols++;
    }
    s = N->data; // retrieve the actual state of the symbol
    // c is mapping from original alphabet to subset
    return createPTNode(SYM, current, *s); 
  }else{
    fprintf(stderr, "Insufficient memory to create symbol.");
    exit(EXIT_FAILURE);
  }*/
}

void preprocessSyntaxTree(PTNode *T) {
  //TODO: ptnode is no longer a tree but a stack of nodes
  /*PTNode *L = T->left;
  PTNode *R = T->right;
  switch(T->token) {
    case AUGMENTED:
    case SYM:
      T->leaf = numPositions++;
      if(T->lexeme == '\0') { // Probably need to change this
        T->nullable = 1;
      }else{
        T->nullable = 0;
        rbt.insert(&T->firstpos, numPositions, NULL);
        rbt.insert(&T->lastpos, numPositions, NULL);
      }
      break;
    case OR:
      T->nullable = L->nullable || R->nullable;
      joinRBT(&T->firstpos, L->firstpos);
      joinRBT(&T->firstpos, R->firstpos);
      joinRBT(&T->lastpos, L->lastpos);
      joinRBT(&T->lastpos, R->lastpos);
      break;
    case CAT:
      T->nullable = L->nullable && R->nullable;
      if(L->nullable) {
        joinRBT(&T->firstpos, L->firstpos);
        joinRBT(&T->firstpos, R->firstpos);
      }else{
        joinRBT(&T->firstpos, L->firstpos);
      }
      if(R->nullable) {
        joinRBT(&T->lastpos, L->lastpos);
        joinRBT(&T->lastpos, R->lastpos);
      }else{
        joinRBT(&T->lastpos, R->lastpos);
      }
      break;
    case STAR:
      T->nullable = 1;
      joinRBT(&T->firstpos, L->firstpos);
      joinRBT(&T->lastpos, L->lastpos);
      break;
    case PLUS:
      T->nullable = 0;
      joinRBT(&T->firstpos, L->firstpos);
      joinRBT(&T->lastpos, R->lastpos);
  }*/
}

void computeAuxiliaryFunctions(PTNode *T) {
  // at this point symbols contains the list of all necessary symbols for FDA
  // lets create the alphabet for ParseInfo Structure
  info->alphabet  = (uint8_t *) malloc(info->numSymbols * sizeof(uint8_t));
  RBT *next = symbols;
  do {
    uint8_t *c = next->data;
    info->alphabet[*c] = next->key;
    next = next->next;
  } while(next != symbols); 
  rbt.destroy(&symbols, NULL); // we dont need this anymore
  //TODO: ptnode is no longer a tree but a stack of nodes postordenPT(T, &preprocessSyntaxTree);
  followpos = (RBT **) malloc(numPositions * sizeof(RBT));
  // I expect less positions than symbols.. char will be fine?
  positionToSymbol = (uint8_t *) malloc(numPositions * sizeof(uint8_t)); 
}

void computeFollowPos(PTNode *T) {
  //TODO: ptnode is no longer a tree but a stack of nodes
  /*
  // Take advantage of the walk to initialize the array with the leafs
  if(T->leaf > -1) {
    *(followpos + T->leaf) = rbt.create();
    // recall T->symbol is not the actual symbol but a reduced set
    // actual symbol read is in T->lexeme
    *(positionToSymbol + T->leaf) = T->symbol; 
  }
  RBT *r, *s, *ri;
  if(T->token == STAR) {
    r = T->lastpos;
    s = T->firstpos;
    ri = r;
    do{
      joinRBT(followpos + ri->key - 1, s);
      ri = ri->next;
    }while(ri != r);

  }else if(T->token == CAT) {
    r = T->left->lastpos;
    s = T->right->firstpos;
    ri = r;
    do{
      joinRBT(followpos + ri->key - 1, s);
      ri = ri->next;
    }while(ri != r);
  }*/
}

int count(RBT *T) {
  if(!rbt.isEmpty(T)) {
  RBT *c = T;
  int count = 0;
  do{
    count++;
    c = c->next;
  } while(c!=T);
  return count;
  }return 0;
}

void generateStates(RBT *firstState) {
  /*TODO: needs several revisions
  // info->transitions are actual transitions from the automaton.
  info->transitions = stack.create();
  // transitions is a temporal variable used to compute automaton states. Given
  // symbol x (recall symbols are a reduced set of the original alphabet), and 
  // current state being analized (set), automaton transitions to state 
  // transitions[x] (which is a set of positions) 
  RBT **transitions = (RBT **) malloc(info->numSymbols * sizeof(RBT));
  uint16_t i;
  for(i = 0; i < info->numSymbols; i++) {
    *(transitions + i) = rbt.create();
  }
  RBT *states = rbt.create();
  State *set = insertState(&states, firstState, numPositions);
  Stack *newStates = createStack();
  // This is a shortcut to avoid reading all symbols;
  Stack *symbols = createStack(); 
  push(newStates, set);
  while(newStates->size > 0) {
    set = (State *) stack.pop(newStates);
    // loop the positions
    RBT *current = set->positions;
    do {
      int16_t p = current->key;
      // this check is necessary, followpos(p) can be empty
      if(!isEmptyRBT(followpos[p - 1])) {
        uint8_t symbol = positionToSymbol[p - 1];
        if(isEmptyRBT(*(transitions + symbol))) {
          uint8_t *s = malloc(sizeof(SYM));
          *s = symbol;
          push(symbols, s); 
        }
        joinRBT(transitions + symbol, followpos[p - 1]);    
      }
      current = current->next;
    } while(current != set->positions);
    //freeRBT(set->positions); // we migth free this now
    // We need to search for new states
    while(symbols->size > 0) {
      uint8_t *symbol = stack.pop(symbols);
      RBT *candidate = *(transitions + *symbol);
      State *new = insertState(&states, candidate, numPositions);
      if(found) { // A new state was found!
        push(newStates, new);
      }
      fprintf(fpLog, "(%d%c, %c, %d%c)\n", set->id, set->isFinal?'f':'q', info->alphabet[*symbol], new->id, new->isFinal?'f':'q');
      push(info->transitions, createTransition(set->id, *symbol, new->id));
      *(transitions + *symbol) = rbt.create();
      free(symbol);
    }
  }
  info->numStates = numStates;
  //dumpStates(states, 0);
  // clean up section
  freeStates(states);
  free(transitions);
  destroyStack(symbols);
  destroyStack(newStates);*/
}

void makeAutomaton(Config *config) {
  // 0. Initialize necessary structures. 
  init(config);
  // 1. Build syntax tree for augmented regular expression (r)#
  PTNode *T = parse(config->input);  /*
  // 2. Compute auxiliary functions nullable, firstpos and lastpos 
  computeAuxiliaryFunctions(T);
  // 3. Compute followpos function
  postordenPT(T, &computeFollowPos);
  // Dumping nullable, firstpos, lastpos and follopos to file
  dumpSyntaxTree(fpLog, T);
  dumpFP(followpos, positionToSymbol, numPositions);
  // Syntax tree is no longer needed except T->firstpos 
  RBT *firstpos = T->firstpos;
  T->firstpos = rbt.create(); // overwrite so firstpost does not get deleted
  freePT(T);
  // Generate states
  generateStates(firstpos);
  printf("FDA construction completed.\n"); */
}

Transition *createTransition(int16_t currentState, uint8_t symbol, int16_t nextState) {
  Transition *T = (Transition *) malloc(sizeof(Transition));
  if(T) {
    T->currentState = currentState;
    T->symbol = symbol;
    T->nextState = nextState;
  }else{
    fprintf(stderr, "Insufficient memory to create Transition.");
    exit(EXIT_FAILURE);
  }
  return T;
}

void dumpFP(RBT **followpos, uint8_t *positionToSymbol, int16_t size) {
  if(fpLog) {
    fprintf(fpLog, "\nDumping followpos information\n\n");
    uint16_t i;
    for(i = 0; i < size; i++) {
      fprintf(fpLog, "Leaf[%d] = %c\n", i + 1, info->alphabet[positionToSymbol[i]]);
      fprintf(fpLog, "FollowPos[%d]:\n", i + 1);
      //dumpRBT(fpLog, *(followpos + i));
      fprintf(fpLog, "\n________\n");
    }
  }
}
