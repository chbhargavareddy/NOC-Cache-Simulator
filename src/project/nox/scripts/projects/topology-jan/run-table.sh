#!/bin/tcsh
if($# < 2) then
echo "usage <load rate> <config table> <optional-stages>"
echo $#
exit -1
endif

cd /i3c/hpcl/rdas/workspace-linux-simics/modules/pepsi/NoX/nox/ 

if($# == 3) then
 set stages = $3
else
 set stages = 2
endif

set load_rate = $1
set table = $2
set eoftable = `wc -l $table | cut -d" " -f1`

set index = 1

while ($eoftable)
set nodes = `cat $table | head -n $index | tail -n 1 | cut -f1` 
set topology = `cat $table | head -n $index | tail -n 1 | cut -f2` 
set runtopo = `cat $table | head -n $index | tail -n 1 | cut -f3` 
set row = `cat $table | head -n $index | tail -n 1 | cut -f4` 
set col = `cat $table | head -n $index | tail -n 1 | cut -f5` 
set cd = `cat $table | head -n $index | tail -n 1 | cut -f6` 
set mratio = `cat $table | head -n $index | tail -n 1 | cut -f7` 

set args = "-t $runtopo -nr $row -nc $col -cdegree $cd" 

if($topology == "mesh") then
set args = "-t $runtopo -nr $row -nc $col" 
endif

if($topology == "cmesh-L4") then
set args = "-t $runtopo -nr $row -nc $col -cdegree $cd -balanced" 
endif

if($topology == "bfly") then
set args = "-t $runtopo -ary $row -fly $col" 
endif



foreach chwidth (64 128 205 256 341 512) 
  set file = "temp-results/out-$topology-$chwidth-$nodes-$load_rate"
  set mratio = `echo "scale = 2 ; $chwidth/1024" | bc` 
  if($topology == "hybrid-M8" || $topology == "hybrid-T8" || $topology == "hybrid-R8") then
    set args = "-t $runtopo -nr $row -nc $col -cdegree $cd -hybrid $mratio"
  endif
  ./bin/nox $args -fsize $chwidth -l $load_rate -arch $stages > $file
  #echo "./bin/nox $args -fsize $chwidth -l $load_rate"
  grep "exceeds" $file 
  grep "Power Config not found" $file 
  echo "$load_rate" > temp-results/$topology-$chwidth-$nodes-temp0 
  grep "Avg. Message Latency" $file |  tail -1 | cut -d: -f2 | cut -d"c" -f1 > temp-results/$topology-$chwidth-$nodes-temp1 
  grep "Avg. Throughput" $file |  tail -1 | cut -d: -f2 | cut -d"p" -f1 > temp-results/$topology-$chwidth-$nodes-temp2 
  grep "Power consumption" $file | tail -1 | cut -d: -f2 | cut -d"W" -f1 > temp-results/$topology-$chwidth-$nodes-temp3 
  grep "Energy per message" $file | tail -1 | cut -d: -f2 | cut -d"n" -f1 > temp-results/$topology-$chwidth-$nodes-temp4 
  grep "Power Delay Product" $file | tail -1 | cut -d: -f2 | cut -d"W" -f1 > temp-results/$topology-$chwidth-$nodes-temp5 
  paste  temp-results/$topology-$chwidth-$nodes-temp0 temp-results/$topology-$chwidth-$nodes-temp1 temp-results/$topology-$chwidth-$nodes-temp2 temp-results/$topology-$chwidth-$nodes-temp3 temp-results/$topology-$chwidth-$nodes-temp4 temp-results/$topology-$chwidth-$nodes-temp5 >> results/out-$topology-$chwidth-$nodes 
  rm temp-results/$topology-$chwidth-$nodes-temp? 
  mv temp-results/out-$topology-$chwidth-$nodes-$load_rate results/simout-$topology-$chwidth-$nodes-$load_rate
  echo "Done $topology-$chwidth-$nodes-$load_rate"
end
set index = `expr $index + 1`
set eoftable = `expr $eoftable - 1`
end
