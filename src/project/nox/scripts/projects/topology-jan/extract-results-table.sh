#!/bin/tcsh
if($# < 3) then
echo "usage <load rate> <config table> <param(latency/throughput/power/pdp/epm/hop>"
echo $#
exit -1
endif

cd /i3c/hpcl/rdas/workspace-linux-simics/modules/pepsi/NoX/nox/results 

set injrate = $1
set table = $2
set eoftable = `wc -l $table | cut -d" " -f1`
set param = $3

# 1. load-rate 2. latency	3. throughput 4.power 5.energy-per-packet 6. PDP
if($param == "latency") then
  set field = 2
endif
if($param == "throughput") then 
set field = 3
endif
if($param == "power") then 
set field = 4
endif
if($param == "epm") then 
set field = 4
endif
if($param == "pdp") then 
set field = 6
endif


set index = 1

while ($eoftable)
set node = `cat $table | head -n $index | tail -n 1 | cut -f1` 
set topology = `cat $table | head -n $index | tail -n 1 | cut -f2` 
set chwidth = `cat $table | head -n $index | tail -n 1 | cut -f3` 
set file = out-$topology-$chwidth-$node
if(-e $file) then
  set value = `grep $injrate $file | cut -f$field ` 
else
 set value  = "no"
endif
echo "$node\t$topology\t$value"
set index = `expr $index + 1`
set eoftable = `expr $eoftable - 1`
end
