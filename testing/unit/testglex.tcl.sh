#!/usr/bin/tclsh

set dirs {stack queue rbtree}

foreach dir $dirs {
  cd $dir
  exec tclsh compile.tcl.sh
  exec "./${dir}.exe" > "${dir}.log"
  set output1 [exec sha1sum "${dir}.log.ref"]
  set output2 [exec sha1sum "${dir}.log"]
  if {[string equal [lindex $output1 0] [lindex $output2 0]]} {
    puts "\[$dir]: PASS"
  } else {
    puts "\[$dir]: FAILED"
  }
  cd ..
}
