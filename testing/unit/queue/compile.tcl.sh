#!/usr/bin/tclsh

exec rm -f queue.exe
exec rm -f compilation.log

set command {gcc -g -DDEBUG_H -o queue.exe}
lappend command "../../../src/glxerrors.h"
lappend command "../../../src/glxerrors.c"
lappend command "../../../src/glxqueue.h"
lappend command "../../../src/glxqueue.c"
lappend command "test_glxqueue.c"
if {[catch {exec >& compilation.log {*}$command}]} {
  puts "compilation FAILED." 
  exit 1
} else {
  puts "compilation SUCCEDED." 
  exit 0
}
