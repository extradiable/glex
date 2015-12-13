#!/usr/bin/tclsh

exec rm -f rbtree.exe
exec rm -f compilation.log

set command {gcc -g -DDEBUG_H -o rbtree.exe}
lappend command "../../../src/glxerrors.h"
lappend command "../../../src/glxerrors.c"
lappend command "../../../src/glxrbtree.h"
lappend command "../../../src/glxrbtree.c"
lappend command "test_glxrbtree.c"
if {[catch {exec >& compilation.log {*}$command}]} {
  puts "compilation FAILED." 
  exit 1
} else {
  puts "compilation SUCCEDED." 
  exit 0
}
