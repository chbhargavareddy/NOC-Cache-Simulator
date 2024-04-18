#!/bin/bash
#PBS -l walltime=0:30:00
#PBS -l nodes=1:ppn=1
#PBS -j oe

cd $PBS_O_WORKDIR 

if [ -z "${traffic_pattern}" ] 
then 
  echo "ERROR: traffic pattern not defined" 
  exit
fi 

echo "-+-+-+-+ Running traffic ${traffic_pattern} - start time `date`" 
for ((j = 1;j <= 30;j ++))
do
  load="$(echo "scale=5;$j*0.005" | bc)"  
  ./bin/nox -num-PE 16 -nr 4 -nc 4 -nvc 5 -bd 8 -mlen 1 -max-msgs 100000 -arb RR -t MESH -l ${load} -p ${traffic_pattern}
done 
echo "-+-+-+-+ Completed traffic ${traffic_pattern} - end time `date`" 
