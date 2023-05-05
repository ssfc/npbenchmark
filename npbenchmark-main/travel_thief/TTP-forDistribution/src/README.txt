Code for the publication "A Comprehensive Benchmark Set and Heuristics for the Traveling Thief Problem"

Download: http://cs.adelaide.edu.au/~optlog/research/ttp.php

Author: Markus Wagner (wagner@acrocon.com)

If you have any questions, please do not hesitate to contact the author.

-- Details --

Driver.java is the main driver file and contains several examples of how the
code can be used.

The objective function is implemented in TTPInstance.evaluate(...). An 
alternative implementation that is based on Bonyadi's Matlab code can be found 
in ttp.Utils.DriverOld.

ttp.Optimisation contains the iterative heuristics random local search (RLS) and
the (1+1) evolutionary algorithm (EA). Via the parameter "mode", the user can
switch between RLS (mode==1) and EA (mode==2). The switch-case-statements can be
extended easily to incorporate additional heuristics.

ttp.Utils contains several helper function, e.g., a generator for Gridengine 
batch jobs.

instances:
- The Chained Lin-Kernighan heuristic tours in the folder "instances" are based 
on the implementation from http://www.tsp.gatech.edu/concorde/downloads/downloads.htm 
- Exemplary, we include the instances based on the TSP instance a280. All others
can be downloaded from http://cs.adelaide.edu.au/~optlog/CEC2014COMP_Instances/

approach1-10000-600000.txt: a Gridengine batch job file that would execute all
9720 TTP instances once using RLS, and a maximum of "10000 iterations no 
improvment" (up to a total time limit of 600000 ms (=10min)).