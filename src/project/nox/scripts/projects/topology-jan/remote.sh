#!/usr/bin/expect -f
send_user "$argv0 [lrange $argv 0 2]\n" 
set machine [lindex $argv 0] 
set exe [lindex $argv 1] 
spawn ssh $machine 
sleep 20
expect "%"
send "cd /i3c/hpcl/rdas/workspace-linux-simics/modules/pepsi/NoX/nox/scripts\r"
expect "%"
send "$exe &\r"
expect "%"
send exit
