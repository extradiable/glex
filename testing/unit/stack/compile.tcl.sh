exec rm -f stack.exe
exec rm -f compilation.log
exec gcc -g -DDEBUG_H -o stack.exe \
  ../../../src/glxstack.h \
  ../../../src/glxstack.c \
  test_glxstack.c \
  > compilation.log
