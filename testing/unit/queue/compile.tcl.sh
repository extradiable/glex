#!/usr/bin/tclsh

exec rm -f queue.exe
exec rm -f compilation.log

set files {}
lappend files "../../../src/glxerrors.h"
lappend files "../../../src/glxerrors.c"
lappend files "../../../src/glxqueue.h"
lappend files "../../../src/glxqueue.c"
lappend files "test_glxqueue.c"

set command {gcc -g -DDEBUG_H -o queue.exe}
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
