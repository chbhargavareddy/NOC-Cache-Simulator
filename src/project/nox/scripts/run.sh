#!/bin/tcsh
if($# < 2) then
echo "usage for running <table> <load rate>"
echo $#
exit -1
endif

cd /i3c/other/rdas/fairness-parbs 

set common = "-t MESH -nr 8 -nc 4 -fsize 256 -max-msgs 100000 -nvc 4"
set load   = "-l $2"
set cap    = "-batch-cap $3"
set interval = "-batch-int $4"

set table = $1
set eoftable = `wc -l $table | cut -d" " -f1`

set index = 1

while ($eoftable)
  set file = `cat $table | head -n $index | tail -n 1 | cut -d: -f1`
  if($file != "##")
    set cmd  = `cat $table | head -n $index | tail -n 1 | cut -d: -f2`
    #./nox $common $load $cmd $cap $interval > results/$file-load$2-cap$3-out 
    echo done ... $file $cmd
  endif
  set eoftable = `expr $eoftable - 1`
  set index = `expr $index + 1`
end
