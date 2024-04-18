#!/bin/tcsh
if($# < 3) then
echo "usage for running xshare - <yes/no> short flit ratio - <sm ratio> <table>"
echo $#
exit -1
endif

cd /i3c/other/rdas/pact-topology/xshare 



if($1 == "yes") then
 set xshare = "-xshare -sm $2"
else
 set xshare = ""
endif

set table = $3
set eoftable = `wc -l $table | cut -d" " -f1`

set index = 1

while ($eoftable)
  set file = `cat $table | head -n $index | tail -n 1 | cut -d: -f1`
  set cmd  = `cat $table | head -n $index | tail -n 1 | cut -d: -f2`
  ./nox $cmd -$xshare > results/$file-out 
  set eoftable = `expr $eoftable - 1`
  set index = `expr $index + 1`
end
