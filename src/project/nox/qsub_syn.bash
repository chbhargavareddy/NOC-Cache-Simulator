#!/bin/bash
traffics=("NR" "TP" "TN" "BC" "NB" "HT")  
export traffic_pattern

for(( i = 0;i < 5;i ++)); do 
  traffic_pattern=${traffics[$i]}
  qsub -v traffic_pattern -N $traffic_pattern run.bash 
done

qsub -v traffic_pattern="HT" -N HT run_HT.bash 
