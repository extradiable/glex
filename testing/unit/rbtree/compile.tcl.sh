#!/usr/bin/tclsh

exec rm -f rbtree.exe
exec rm -f compilation.log

set files {}
lappend files "../../../src/glxerrors.h"
lappend files "../../../src/glxerrors.c"
lappend files "../../../src/glxrbtree.h"
lappend files "../../../src/glxrbtree.c"
lappend files "test_glxrbtree.c"

set command {gcc -g -DDEBUG_H -o rbtree.exe}
foreach file $files {
  lappend command $file
}

if {[catch {exec >& compilation.log {*}$command}]} {
  puts "compilation FAILED." 
  exit 1
} else {
  puts "compilation SUCCEDED." 
  exit 0
}
