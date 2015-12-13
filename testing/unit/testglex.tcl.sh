#!/usr/bin/tclsh

cd stack
source compile.sh
exec ./stack.exe >& stack.log
set output1 [exec sha1sum stack.log.ref]
set output2 [exec sha1sum stack.log]
if {[string equal [lindex $output1 0] [lindex $output2 0]]} {
  puts "\[stack]: PASS"
} else {
  puts "\[stack]: FAILED"
}

cd ../queue
source compile.sh


cd ../rbtree
source compile.sh
