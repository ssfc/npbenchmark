#!/bin/bash
g++ my_main.cpp -O3
#./a.out 999999 3 <./data/DSJC0250.9.txt > sln.250.9.txt
#./a.out 999999 4 <./data/DSJC0250.9.txt > sln.250.9.txt
#./a.out 999999 5 <./data/DSJC0250.9.txt > sln.250.9.txt

num_seed=$1

for (( i=1; i<="$num_seed"; i++ ))
do
    echo "Random seed: $i "
    ./a.out
done





