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
#include <argp.h>
#include "glex.h"
#include "glxparsetree.h"

const char *argp_program_version = "glex 1.0";
const char *argp_program_bug_address = "?";

/*
   OPTIONS.  Field 1 in ARGP.
   Order of fields: {NAME, KEY, ARG, FLAGS, DOC}.
*/
static struct argp_option options[] = {
  {"verbose", 'v', 0, 0, "verbose"},
  {"output",  'o', "STRING1", 0, "Do something with STRING1 related to the letter A"},
  {0}
};

/*
   PARSER. Field 2 in ARGP.
   Order of parameters: KEY, ARG, STATE.
*/
static error_t parse_opt (int key, char *arg, struct argp_state *state) {
  Config *config = state->input;
  switch (key) {
    case 'v':
      config->verbose = 1;
      break;
    case 'o':
      config->outfile = arg;
      break;
    case ARGP_KEY_ARG:
      if (state->arg_num > 1) {
        argp_usage(state);
      }
      config->input = arg;
      break;
    case ARGP_KEY_END:
      if (state->arg_num < 1) {
       argp_usage(state);
      }
      break;
    default:
      return ARGP_ERR_UNKNOWN;
    }
  return 0;
}

/* Description of the non-option command-line arguments that we accept. */
static char args_doc[] = "ARG1 ARG2";

/*   */
static char doc[] =
"GLEX -- DFA Generator beta (2015 Oct 31\n).\vwhat?.";

/*
   The ARGP structure itself.
*/
static struct argp argp = {options, parse_opt, args_doc, doc};

void usage(void) {
  
  printf("GLEX - DFA Generator beta (2015 Oct 31)\n");
  printf("usage: glex <file>    parse specified file\n");
  //printf("Arguments:\n");
  //printf(" -x    xx\n");
}

static void err_show(char *mod, char *fn, char *msg) {
  fprintf(
    #ifndef DEBUG_H
    stderr,
    #else
    stdout, 
    #endif
    "[error]: Source: %s:%s, Message: %s.\n", mod, fn, msg
  );
  #ifndef DEBUG_H
  exit(EXIT_FAILURE);
  #endif
}

err_lib const err = { 
  err_show
};

#ifndef UNIT_TESTING_H
int main(int argc, char **argv) {
  Config config;
  argp_parse (&argp, argc, argv, 0, 0, &config);
  makeAutomaton(&config);
  return 0;
}
#endif
