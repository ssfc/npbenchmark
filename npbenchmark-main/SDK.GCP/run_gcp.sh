#!/bin/bash
g++ -static-libgcc -static-libstdc++ tabucol.cpp GraphColoring.cpp Main.cpp -O3
#./a.out 999999 3 <./data/DSJC0250.9.txt > sln.250.9.txt
#./a.out 999999 4 <./data/DSJC0250.9.txt > sln.250.9.txt
#./a.out 999999 5 <./data/DSJC0250.9.txt > sln.250.9.txt

data_path=$1
save_path=$2
num_seed=$3

for (( i=1; i<="$num_seed"; i++ ))
do
    echo "Random seed: $i "
    ./a.out 999999 "$i" < "$data_path" > "$save_path"
done





