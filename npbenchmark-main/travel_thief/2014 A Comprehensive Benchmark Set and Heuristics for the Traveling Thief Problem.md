# A Comprehensive Benchmark Set and Heuristics for the Traveling Thief Problem

(Prompt: 翻译本文标题)

旅行窃贼问题的综合基准集和启发式算法

https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2014%20A%20Comprehensive%20Benchmark%20Set%20and%20Heuristics%20for%20the%20Traveling%20Thief%20Problem.pdf

https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2014%20A%20Comprehensive%20Benchmark%20Set%20and%20Heuristics%20for%20the%20Traveling%20Thief%20Problem.md

https://cs.adelaide.edu.au/~optlog/research/combinatorial.php

## Abstract

(Prompt: generalize abstract of this article in Chinese)

实际优化问题通常由多个相互作用的NP难问题组成。本文的目的是提供一个基准集，促进对问题之间的相互作用和相互影响的研究。我们建立了一个综合的旅行窃贼问题 (Traveling Thief Problem, TTP) 基准集，它结合了旅行商问题和背包问题。我们的基准集基于两个子问题的常用基准，为检验两个经典问题结合所带来的潜在难度提供了基础。此外，我们还提出了一些简单的TTP启发式算法，并给出了它们在我们的基准集上的结果。

Keywords: Traveling Thief Problem benchmark; Simple Heuristic, random local search, 1+1 evolutionary; 

Comment: 为TTP提供了数据集

### Q: 这篇文章发表会议/期刊属于CCF哪类？这篇文章在google scholar引用次数多少？

这篇文章发表在GECCO 2014会议上，GECCO是遗传和进化计算领域的国际会议，根据CCF推荐国际学术会议目录¹，GECCO属于C类会议。根据Google Scholar²，这篇文章截至目前被引用了119次。

### Q: GECCO是什么会议？

(The Genetic and Evolutionary Computation Conference, GECCO) 是遗传和进化计算会议的缩写，是遗传和进化计算领域的国际会议，每年举办一次，自1999年以来一直展示了遗传和进化计算领域的最新高质量成果。会议涵盖了遗传算法、遗传编程、蚁群优化和群智能、复杂系统、进化组合优化和元启发式、进化机器学习、进化多目标优化、进化数值优化等多个主题³。

### Q: 用中文依次介绍本文作者

A: 根据网页正文，本文的作者有：

- Sergey Polyakovskiy，澳大利亚莫纳什大学的研究员。他的研究方向是进化计算和组合优化。
- Mohammad Reza Bonyadi，澳大利亚阿德莱德大学的研究员。他的研究方向是进化算法和复杂问题。
- Markus Wagner，澳大利亚阿德莱德大学的高级讲师。他的研究方向是进化计算和优化。
- Zbigniew Michalewicz，澳大利亚阿德莱德大学的教授。他的研究方向是人工智能和优化。
- Frank Neumann，澳大利亚阿德莱德大学的教授。他的研究方向是理论计算机科学和进化计算。

## 1. INTRODUCTION

(Prompt: Generalize Section "INTRODUCTION" of this article in Chinese)

本文的引言部分主要介绍了旅行小偷问题（Traveling Thief Problem, TTP）的定义和研究背景。TTP是一个将旅行商问题（TSP）和背包问题（KP）结合起来的复杂优化问题，涉及到两个子问题之间的相互作用和影响。作者提出了一个基于TSP和KP的公认基准实例的TTP基准集，旨在为研究者提供一个探索TTP难度和特征的平台。此外，作者还提出了一些简单的启发式算法，并在基准集上进行了实验评估。

## 2. TRAVELING THIEF PROBLEM

(Prompt: Generalize Section "TRAVELING THIEF PROBLEM" of this article in Chinese)

本文的第二节给出了旅行小偷问题的数学定义和一个数值例子。TTP的输入包括一组城市和它们之间的距离，每个城市（除了第一个）有一些物品，每个物品有一个价值和一个重量。小偷必须从第一个城市出发，按照一定的顺序访问每个城市一次，然后回到第一个城市。小偷可以在任何城市拿走一些物品，但是不能超过背包的最大容量。小偷在路上的**速度**取决于背包的**重量**，速度越慢，花费的时间越长。小偷需要为背包的租用支付一定的费用，**费用**与**时间**成正比。TTP的目标是找到一条**最大利润**的路线和装载方案，即拿走物品的总价值减去背包租用的总费用。

## 3. BENCHMARK SET

(Prompt: Generalize Section "BENCHMARK SET" of this article in Chinese)

本文的第三节介绍了作者构建的TTP基准集的方法和特点。作者**从TSP库中选择了81个**具有不同城市数量和距离类型的实例，作为TTP的路线部分。然后，作者根据Martello等人提出的三种背包问题类型，分别为无关联、无关联相似重量和有界强相关，**为每个TSP实例生成了背包**部分。作者还考虑了每个城市的物品数量（F）和背包容量类别（C）两个因素，使得TTP基准集能够覆盖不同的难度和特征。最后，作者得到了9720个TTP实例，每个实例都有一个唯一的标识符，方便研究者进行比较和分析。

## 4. HEURISTIC ALGORITHMS

(Prompt: Generalize Section "HEURISTIC ALGORITHMS" of this article in Chinese)

本文的第四节提出了三种启发式算法来求解TTP问题，分别是单纯贪心算法（Simple Heuristic, SH）、随机局部搜索算法（random local search, RLS）和（1+1）进化算法（evolutionary algorithm, EA）。这三种算法都是基于固定的TSP路线来优化背包装载方案的，即假设TSP路线已经由其他高效的算法给出。**SH算法**是一种简单的贪心策略，它按照物品的单位价值从高到低的顺序来选择物品，直到背包装满或者没有更多的物品可选。**RLS算法**是一种局部搜索策略，它从一个空背包开始，每次随机选择一个物品改变其装载状态，如果得到了更好的解就接受，否则就拒绝。**EA算法**是一种进化策略，它也从一个空背包开始，每次对每个物品以一定的概率改变其装载状态，产生一个新的解，如果新解比旧解更好或者相等就替换旧解。

## 5. EXPERIMENTS

(Prompt: Generalize Section "EXPERIMENTS" of this article in Chinese)

本文的第五节报告了作者在TTP基准集上进行的实验结果和分析。作者使用了Chained Lin-Kernighan算法来为每个TTP实例生成一个高质量的TSP路线，然后使用SH、RLS和EA三种启发式算法来优化背包装载方案。作者比较了不同算法在不同类型、规模和难度的TTP实例上的表现，包括目标值、运行时间和标准差等指标。作者发现，SH算法虽然速度很快，但是目标值往往较低；RLS和EA算法虽然能够得到更好的目标值，但是运行时间较长，并且对于大规模的TTP实例，它们很难找到全局最优解。作者还使用了决策树来分析不同算法在不同条件下的相对优势，发现TTP实例的特征对算法的选择有很大的影响。

## 6. CONCLUSIONS

(Prompt: Generalize Section "CONCLUSIONS" of this article in Chinese)

本文的最后一节总结了作者的主要贡献和未来的研究方向。作者提出了一个系统的TTP基准集 (Traveling Thief Problem benchmark)，基于TSP和KP的经典实例，覆盖了不同的问题特征和难度。作者的目标是促进研究者对TTP问题的相互作用和影响的理解和处理。作者还提出了三种启发式算法 (Simple Heuristic, random local search, 1+1 evolutionary) 来有效地求解TTP问题，并在基准集上进行了实验评估。作者发现，一个好的初始解和一个能够逃离局部最优的迭代搜索都是很重要的。作者计划在未来对TTP问题进行更深入的理论和实验分析，识别导致问题难易的特征，基于问题的实际属性设计专门的算法，探索问题组件之间的内在规律和相互影响。

Keywords: Traveling Thief Problem benchmark; Simple Heuristic, random local search, 1+1 evolutionary; 