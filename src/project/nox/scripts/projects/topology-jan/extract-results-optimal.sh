#!/bin/tcsh
if($# < 2) then
echo "usage <load rate> <param(latency/throughput/power/pdp/epm/hop>"
echo $#
exit -1
endif

cd /i3c/hpcl/rdas/workspace-linux-simics/modules/pepsi/NoX/nox/results 

set injrate = $1
set param = $2

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
# sample file name simout-bus-256-16-0.08
# sample file name out-cmesh4-256-32

if(-e tmp) rm tmp
foreach node (16 32 64 128)
foreach topology ("hybrid-T8" "hybrid-M8" "hybrid-R8" "bfly" "cmesh4" "cmesh-L4""cmesh2" "mesh")
#if($topology == "bfly" && $node == 32) then
  #set node = 27
#endif
#if($topology == "bfly" && $node == 128) then
  #set node = 125
#endif
#foreach chwidth (64 128 142 256 512 1024)
foreach chwidth (64 128 205 256 341 512) 
  set file = out-$topology-$chwidth-$node
  if(-e $file) then
    set value = `grep $injrate $file | cut -f$field` 
    set valid = `grep $injrate $file | cut -f$field | wc -l` 
    #grep $injrate $file | cut -f$field | wc -l
    #echo $valid
    if( $valid >= 1) then
      echo $node $topology $chwidth $value >> tmp
    else
      echo $node $topology $chwidth "no" >> tmp
    endif
  endif
end
if(-e tmp) then
  sort -n -k 4 tmp | head -n 1
  rm tmp
endif
#if($topology == "bfly" && $node == 27) then
 #set node = 32
#endif
#if($topology == "bfly" && $node == 125) then
 #set node = 128
#endif
end
end

