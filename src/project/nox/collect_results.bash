#!/bin/bash
traffics=("NR" "TP" "TN" "BC" "NB" "HT")  
echo -ne "" > res.csv

for(( i = 0;i < 6;i ++)); do 
  traffic_file=$(ls ${traffics[$i]}.*) 
  echo "$traffic_file" 
  awk -f make_table.txt $traffic_file >> res.csv 
  echo "" >> res.csv
done 
