
exec rm -f rbtree.exe
exec rm -f compilation.log
exec gcc -g -DDEBUG_H -o rbtree.exe \
  ../../../src/glxrbtree.h \
  ../../../src/glxrbtree.c \
  test_glxrbtree.c \
  > compilation.log
