#!/usr/bin/tclsh

exec rm -f rtree.exe
exec rm -f compilation.log

set files {}
lappend files "../../../src/glxerrors.h"
lappend files "../../../src/glxerrors.c"
lappend files "../../../src/glxrbtree.h"
lappend files "../../../src/glxrbtree.c"
lappend files "../../../src/glxrtree.h"
lappend files "../../../src/glxrtree.c"
lappend files "test_glxrtree.c"

set command {gcc -g -DDEBUG_H -o rtree.exe}
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
