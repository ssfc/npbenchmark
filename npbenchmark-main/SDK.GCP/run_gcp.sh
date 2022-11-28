#!/bin/bash
g++ -static-libgcc -static-libstdc++ tabucol.cpp GraphColoring.cpp Main.cpp -O3
#./a.out 999999 3 <./data/DSJC0250.9.txt > sln.250.9.txt
#./a.out 999999 4 <./data/DSJC0250.9.txt > sln.250.9.txt
#./a.out 999999 5 <./data/DSJC0250.9.txt > sln.250.9.txt

data_path="./data/DSJC0250.9.txt"

for (( i=1; i<=$1; i++ ))
do
    echo "Random seed: $i "
    ./a.out 999999 "$i" < $data_path > sln.250.9.txt
done





