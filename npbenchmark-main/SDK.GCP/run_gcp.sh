#!/bin/bash
g++ -static-libgcc -static-libstdc++ tabucol.cpp GraphColoring.cpp Main.cpp -O3
./a.out 999999 5 <./data/DSJC0250.9.txt > sln.250.9.txt


