#!/usr/bin/tclsh

exec rm -f lexer.exe
exec rm -f compilation.log
exec \
  gcc -g -DDEBUG_H -o lexer.exe \
  ../../../src/glxlexer.h \
  ../../../src/glxlexer.c \
  test_glxlexer.c \
  > compilation.log
