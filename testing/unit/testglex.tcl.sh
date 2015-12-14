#!/usr/bin/tclsh

proc color {foreground text} {
  return [exec tput setaf $foreground]$text[exec tput sgr0]
}

set dirs {stack queue rbtree rtree lexer}

foreach dir $dirs {
  set success 0
  cd $dir
  puts "\[$dir]: compiling test."
  if {[catch {exec tclsh compile.tcl.sh} message]} {
    puts "\[$dir]: Compilation failed."
    puts "\[$dir]: Compilation log: ./${dir}/compilation.log."
  } else {
    puts "\[$dir]: running test."
    exec "./${dir}.exe" > "${dir}.log"
    set output1 [exec sha1sum "${dir}.log.ref"]
    set output2 [exec sha1sum "${dir}.log"]
    puts "\[$dir]: comparing output with reference log."
    set success [string equal [lindex $output1 0] [lindex $output2 0]]
  }
  if {$success} {
    puts "\[$dir]: [color 2 PASS]."
  } else {
    puts "\[$dir]: [color 1 FAILED]."
  }
  cd ..
}
