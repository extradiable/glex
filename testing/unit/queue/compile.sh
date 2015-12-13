
exec rm -f queue.exe
exec rm -f compilation.log
exec gcc -g -DDEBUG_H -o queue.exe \
  ../../../src/glxqueue.h \
  ../../../src/glxqueue.c \
  test_glxqueue.c \
  > compilation.log
