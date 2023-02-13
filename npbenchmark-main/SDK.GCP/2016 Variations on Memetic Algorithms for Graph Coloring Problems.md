# Variations on Memetic Algorithms for Graph Coloring Problems
Cited by 54, 2016

## Abstract
HEAD - for HEA in Duet - works with a population of only two individuals. Moreover, a new way of managing diversity is brought by HEAD.

## Introduction
1. k, the number of colors; 
2. assigns to each vertex an integer between 1 and k; 
3. c(v), the color of vertex v.
4. minimal graph partition of independent sets == GCP; 
5. HEAD’ and HEAD, integrating a tabu search procedure with an evolutionary algorithm; 

## Related works
### 2.1 TabuCol
1. **Even** if the objective function is **worse**, at each iteration, one of the best neighbors which are not inside the tabu list is chosen; If there are several **equal best** moves, one chooses one of them at **random**; (2023年2月12日)
2. the objective function of the neighbors is not computed from scratch, but only the difference between the two solutions is computed. (2023年2月13日)

### 2.2 Memetic Algorithms for graph coloring and HEA 
1. In MA, the mutation of the EA is replaced by a local search algorithm. (2023年2月13日)
2. HEA belongs to MA; (2023年2月13日)
3. The crossover used in HEA is called the Greedy Partition Crossover (GPX); **Drawbacks**: (1) GPX is asymmetrical; (2) GPX is a random crossover; (3) The final step produces many conflicts. (2023年2月13日)

### QA-col: Quantum Annealing for graph coloring

## 3 HEAD: Hybrid Evolutionary Algorithm in Duet
### 3.1 First hybrid algorithm: HEAD’
1. two parallel TabuCol algorithms which periodically interact by crossover; (2023年2月13日)
2. A major risk is stops sometimes too quickly. the greater the population size, the greater the search diversity. (2023年2月13日)

### 3.2 Improved hybrid algorithm: HEAD
1. Itercycle = 10, the number of generations into one
cycle. (2023年2月13日)
2. saveBest是找出这几个conflict最小的那个意思吗?
3. 








