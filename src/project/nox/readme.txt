Steps to execute the Assignment

1. Extract the file from zip folder
2. copy them to nox folder and replace the existing files
3. Run "make clean" to clear the object files
4. Run "make" to create new object files
5. Run the following command to use North last algorithm

./bin/nox -num-PE 64 -nr 8 -nc 8 -nvc 4 -bd 4 -mlen 4 -max-msgs 10000 -arb RR -t MESH -l 0.02 -p TP -r NL