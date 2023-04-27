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

这篇文章提出了一种定制非支配排序基于偏随机键的遗传算法（customized non-dominated sorting based random-key genetic algorithm, NDS-BRKGA），它结合了两种经典的进化多目标优化方法：偏随机键遗传算法（sorting based random-key genetic algorithm, BRKGA）和非支配排序遗传算法II（NSGA-II）。这种算法针对BI-TTP的以下特点进行了定制：（i）利用已有的子问题求解器生成初始种群，结合子问题的近似最优解和随机解，保持种群的多样性；（ii）利用修复操作处理超过背包容量限制的不可行解，使算法只在可行解空间搜索；（iii）利用偏随机键编码处理混合变量类型，将排列和二进制向量映射为实数向量，便于应用传统的进化重组算子；（iv）利用非支配排序和拥挤距离进行生存选择，保证最终种群包含一组具有良好多样性的非支配解。文章还详细介绍了算法中各个步骤和参数的设计，并在不同的测试问题上进行了实验评估，证明了算法的有效性和稳健性。

## 5 Computational Experiments

(Prompt: Generalize Section "Computational Experiments" of this article in Chinese)

## 6 Concluding Remarks

(Prompt: Generalize Section "Concluding Remarks" of this article in Chinese)

