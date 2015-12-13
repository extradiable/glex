#!/usr/bin/tclsh

exec rm -f stack.exe
exec rm -f compilation.log

set command {gcc -g -DDEBUG_H -o stack.exe}
lappend command "../../../src/glxerrors.h"
lappend command "../../../src/glxerrors.c"
lappend command "../../../src/glxstack.h"
lappend command "../../../src/glxstack.c"
lappend command "test_glxstack.c"
if {[catch {exec >& compilation.log {*}$command}]} {
  puts "compilation FAILED." 
  exit 1
} else {
  puts "compilation SUCCEDED." 
  exit 0
}
