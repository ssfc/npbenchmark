# A non-dominated sorting based customized random-key genetic algorithm for the bi-objective traveling thief problem

(Prompt: 翻译本文标题)

一种基于非支配排序的定制随机键遗传算法，用于解决双目标旅行窃贼问题

https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2021%20A%20non-dominated%20sorting%20based%20customized%20random-key%20genetic%20algorithm%20for%20the%20bi-objective%20traveling%20thief%20problem.pdf

https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2021%20A%20non-dominated%20sorting%20based%20customized%20random-key%20genetic%20algorithm%20for%20the%20bi-objective%20traveling%20thief%20problem.md

## Abstract

(Prompt: generalize abstract of this article in Chinese)

在本文中，我们提出了一种用于解决双目标旅行窃贼问题（bi-objective traveling thief problem, BI-TTP）的方法。BI-TTP是TTP的一个双目标版本，其中目标是最小化总旅行时间和最大化收集物品的利润。我们提出的方法基于一个带有定制随机键编码的偏向随机键遗传算法，该算法解决了问题的混合变量特性。我们通过进化算子结合领域知识，利用每个子问题的近似最优解作为初始种群，并使用定制的修复算子避免评估不可行解。问题的双目标方面通过基于非支配等级和拥挤距离提取的精英种群来处理。此外，我们提供了一个全面的研究，展示了每个参数对性能的影响。最后，我们讨论了EMO-2019和GECCO-2019会议上BI-TTP竞赛的结果，我们的方法分别获得了第一和第二名，证明了它能够一致地找到高质量的解。

Keywords: 基于一个带有偏向随机键编码的非支配排序遗传算法 (non-dominated sorting based customized random-key genetic algorithm, NDS-BRKGA) => 双目标旅行窃贼问题 (bi-objective traveling thief problem, BI-TTP)

### Q: 这篇文章发表会议/期刊属于CCF哪类？这篇文章在google scholar引用次数多少？

这篇文章发表在《启发式》（Journal of Heuristics）杂志上，这是一本CCF推荐的C类国际学术刊物³。这篇文章在谷歌学术上的引用次数是11次²。

### Q: Journal of Heuristics是什么期刊？

Journal of Heuristics是一本关于启发式方法的学术期刊，它提供了一个论坛，用于推进理论和实际应用方面的最新进展，解决那些无法精确求解的问题。它促进了启发式解决方案技术的发展、理解和实际应用，用于解决商业、工程和社会问题。它重视与启发式方法的发展相关的理论、实证和实验工作。¹² 这个期刊涵盖了基于人工智能技术的启发式搜索及其在人工智能规划、调度、设计、约束满足和博弈论等方面的应用。它包括了在人工智能中进行系统性和局部搜索的启发式和元启发式方法。³

### Q: Introduce authors of this article in Chinese one by one

这篇文章的作者有以下介绍：

- Jonatas B. C. Chagas是巴西联邦奥鲁普雷托大学计算机科学系的博士，也是巴西联邦维索萨大学信息学系的教授。他的研究兴趣包括组合优化、多目标优化、实际优化问题和启发式方法。他曾在多个国际期刊和会议上发表论文，也参与了多个优化竞赛。¹²
- Julian Blank是美国密歇根州立大学计算机科学与工程系的博士生，也是COIN实验室的成员。他的研究兴趣包括进化计算、多目标优化、实际优化问题和启发式方法。他曾在多个国际期刊和会议上发表论文，也参与了多个优化竞赛。²³
- Markus Wagner是澳大利亚阿德莱德大学计算机科学系的副教授，也是Optimisation and Logistics Research Group的成员。他的研究兴趣包括进化计算、元启发式、实际优化问题和启发式搜索。他曾在多个国际期刊和会议上发表论文，也参与了多个优化竞赛。² 
- Marcone J. F. Souza是巴西联邦奥鲁普雷托大学计算机科学系的教授，也是Operations Research and Computational Intelligence Laboratory (ORCILab)的负责人。他的研究兴趣包括组合优化、整数规划、元启发式、实际优化问题和运筹学。他曾在多个国际期刊和会议上发表论文，也参与了多个优化竞赛。² 
- Kalyanmoy Deb是美国密歇根州立大学计算机科学与工程系的教授，也是COIN实验室的主任。他是进化计算领域的知名专家，曾获得过多个国际奖项和荣誉，如IEEE Kiyo Tomiyama Award, IEEE CIS Evolutionary Computation Pioneer Award等。他的研究兴趣包括进化计算、多目标优化、实际优化问题和启发式方法。他曾在多个国际期刊和会议上发表论文，也参与了多个优化竞赛。²

## 1 Introduction

(Prompt: Generalize Section "Introduction" of this article in Chinese)

这篇文章提出了一种用于解决双目标旅行窃贼问题（bi-objective traveling thief problem, BI-TTP）的方法。BI-TTP是一个组合优化问题，由两个经典的子问题组成：旅行商问题（TSP）和背包问题（KP）。这两个子问题之间存在相互影响，使得优化难度增加。文章的方法基于一个带有偏向随机键编码的非支配排序遗传算法（non-dominated sorting based customized random-key genetic algorithm, NDS-BRKGA），结合了针对问题特征的定制化操作。文章利用了各个子问题的现有求解器来生成初始种群，并使用了一个修复算子来避免不可行解的评估。文章还通过非支配排序和拥挤距离来处理问题的双目标性质，并提供了一个全面的参数影响分析。最后，文章讨论了在EMO-2019和GECCO-2019会议上举办的BI-TTP竞赛的结果，证明了该方法能够一致地找到高质量的解决方案。

## 2 Related Work

(Prompt: Generalize Section "Related Work" of this article in Chinese)

这篇文章回顾了旅行窃贼问题（TTP）的研究进展，主要关注了单目标和双目标的TTP变体。文章介绍了TTP的定义和特点，以及它与旅行商问题（TSP）和背包问题（KP）的相互影响。文章总结了针对**单目标TTP**的各种方法，包括迭代搜索启发式 (iterative search-heuristics)、协同进化 (co-evolutionary)、模拟进化 (memetic approaches)、群智能 (swarm-intelligence based) 和问题特定的搜索算子等。文章也讨论了少数针对多目标TTP的方法，包括使用非支配排序和拥挤距离的方法，以及考虑动态变化的方法。文章还提到了一些更一般的关于多组件问题和相互依赖问题的讨论。文章指出，尽管TSP和KP都是经典的组合优化问题，但是它们的相互作用使得TTP成为一个具有挑战性的问题。

## 3 Bi-objective Traveling Thief Problem

(Prompt: Generalize Section "Bi-objective Traveling Thief Problem" of this article in Chinese)

这篇文章介绍了双目标旅行窃贼问题（bi-objective traveling thief problem, BI-TTP），它是一个组合优化问题，由两个相互影响的子问题组成：旅行商问题（TSP）和背包问题（KP）。文章描述了TSP和KP的独立定义，以及它们在TTP中的相互作用。文章说明了TTP的目标是最小化窃贼的总旅行时间和最大化收集物品的利润，这两个目标是冲突的。文章还给出了一个例子，展示了如何计算TTP的目标值和如何确定非支配解集。文章指出，尽管TSP和KP都是经典的组合优化问题，但是它们的相互作用使得TTP成为一个具有挑战性的问题。

## 4 A Customized Non-dominated Sorting Based Genetic Algorithm with Biased Random-Key Encoding

(Prompt: Generalize Section "A Customized Non-dominated Sorting Based Genetic Algorithm with Biased Random-Key Encoding" of this article in Chinese)

根据当前网页内容，这篇文章的定制非支配排序基于偏随机键的遗传算法部分的中文概括如下：

这篇文章提出了一种定制非支配排序基于偏随机键的遗传算法（customized non-dominated sorting based random-key genetic algorithm, NDS-BRKGA），它结合了两种经典的进化多目标优化方法：偏随机键遗传算法（Biased Random Key Genetic Algorithm, BRKGA）和非支配排序遗传算法II（NSGA-II）。这种算法针对BI-TTP的以下特点进行了定制：（i）利用已有的子问题求解器生成初始种群，结合子问题的近似最优解和随机解，保持种群的多样性；（ii）利用修复操作处理超过背包容量限制的不可行解，使算法只在可行解空间搜索；（iii）利用偏随机键编码处理混合变量类型，将排列和二进制向量映射为实数向量，便于应用传统的进化重组算子；（iv）利用非支配排序和拥挤距离进行生存选择，保证最终种群包含一组具有良好多样性的非支配解。文章还详细介绍了算法中各个步骤和参数的设计，并在不同的测试问题上进行了实验评估，证明了算法的有效性和稳健性。

### Q: 什么是偏随机键遗传算法（Biased Random Key Genetic Algorithm, BRKGA）？

偏随机键遗传算法（Biased Random Key Genetic Algorithm, BRKGA）是一种元启发式算法，用于解决优化问题。该算法的主要思想是将决策变量表示为二进制编码的随机键，通过遗传算法的方式进行搜索。

在BRKGA中，每个个体都由一个随机键向量表示，其中每个随机键对应于一个决策变量。随机键向量中的每个元素都是在0到1之间均匀分布的随机数。这些随机键被用于构建解决方案，并通过适应度函数进行评估。

BRKGA使用两个种群：主种群和次种群。主种群是由随机生成的个体组成的，而次种群是由主种群中适应度较高的个体组成的。次种群的作用是增加搜索空间中的多样性，以便更好地避免局部最优解。

BRKGA还使用了一种偏置策略，以加速收敛速度。偏置策略可以通过增加次种群中适应度较低的个体的概率来实现。这种策略会使次种群中更多的个体参与到下一代的生成中，从而增加了搜索空间的多样性。

总的来说，BRKGA是一种高效的元启发式算法，可用于解决各种类型的优化问题。

## 5 Computational Experiments

(Prompt: Generalize Section "Computational Experiments" of this article in Chinese)

本节报告了作者对所提出的算法在不同测试问题上的实验结果。作者使用了两个测试集，一个是从TSP库中选择的经典TSP问题，另一个是从BI-TTP竞赛中选择的新问题。作者使用了一些性能指标，如**超体积**、**分布指标**和**覆盖率**，来评估算法的有效性和稳健性，并与其他参与竞赛的算法进行了比较。作者还对算法的各个参数进行了敏感性分析，探讨了它们对解决方案质量和多样性的影响。实验结果表明，作者的算法在大多数情况下都能够产生具有高超体积和良好分布的解决方案，并且在两次竞赛中都取得了优异的成绩。

## 6 Concluding Remarks

(Prompt: Generalize Section "Concluding Remarks" of this article in Chinese)

本节总结了本文的主要贡献，并提出了一些未来的研究方向。作者指出，他们提出的非支配排序带有偏随机键编码的遗传算法是一种有效且稳健的方法，可以解决具有相互作用组件的双目标优化问题，如双目标旅行窃贼问题 (bi-objective traveling thief problem, BI-TTP)。作者还展示了该算法在两次国际竞赛中的优异表现，并分析了不同参数和算子对算法性能的影响。作者认为，该算法可以推广到其他具有相互作用组件的优化问题，并为未来的研究提出了一些方向，如考虑更复杂的问题变体、设计更有效的局部搜索策略、探索多目标优化的潜力等。

Keywords: 基于一个带有偏向随机键编码的非支配排序遗传算法 (non-dominated sorting based customized random-key genetic algorithm, NDS-BRKGA) => 双目标旅行窃贼问题 (bi-objective traveling thief problem, BI-TTP)
