#!/bin/bash
traffics=("NR" "TP" "HT") 
topologies=("mesh64" "fbfly64" "mecs64" "mesh64_I" "fbfly64_I" "mecs64_I") 
echo -ne "" > pow_res.csv

for(( i = 0;i < 2;i ++)); do 
  for((j = 0;j < 6;j ++)); do 
    traffic_file=$(ls ./${topologies[$j]}/pow_${traffics[$i]}*.*) 
    echo "$traffic_file" 
    awk -f make_pow_table.txt $traffic_file >> pow_res.csv 
    echo "" >> pow_res.csv
  done
done 
