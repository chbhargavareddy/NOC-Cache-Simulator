#!/bin/bash
opt="-num-PE 64 -t MESH -nr 8 -nc 8 -nvc 6 -mlen 4 -max-msgs 100000 -p NR -bd 5 \
     -arb GSF -atomic "
 
echo "####Sim Start###" > sim_result_GSF_NR 

#for((j=1;j <= 8;j=j*2 ))
#do
  FM_SZ="-frame-size 2 "
  WD_SZ="-window-size 6 "
  echo "###FRAME_SIZE $j" >> sim_result_GSF_NR 
  for((i=20;i<=20;i+=5))
  do 
    echo "#---LOAD: $i" >> sim_result_GSF_NR 
    LD="-l $(echo "scale=4; 0.01*$i" | bc) " 
    ./bin/nox ${opt} ${LD} ${FM_SZ} ${WD_SZ} | tee -a sim_result_GSF_NR
  done
#done  
