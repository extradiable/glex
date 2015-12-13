#!/bin/bash

clear
gcc -g -o glex      \
  glex.c            \
  glxparsetree.c    \
  glxreader.c	    \
  glxrbtree.c	    \
  glxstate.c        \
  glxptnode.c	    \
  glxstack.c        \
2> compilation.log
cat compilation.log	
