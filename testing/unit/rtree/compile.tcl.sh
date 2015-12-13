#!/usr/bin/tclsh

exec rm -f rtree.exe
exec rm -f compilation.log

set command {gcc -g -DDEBUG_H -o rtree.exe}
lappend command "../../../src/glxrbtree.h"
lappend command "../../../src/glxrbtree.c"
lappend command "../../../src/glxrtree.h"
lappend command "../../../src/glxrtree.c"
lappend command "test_glxrtree.c"
if {[catch {exec >& compilation.log {*}$command}]} {
  puts "compilation FAILED." 
  exit 1
} else {
  puts "compilation SUCCEDED." 
  exit 0
}
