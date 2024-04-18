#!/usr/bin/expect -f
set machine [lindex $argv 0] 
spawn ssh $machine 
#sleep 20
expect "%"
send "pgrep -u rdas nox\r"  
expect "%"
send "kill -9 -1\r"  
expect "%"
