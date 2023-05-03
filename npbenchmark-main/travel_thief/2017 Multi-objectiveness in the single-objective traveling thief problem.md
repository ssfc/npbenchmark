# Multi-objectiveness in the single-objective traveling thief problem

(Prompt: 翻译本文标题)

单目标旅行窃贼问题中的多目标性

https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2017%20Multi-objectiveness%20in%20the%20single-objective%20traveling%20thief%20problem.pdf

https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2017%20Multi-objectiveness%20in%20the%20single-objective%20traveling%20thief%20problem.md

## Abstract

(Prompt: generalize abstract of this article in Chinese)

本文研究了多组件问题，即由多个相互影响的子问题组成的优化问题。本文以旅行窃贼问题（traveling thief problem, TTP）为例，将其作为一个双目标问题进行探讨。TTP是一个结合了旅行商问题（TSP）和背包问题（KP）的新型基准问题。本文的结果表明，双目标方法可以在与当前最先进的求解器竞争的同时，产生单目标TTP变体的解决方案。

Keywords: (Evolutionary Multi-Objective Algorithm, EMOA-TTP) => traveling thief problem

### Q: 这篇文章发表会议/期刊属于CCF哪类？这篇文章在google scholar引用次数多少？

根据CCF推荐国际学术会议和期刊目录-2022 ¹，这篇文章发表的会议GECCO（Genetic and Evolutionary Computation Conference）属于CCF C类会议。

根据Google Scholar ²，这篇文章截至2023年5月1日的引用次数为15次。

### Q: GECCO是什么会议？

GECCO是**遗传和进化计算会议**的缩写，它是遗传和进化计算领域最大的同行评审会议，也是ACM遗传和进化计算特别兴趣小组(SIGEVO)的主要会议¹。GECCO涵盖了遗传算法、遗传编程、蚁群优化和群智能、复杂系统、进化组合优化和元启发式、进化机器学习、进化多目标优化、进化数值优化等多个主题²。GECCO每年举办一次，2023年将在葡萄牙里斯本举行³。

### Q: Introduce authors of this article in Chinese one by one

- Mohamed El Yafrani，丹麦奥尔堡大学材料与生产系的博士后研究员，主要研究方向是优化算法设计、元启发式、进化计算、机器学习等，曾在GECCO、CEC等会议发表多篇论文 ¹²³。
- Shelvin Chand，澳大利亚新南威尔士大学的博士生，主要研究方向是多组件问题、旅行窃贼问题、进化计算等，曾在GECCO等会议发表论文 ¹⁴。
- Aneta Neumann，澳大利亚阿德莱德大学优化与物流系的博士后研究员，主要研究方向是进化计算、机器学习、神经网络等，曾在GECCO、PPSN等会议发表论文 ¹⁴。
- Belaïd Ahiod，摩洛哥拉巴特穆罕默德五世大学科学系的教授，主要研究方向是元启发式、进化计算、数据挖掘等，曾在GECCO、CEC等会议发表论文 ¹²。
- Markus Wagner，澳大利亚阿德莱德大学优化与物流系的副教授，主要研究方向是进化计算、多目标优化、旅行窃贼问题等，曾在GECCO、PPSN等会议发表论文，并担任过GECCO的程序委员会主席 ¹⁴。

## 1 MOTIVATION

(Prompt: Generalize Section "MOTIVATION" of this article in Chinese)

本文的动机是探讨在处理多个相互依赖的组件时，是否考虑多个目标会更好。为此，本文以旅行窃贼问题（TTP）为例，将其作为一个双目标问题进行研究。TTP是一个结合了旅行商问题（TSP）和背包问题（KP）的新型基准问题，模拟了多组件问题中子问题之间的相互影响。本文认为，单目标TTP模型是一个多目标问题的简单标量化，并且一个不考虑标量化的多目标方法应该能够产生覆盖各种相互依赖情况的解决方案。

## 2 PROPOSED APPROACH

(Prompt: Generalize Section "PROPOSED APPROACH" of this article in Chinese)

本文提出了一种基于NSGA-II框架的算法，称为EMOA-TTP，用于求解双目标TTP问题。该算法使用两种破坏性的变异算子和两种局部搜索启发式作为NSGA-II的变异算子。由于缺乏有效的交叉算子，该算法使用空交叉来简单地克隆选中的解。在每一代结束时，根据支配关系对解进行排序（非支配排序算子）。每个前沿中的解进一步根据其拥挤距离进行排序。基于这两个算子，选择下一代的解。

### Q: NSGA-II框架是什么？

NSGA-II (Non-dominated Sorting Genetic Algorithm II) 是一种经典的多目标优化算法，是基于遗传算法的一种改进。它在优化多目标问题方面表现出色，并已经被广泛应用于不同的领域。

NSGA-II 的核心思想是通过将种群按照非支配关系进行分层，从而实现多目标优化问题的求解。具体来说，NSGA-II 通过执行快速非支配排序，计算每个个体的拥挤度，以及采用选择、交叉和变异操作等策略，来实现种群的不断进化和优化。

相较于传统的遗传算法，NSGA-II 通过使用非支配排序和拥挤度计算方法，有效地维护了种群的多样性，从而可以在保持高质量解的同时，探索到更多的解空间。因此，NSGA-II 在多目标优化问题上的表现非常优秀，并已被广泛应用于许多领域，如工程设计、智能制造、自动化控制、机器学习等。

## 3 EXPERIMENTS & RESULTS

(Prompt: Generalize Section "EXPERIMENTS & RESULTS" of this article in Chinese)

本文对EMOA-TTP算法在TTP问题上的表现进行了实验评估，并与当前最先进的求解器进行了比较。本文使用了一种基于Pareto前沿的图形化方法来展示和分析双目标TTP问题的解决方案。本文发现，EMOA-TTP算法能够在时间和利润之间找到一组有效的权衡解，并且最优的TTP分数集中在Pareto前沿的膝部区域。本文还发现，单目标TTP模型中的最佳已知解也可以在EMOA-TTP算法产生的Pareto集区域中找到，甚至有时可以超越它们。这表明，多目标方法可以隐式地求解单目标TTP问题，同时提供更多的选择空间。

Keywords: (Evolutionary Multi-Objective Algorithm, EMOA-TTP) => traveling thief problem