# Exact Approaches for the Travelling Thief Problem

(Prompt: 翻译本文标题)

精确方法解决旅行商偷窃问题

https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2017%20Exact%20Approaches%20for%20the%20Travelling%20Thief%20Problem.pdf

https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2017%20Exact%20Approaches%20for%20the%20Travelling%20Thief%20Problem.md

## Abstract

(Prompt: generalize abstract of this article in Chinese)

本文介绍了三种精确算法和一种混合方法，用于解决旅行商偷窃问题（TTP）。TTP是一个结合了旅行商问题（TSP）和0-1背包问题（KP）的组合优化问题。目前，已有许多进化算法和启发式算法用于求解TTP，但由于它们无法找到全局最优解，因此它们的精度是未知的。本文通过与现有的最先进方法进行比较，对小规模的TTP实例进行了全面的精度分析。

Keywords: 三种精确算法 (Dynamic Programming, Branch and Bound Search, Constraint Programming) 和一个混合方法 => Travelling Thief Problem

### Q: 这篇文章发表会议/期刊属于CCF哪类？这篇文章在google scholar引用次数多少？

ArXiv. Cited by 36 times. 

### Q: 是什么期刊？

ArXiv. 

### Q: Introduce authors of this article in Chinese one by one

这篇文章的作者如下：

- **吴俊华**，阿德莱德大学计算机科学学院的博士研究生，主要研究方向是组合优化、进化计算和混合算法。他在国际会议和期刊上发表了多篇论文，包括《精确方法求解旅行商窃贼问题》和《进化计算加动态规划求解双目标旅行商窃贼问题》等。
- **Markus Wagner**，阿德莱德大学计算机科学学院的副教授，主要研究方向是进化计算、多目标优化和实际问题的求解。他是国际进化计算协会的副主席，也是多个国际会议和期刊的编委或审稿人。他在国际会议和期刊上发表了超过100篇论文，包括《旅行商窃贼问题的进化算法》和《旅行商窃贼问题的蚁群系统》等。
- **Sergey Polyakovskiy**，迪肯大学信息技术学院的博士后研究员，主要研究方向是组合优化、元启发式算法和多组件优化问题。他是旅行商窃贼问题的提出者之一，也是多个国际会议和期刊的审稿人。他在国际会议和期刊上发表了多篇论文，包括《旅行商窃贼问题：基准实例、性能分析和近似算法》和《双目标旅行商窃贼问题的进化计算加动态规划》等。
- **Frank Neumann**，阿德莱德大学计算机科学学院的教授，主要研究方向是进化计算、理论分析和复杂系统。他是国际进化计算协会的前任主席，也是多个国际会议和期刊的编委或审稿人。他在国际会议和期刊上发表了超过200篇论文，包括《旅行商窃贼问题：一个新颖的组合优化问题》和《动态规划求解背包边旅行问题》等。

## 1 Introduction

(Prompt: Generalize Section "Introduction" of this article in Chinese)

本文介绍了一种新的学术问题，即旅行窃贼问题（Travelling Thief Problem, TTP），它是旅行商问题（TSP）和0-1背包问题（KP）的结合。它反映了现实应用中存在多个NP难问题的复杂性，例如规划、调度和路由等领域。本文提出了三种精确算法和一个混合方法来求解TTP，并与现有的启发式方法进行了全面的比较，以评估它们在小型TTP实例上的准确性。

## 2 Problem Statement

(Prompt: Generalize Section "Problem Statement" of this article in Chinese)

本文阐述了旅行窃贼问题（Travelling Thief Problem, TTP）的定义和数学模型。TTP是一个组合优化问题，涉及到一位小偷在一组城市中旅行，同时选择一些物品放入背包中，以最大化自己的收益。TTP的难点在于旅行和背包两个子问题之间的相互影响：旅行的路线会影响背包的重量，而背包的重量会影响旅行的速度和成本。TTP是一个NP难问题，因此需要有效的算法来求解。本文的目标是找到一种精确算法或混合方法来求解TTP，并与现有的启发式方法进行比较。

## 3 Exact Approaches to the TTP

(Prompt: Generalize Section "Exact Approaches to the TTP" of this article in Chinese)

本文提出了三种精确算法和一种混合方法来求解旅行窃贼问题（Travelling Thief Problem, TTP）。TTP是一个组合优化问题，涉及到旅行商问题（TSP）和0-1背包问题（KP）的相互作用。TTP的求解难度很高，因为目前没有已知的能够找到全局最优解的算法。本文的精确算法分别基于动态规划（Dynamic Programming, DP）、分支定界（Branch and Bound Search, BnB）和约束编程（Constraint Programming, CP）的思想。动态规划算法利用了一种简化版本的TTP，即边旅行边装包问题（PWT），并结合了Held-Karp算法来求解TSP。分支定界算法采用了一种上界估计来剪枝搜索空间，并利用预先计算的最短路径来改进上界估计。约束编程算法使用了一种基于排列的表示方法来描述旅行路线，并使用了AllDifferent约束来消除子环。本文还提出了一种混合方法，将动态规划算法与启发式方法相结合，以提高启发式方法的精度。本文通过在一系列小规模的实例上比较这些精确算法和现有的启发式方法，对它们的性能进行了全面的评估。

### Q: 什么是分支定界（Branch and Bound Search, BnB）法？

分支定界（Branch and Bound, BnB）是一种求解最优化问题的算法。它通过不断地将问题分解成子问题，并对每个子问题进行优化，从而找到全局最优解。具体来说，BnB算法通过将问题的解空间划分为多个子空间，然后在每个子空间上使用一个界函数（bounding function）来估计每个可能的解的质量，以便确定哪些子空间不可能包含最优解。这些无用的子空间会被排除掉，而对于可能包含最优解的子空间，则继续分解，直到找到全局最优解为止。

BnB算法的主要优点是能够在相对较短的时间内找到最优解，尤其是对于组合优化问题和规划问题。缺点是，由于需要对解空间进行划分和计算界函数，BnB算法的时间复杂度较高，因此不适用于大规模的问题。

### Q: 举一个分支定界（Branch and Bound Search, BnB）的例子？

假设我们有一个0/1背包问题，即给定一组物品，每个物品有一个重量和一个价值，需要选择一些物品装入背包中，使得背包不超过其容量，并且价值最大化。我们可以使用分支定界算法解决这个问题。

具体的分支定界过程如下：

1. 初始化一个根节点，表示空背包。
2. 计算根节点的上界，即背包能够装下的最大价值。
3. 将根节点加入到待处理节点列表中。
4. 对于待处理节点列表中的每个节点： a. 如果该节点已经达到了最大深度或已经不可能找到更优解，则将其剪枝。 b. 否则，根据某种规则，选择一个物品将其加入背包中，生成两个子节点，分别表示选择该物品和不选择该物品两种情况，并计算子节点的上界。 c. 将生成的子节点加入到待处理节点列表中。
5. 重复步骤4，直到待处理节点列表为空或者所有节点的上界都小于等于当前最优解。

## 4 Computational Experiments

(Prompt: Generalize Section "Computational Experiments" of this article in Chinese)

本文在一系列小规模的实例上对三种精确算法和一种混合方法进行了计算实验，比较了它们的运行时间和精度。实验使用了Phoenix HPC服务提供的超级计算资源，为每个实验分配了一个CPU核和32GB的内存。实验中生成了一些额外的测试实例，遵循了文献[17]中的方法，从原始的TSP库中选择了一个名为eil51的实例作为起点，随机删除了一些城市，得到了不同规模的TTP实例。背包问题的参数也按照文献[15]中的方法进行了设置，包括物品数量、背包容量和物品利润与重量之间的相关性。实验结果表明，动态规划算法在小规模实例上表现最好，能够在24小时的时间限制内求解出最优解。分支定界算法和约束编程算法在某些情况下也能够求解出最优解，但是运行时间较长。混合方法将动态规划算法与启发式方法相结合，能够提高启发式方法的精度，但是仍然存在一定的误差。本文还利用动态规划算法得到的最优解，对现有的启发式方法进行了评估，发现它们在大多数情况下能够得到接近最优或者最优的解，但是在少数情况下与最优解有超过10%的差距。

## 5 Conclusion

(Prompt: Generalize Section "Conclusion" of this article in Chinese)

本文介绍和评估了基于动态规划、分支定界和约束编程的TTP的精确方法。我们利用动态规划方法得到的最优解，对当前的启发式TTP求解器进行了评估。我们的研究表明，它们在大多数情况下能够得到接近最优或者最优的解，但是在少数情况下与最优解有超过10%的差距。这些结果为未来的研究提供了一个基准，也揭示了TTP的复杂性和挑战性。

Keywords: 三种精确算法 (Dynamic Programming, Branch and Bound Search, Constraint Programming) 和一个混合方法 => Travelling Thief Problem