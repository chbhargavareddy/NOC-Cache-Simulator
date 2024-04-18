#!/bin/tcsh
if($# < 3) then
echo "usage <topology> <channel width> <network size param#1> <network size param#2> <network size param#3 (optional)>"
echo $#
exit -1
endif

cd /i3c/hpcl/rdas/workspace-linux-simics/modules/pepsi/NoX/nox  

set topology = $1
set chwidth = $2
set nsize = ""

if($topology == "hybrid-R") then
set topology = ${topology}$4
set mratio = `echo "scale = 2 ; $chwidth/1024" | bc` 
set nsize =  "-t TORUS -nr 1 -nc $3 -cdegree $4 -hybrid $mratio" 
set nodes = `echo "scale = 2 ; $3*$4" | bc` 
echo "HYBRID-R $nodes $chwidth"
endif

if($topology == "hybrid-T") then
set topology = ${topology}$5
set mratio = `echo "scale = 2 ; $chwidth/1024" | bc` 
set nsize =  "-t TORUS -nr $3 -nc $4 -cdegree $5 -hybrid $mratio" 
set nodes = `echo "scale = 2 ; $3*$4*$5" | bc` 
echo "HYBRID-T $nodes $chwidth"
endif


if($topology == "bfly") then
set nsize =  "-t BFLY -ary $3 -fly $4"
set nodes = `echo "scale = 2 ; $3^$4" | bc` 
echo "BFLY $nodes $chwidth"
endif

if($topology == "bus") then
echo "BUS"
set nsize =  "-t BUS -nr 1 -nc $3"
set nodes = $3
echo "BUS $nodes $chwidth"
endif

if($topology == "mesh") then
set nsize =  "-t MESH -nr $3 -nc $4"
set nodes = `echo "scale = 2 ; $3*$4" | bc` 
echo "MESH $nodes $chwidth"
endif

if($topology == "cmesh") then
echo "CMESH"
set topology = ${topology}$5
set nsize =  "-t MESH -nr $3 -nc $4 -cdegree $5"
set nodes = `echo "scale = 2 ; $3*$4*$5" | bc` 
echo "CMESH $nodes $chwidth"
endif

if($topology == "ring") then
echo "RING"
set nsize =  "-t TORUS -nr 1 -nc $3"
set nodes = $3
echo "RING $nodes $chwidth"
endif

echo "load-rate\tlatency\tthroughput\tpower\tenergy-per-packet\tPDP" >  results/out-$topology-$chwidth-$nodes
#foreach load_rate ("0.1" "0.2") 
foreach load_rate ("0.005" "0.01" "0.02" "0.03" "0.04" "0.05" "0.06" "0.07" "0.08" "0.09" "0.1") 
  set file = "temp-results/out-$topology-$chwidth-$nodes-$load_rate"
  ./bin/nox $nsize -fsize $chwidth -l $load_rate > $file 
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
  echo "Done $load_rate"
 end
