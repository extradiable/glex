#!/usr/bin/tclsh

proc color {foreground text} {
  return [exec tput setaf $foreground]$text[exec tput sgr0]
}

proc compile {module} {
  puts "\[$module] Prepare for compilation."
  exec rm -f "${module}.exe"
  exec rm -f compilation.log
  puts "\[$module] Prepare gcc command."
  source sources.tcl.sh
  set command "gcc -g -DDEBUG_H -o ${module}.exe"
  foreach file $files {
    lappend command $file
  } 
  puts "\[$module] Compiling sources."
  puts -nonewline "\[$module] Compilation: "
  if {[catch {exec >& compilation.log {*}$command}]} {
    puts "[color 1 FAILED]."
    return 0
  } else {
    puts "[color 2 SUCCEDED]."
    return 1
  }
}

proc runTest {module} {
  puts "\[$module] Running test."
  puts -nonewline "\[$module] Test execution: "
  if {[catch {exec "./${module}.exe" >& "${module}.log"}]} {
    puts "[color 1 FAILED]."
    return 0
  } else {
    puts "[color 2 SUCCEDED]."
    return 1
  }
}

proc checkOutput {module} {
  puts "\[$module] Comparing output with reference log."
  set output1 [exec sha1sum "${module}.log.ref"]
  set output2 [exec sha1sum "${module}.log"]
  return [string equal [lindex $output1 0] [lindex $output2 0]]
}

cd unit
set dirs [glob -type d *]

foreach dir $dirs {
  cd $dir
  set pass 0
  if {[compile $dir]} {
    if {[runTest $dir]} {
      if {[checkOutput $dir]} {
        set pass 1
      }
    }
  }
  puts -nonewline "\[$dir] Test: "
  if {$pass == 1} {
    puts "[color 2 SUCCEDED]."
  } else {
    puts "[color 1 FAILED]."
  }
  cd ..
  puts "------------------------------"
}
