/* 
 * Starting point for GLEX. GLEX intends to be a basic alternative to LEX. GLEX
 * parses a set of regular expressions and generates a deterministic finite 
 * automaton (DFA).
 *
 * @name: glex.c
 * @purpose: Parse a set of regular expressions and produces a DFA
 * @author: extradiable
 * @references: 
 *   An n log n Algorithm for Minimizing States in a Finite Automaton
 *   Compilers Priciples, Techniques, and Tools
 *   Introduction to Algorithms (Cormen, Leiserson, Rivest, Stein)
 * @date: mar may 12 06:45:44 CDT 2015
 * @update: sáb oct 31 07:10:38 CST 2015
 * 
 */

#include <stdio.h>
#include "glxparsetree.h"

void usage(void){
  
  printf("GLEX - DFA Generator beta (2015 Oct 31)\n");
  printf("usage: glex <file>    parse specified file\n");
  //printf("Arguments:\n");
  //printf(" -x    xx\n");
}

int main(int argc, char **argv){
  if(argc > 1){
    makeAutomaton(argv[1]);
  }else{
    usage();
    return 1;
  }
  return 0;
}
