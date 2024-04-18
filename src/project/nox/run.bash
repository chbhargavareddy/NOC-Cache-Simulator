#!/bin/bash
#PBS -l walltime=0:30:00
#PBS -l nodes=1:ppn=1
#PBS -j oe

#cd $PBS_O_WORKDIR 

traffic_pattern=CUST
if [ -z "${traffic_pattern}" ] 
then 
  echo "ERROR: traffic pattern not defined" 
  exit
fi 

echo "-+-+-+-+ Running traffic ${traffic_pattern} - start time `date`" 
for ((j = 100;j <= 100;j ++))
do
  load="$(echo "scale=5;$j*0.01" | bc)"  
  ./bin/nox -num-PE 64 -nr 8 -nc 8 -nvc 1 -bd 15 -mlen 2 -max-msgs 10 -arb RR -t FBFLY -l ${load} -p ${traffic_pattern} -v
done 
echo "-+-+-+-+ Completed traffic ${traffic_pattern} - end time `date`" 
