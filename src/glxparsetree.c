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

#include <stdio.h>
#include "glxlexer.h"
#include "glxrtree.h"
#include "glxstack.h"
#include "glxparsetree.h"

/** SECTION: DEFINE */

#define consumeBlanks() \
  while (ascii == ' ' || ascii = '\t') {\
    ascii = readByte();\
  }

/** SECTION: GLOBALS */

/* This pointer is maintained for logging purposes. */
FILE *fpLog = NULL;

/*
 * Alphabet is ascii and all symbols are allowed, there are however, symbols 
 * with an special meaning. In order to check if the current symbol has an
 * special meaning you can check in this data structure.
 */
RBNode *specials = NULL;

/* Current symbol read from file. */
uint8_t current;

/* Counter for the number of positions in the syntax tree. */
int16_t numPositions = 0;

/* 
 * In this implementation I'm considering only ascii symbols, nevertheless only
 * a subset of them might be used for a particular automata.
 */
RBNode *symbols;

/*
 * Function followpos, it is used so frequently that I decided to make it global.
 * Given a position p, there is a set of positions Q such that followpos(p) = Q.
 * Fore more details on this fuction please refer to "Compilers: Principles, 
 * Techniques & Tools.
 */
RBNode **followpos = NULL;

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
RBNode *high = NULL;


/** SECTION: FUNCTION DEFINITIONS */

/* Every non empty line should be of the form <name>:<expression> */
void readExpressionName(void);

/* Initialization */
void init(void);

/** SECTION: LOGIC */

PTNode *glx_parse(uint8_t *fileName){
  Stack tree = createStack();
  PTNode *T = NULL;
  initLexer(fileName);
  do {
    readExpressionName();
       
  }



  while((current = readByte()) != '\0'){
    if(!containsKeyRBT(whitespaces, current)){
      if(T){
        T = join(T, &re, OR);
      }else{
        T = re();
      }
    }
  }
  // we are done with the reader
  freeBuffer();
  if(current == '\0'){
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
}

void readExpressionName() {
  RTree *branch = dictionary;
  uint8_t name[MAX_EXPNAME_LENGTH + 1];
  uint8_t ascii = readByte();
  uint8_t i;
  consumeBlanks(); 
  for(i = 0; i < MAX_EXPNAME_LENGTH; i++) {
    uint8_t hnibble = ascii & 0xF0;
    if (!containsKeyRBT(high, hnibble)) {
      if (ascii != '_') {
        break; // ascii is not a valid character
      }
    }
    name[i] = ascii;
    branch =  getBranch(branch, ascii);
  }
  name[MAX_EXPNAME_LENGTH] = '\0'; 
  consumeBlanks()
  if (ascii != ':') {
    fprintf(stderr, "GLEX error: Expected ':' but found '%c'. Line %d.\n", ascii, currentLine);
    exit(EXIT_FAILURE);
  } else if (i == 0) {
    fprintf(stderr, "GLEX error: No expression name was given. Line %d.\n", currentLine);
    exit(EXIT_FAILURE);
  } else if (branch->value != NULL) {
    fprintf(stderr, "GLEX error: Duplicated name found '%s'. Line: %d.\n", name, currentLine);
    exit(EXIT_FAILURE);
  } else {
    uint8_t *value = (uint8_t *) malloc(sizeof(uint8_t));
    currentExpression = ++numExpressions;
    *value = currentExpression;
    setValue(branch, value);
  }
}

void init(){
  high = createRBT();
  insertRBN(&high, NUM_MASK);
  insertRBN(&high, LCL_MASK);
  insertRBN(&high, UCL_MASK);

  dictionary = createRTree();
  fpLog = fopen("glex.log", "w");
  if(!fpLog){
    printf("Unable to open glex.log");
  }
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
  symbols = createRBT();

  info = (ParseInfo *) malloc(sizeof(ParseInfo));
  if(info){
    info->numStates = 0;
    info->numSymbols = 0;
    info->finalState = 0;
    info->alphabet = NULL;
    info->transitions = createStack();
  }else{
    fprintf(stderr, "Insufficient memory to create ParseInfo.");
    exit(EXIT_FAILURE);
  }
}







































/* Logging */

void dumpFP(RBNode **followpos, uint8_t *positionToSymbol, int16_t size);

/* Syntax tree */

PTNode *cat(void);
PTNode *or(void);
PTNode *re(void);
PTNode *star(void);
PTNode *sym(void);
PTNode *createSymbol(void);
PTNode *join(PTNode *left, PTNode *(*fn)(void), int8_t operand);

/* DFA Generation */

void followPos(PTNode *T, RBNode **followpos, int16_t *leaf);
void preprocessSyntaxTree(PTNode *T);
void computeAuxiliaryFunctions(PTNode *T);
void computeFollowPos(PTNode *T);
void generateStates(RBNode *firstState);
Transition *createTransition(int16_t currentState, uint8_t symbol, int16_t nextState);

/* 
 * This function directs the flow from input file to DFA. If you want to know
 * the flow of this file, this is the function you want to take a look.
 */
void glx_makeAutomaton(uint8_t *fileName);





/** SECTION: CONSTANTS */

// Special tokens
#define AUGMENTED   '#'
#define SYM         'S'

// Tokens for specials
#define OR          '|'
#define CAT         '.'
#define STAR        '*'
#define PLUS        '+'
#define OPTIONAL    '?'
#define O_GROUP     '('
#define C_GROUP     ')'
#define END         '\0'
#define EOL         '\n'





PTNode *join(PTNode *left, PTNode *(*fn)(void), int8_t operand){
  PTNode *right = NULL, *root = NULL;
  if(fn){
    right = (*fn)();
  }
  root = createPTNode(operand, operand, 0);
  root->left = left;
  root->right = right;
  return root;
}

PTNode *re(){
  return or();
}

PTNode *or(){
  PTNode *t = cat();
  while(current == OR){
    current = readByte();
    t = join(t, &cat, OR);
  }
  return t;
}

PTNode *cat(){
  PTNode *t = star();
  while(!containsKeyRBT(specials, current)){ // no special meaning for current
    t = join(t, &star, CAT);
  }
  return t;
}

PTNode *star(){
  PTNode *t = sym();
  if(current == STAR){
    t = join(t, NULL, STAR);  
  }
  while(current == STAR){
    current = readByte();
  }
  return t;
}

PTNode *sym(){
  PTNode *t;
  if(current == O_GROUP){ 
    current = readByte();
    t = re();
    if(current == C_GROUP){
      current = readByte();
    }else{
      fprintf(stderr, "Unexpected end of input. ')' is missing.");
      exit(EXIT_FAILURE);
    }
  }else if(current == '\\'){
    t = NULL;
  }else{
    t = createSymbol();
    current = readByte();
  }
  return t;
}

PTNode *createSymbol(void){
  uint8_t *s = malloc(sizeof(uint8_t));
  *s = 0; // try to insert symbol for first time
  if(s != NULL){
    RBNode *N = insertRBN(&symbols, current, s); 
    if(s == N->data){ // new symbol
      *s = info->numSymbols++;
    }
    s = N->data; // retrieve the actual state of the symbol
    // c is mapping from original alphabet to subset
    return createPTNode(SYM, current, *s); 
  }else{
    fprintf(stderr, "Insufficient memory to create symbol.");
    exit(EXIT_FAILURE);
  }
}

void preprocessSyntaxTree(PTNode *T){
  PTNode *L = T->left;
  PTNode *R = T->right;
  switch(T->token){
    case AUGMENTED:
    case SYM:
      T->leaf = numPositions++;
      if(T->lexeme == '\0'){ // Probably need to change this
        T->nullable = 1;
      }else{
        T->nullable = 0;
        insertRBN(&T->firstpos, numPositions, NULL);
        insertRBN(&T->lastpos, numPositions, NULL);
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
      if(L->nullable){
        joinRBT(&T->firstpos, L->firstpos);
        joinRBT(&T->firstpos, R->firstpos);
      }else{
        joinRBT(&T->firstpos, L->firstpos);
      }
      if(R->nullable){
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
  }
}

void computeAuxiliaryFunctions(PTNode *T){
  // at this point symbols contains the list of all necessary symbols for FDA
  // lets create the alphabet for ParseInfo Structure
  info->alphabet  = (uint8_t *) malloc(info->numSymbols * sizeof(uint8_t));
  RBNode *next = symbols;
  do {
    uint8_t *c = next->data;
    info->alphabet[*c] = next->key;
    next = next->next;
  } while(next != symbols); 
  freeRBT(symbols); // we dont need this anymore
  postordenPT(T, &preprocessSyntaxTree);
  followpos = (RBNode **) malloc(numPositions * sizeof(RBNode));
  // I expect less positions than symbols.. char will be fine?
  positionToSymbol = (uint8_t *) malloc(numPositions * sizeof(uint8_t)); 
}

void computeFollowPos(PTNode *T){
  // Take advantage of the walk to initialize the array with the leafs
  if(T->leaf > -1){
    *(followpos + T->leaf) = createRBT();
    // recall T->symbol is not the actual symbol but a reduced set
    // actual symbol read is in T->lexeme
    *(positionToSymbol + T->leaf) = T->symbol; 
  }
  RBNode *r, *s, *ri;
  if(T->token == STAR){
    r = T->lastpos;
    s = T->firstpos;
    ri = r;
    do{
      joinRBT(followpos + ri->key - 1, s);
      ri = ri->next;
    }while(ri != r);

  }else if(T->token == CAT){
    r = T->left->lastpos;
    s = T->right->firstpos;
    ri = r;
    do{
      joinRBT(followpos + ri->key - 1, s);
      ri = ri->next;
    }while(ri != r);
  }
}
int count(RBNode *T){
  if(!isEmptyRBT(T)){
  RBNode *c = T;
  int count = 0;
  do{
    count++;
    c = c->next;
  } while(c!=T);
  return count;
}return 0;
}

void generateStates(RBNode *firstState){
  // info->transitions are actual transitions from the automaton.
  info->transitions = createStack();
  // transitions is a temporal variable used to compute automaton states. Given
  // symbol x (recall symbols are a reduced set of the original alphabet), and 
  // current state being analized (set), automaton transitions to state 
  // transitions[x] (which is a set of positions) 
  RBNode **transitions = (RBNode **) malloc(info->numSymbols * sizeof(RBNode));
  uint16_t i;
  for(i = 0; i < info->numSymbols; i++){
    *(transitions + i) = createRBT();
  }
  RBNode *states = createRBT();
  State *set = insertState(&states, firstState, numPositions);
  Stack *newStates = createStack();
  // This is a shortcut to avoid reading all symbols;
  Stack *symbols = createStack(); 
  push(newStates, set);
  while(newStates->size > 0) {
    set = (State *) pop(newStates);
    // loop the positions
    RBNode *current = set->positions;
    do {
      int16_t p = current->key;
      // this check is necessary, followpos(p) can be empty
      if(!isEmptyRBT(followpos[p - 1])){
        uint8_t symbol = positionToSymbol[p - 1];
        if(isEmptyRBT(*(transitions + symbol))){
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
    while(symbols->size > 0){
      uint8_t *symbol = pop(symbols);
      RBNode *candidate = *(transitions + *symbol);
      State *new = insertState(&states, candidate, numPositions);
      if(found){ // A new state was found!
        push(newStates, new);
      }
      fprintf(fpLog, "(%d%c, %c, %d%c)\n", set->id, set->isFinal?'f':'q', info->alphabet[*symbol], new->id, new->isFinal?'f':'q');
      push(info->transitions, createTransition(set->id, *symbol, new->id));
      *(transitions + *symbol) = createRBT();
      free(symbol);
    }
  }
  info->numStates = numStates;
  //dumpStates(states, 0);
  // clean up section
  freeStates(states);
  free(transitions);
  destroyStack(symbols);
  destroyStack(newStates);
}

void glx_makeAutomaton(uint8_t *fileName){
  // 0. Initialize necessary structures. 
  init();
  // 1. Build syntax tree for augmented regular expression (r)#
  PTNode *T = glx_parse(fileName);  
  // 2. Compute auxiliary functions nullable, firstpos and lastpos 
  computeAuxiliaryFunctions(T);
  // 3. Compute followpos function
  postordenPT(T, &computeFollowPos);
  // Dumping nullable, firstpos, lastpos and follopos to file
  dumpSyntaxTree(fpLog, T);
  dumpFP(followpos, positionToSymbol, numPositions);
  // Syntax tree is no longer needed except T->firstpos 
  RBNode *firstpos = T->firstpos;
  T->firstpos = createRBT(); // overwrite so firstpost does not get deleted
  freePT(T);
  // Generate states
  generateStates(firstpos);
  printf("FDA construction completed.\n");
}

Transition *createTransition(int16_t currentState, uint8_t symbol, int16_t nextState){
  Transition *T = (Transition *) malloc(sizeof(Transition));
  if(T){
    T->currentState = currentState;
    T->symbol = symbol;
    T->nextState = nextState;
  }else{
    fprintf(stderr, "Insufficient memory to create Transition.");
    exit(EXIT_FAILURE);
  }
  return T;
}

void dumpFP(RBNode **followpos, uint8_t *positionToSymbol, int16_t size){
  if(fpLog){
    fprintf(fpLog, "\nDumping followpos information\n\n");
    uint16_t i;
    for(i = 0; i < size; i++){
      fprintf(fpLog, "Leaf[%d] = %c\n", i + 1, info->alphabet[positionToSymbol[i]]);
      fprintf(fpLog, "FollowPos[%d]:\n", i + 1);
      dumpRBT(fpLog, *(followpos + i));
      fprintf(fpLog, "\n________\n");
    }
  }
}
