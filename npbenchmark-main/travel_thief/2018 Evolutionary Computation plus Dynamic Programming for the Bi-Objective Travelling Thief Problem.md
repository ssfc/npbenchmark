# Evolutionary Computation plus Dynamic Programming for the Bi-Objective Travelling Thief Problem

(Prompt: 翻译本文标题)

进化计算与动态规划相结合的双目标旅行窃贼问题

https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2018%20Evolutionary%20Computation%20plus%20Dynamic%20Programming%20for%20the%20Bi-Objective%20Travelling%20Thief%20Problem.pdf

https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2018%20Evolutionary%20Computation%20plus%20Dynamic%20Programming%20for%20the%20Bi-Objective%20Travelling%20Thief%20Problem.md

## Abstract

(Prompt: generalize abstract of this article in Chinese)

这篇文章提出了一种新颖的基于指标的混合进化方法，它结合了近似算法和精确算法。我们将它应用于旅行窃贼问题的一个新的双目标形式，这是一个著名的多组件优化问题，它将两个经典的NP难问题：旅行商问题和0-1背包问题相互联系起来。我们的方法利用了精确的动态规划算法作为双目标进化算法中的一个子程序，来解决背包过程中旅行问题。这种设计利用了动态规划生成的帕累托前沿中提取的数据，来获得更好的解决方案。此外，我们还开发了一些新的指标和选择机制，来增强我们方法中两个算法组件的协同效果。计算实验的结果表明，我们的方法能够超越现有方法在单目标情况下的表现。

Keywords: indicator-based hybrid evolutionary approach that combines approximate and exact algorithms => Bi-Objective Travelling Thief Problem

### Q: 这篇文章发表会议/期刊属于CCF哪类？这篇文章在google scholar引用次数多少？

这篇文章是发表在GECCO 2018的会议论文，GECCO是遗传和进化计算领域的重要会议，根据CCF推荐国际学术会议和期刊目录-2022¹，GECCO属于CCF B类会议。根据Google Scholar²，这篇文章截至目前被引用了29次。

### Q: GECCO是什么会议？

GECCO是遗传和进化计算会议（Genetic and Evolutionary Computation Conference）的缩写，它是一个自1999年以来展示遗传和进化计算领域最新高质量成果的国际会议³。GECCO涵盖了遗传算法、遗传编程、蚁群优化和群智能、复杂系统、进化组合优化和元启发式、进化机器学习、进化多目标优化、进化数值优化、神经进化、实际应用、基于搜索的软件工程、理论、混合等主题⁴。GECCO每年举办一次，2023年将在葡萄牙里斯本以混合模式举行⁵。

### Q: Introduce authors of this article in Chinese one by one

这篇文章的作者是四位计算机科学领域的研究者，他们分别是：

- **吴俊华**，阿德莱德大学计算机科学学院的博士后研究员，主要研究方向是进化计算、组合优化和多目标优化¹。
- **Sergey Polyakovskiy**，迪肯大学信息技术学院的高级讲师，主要研究方向是进化计算、元启发式算法、多组件优化和实际应用²。
- **Markus Wagner**，阿德莱德大学计算机科学学院的副教授，主要研究方向是进化计算、优化、机器学习和复杂系统³。
- **Frank Neumann**，阿德莱德大学计算机科学学院的教授，主要研究方向是进化计算、理论分析、组合优化和生物信息学。

## 1 Introduction

(Prompt: Generalize Section "Introduction" of this article in Chinese)

这篇文章提出了一种新颖的指标为基础的混合进化算法，将近似算法和精确算法相结合，用于解决旅行商窃贼问题（TTP）的一个双目标版本。TTP是一个具有挑战性的多组件优化问题，将两个著名的NP难问题：旅行商问题（TSP）和0-1背包问题（KP）耦合在一起。该算法采用了精确的动态规划算法（DP）作为子程序，在一个双目标进化算法中处理背包问题。该算法利用了DP产生的Pareto前沿中的信息，来改进解的质量。此外，该算法还设计了一些新的指标和选择机制，来增强算法两个部分之间的协作效果。计算实验的结果显示，该算法能够在单目标TTP问题上优于现有方法。

## 2 The Travelling Thief Problem

(Prompt: Generalize Section "The Travelling Thief Problem" of this article in Chinese)

这篇文章介绍了旅行商窃贼问题（TTP）的标准单目标形式和扩展的双目标形式。TTP是一个双组件问题，涉及到n个城市，m个物品，和一个窃贼。窃贼必须做一个旅行路线，恰好访问每个城市一次。每个城市有一些物品，每个物品有一个利润和一个重量。窃贼可以在中间的城市收集任意数量的物品，直到超过背包的容量。选择一个物品会增加总利润，但也会增加运输成本，因为物品的重量会降低窃贼的速度。窃贼在每个时间单位内还要支付一个租金。问题的目标是确定一个旅行路线和一个物品子集，使得总利润和总运输成本之差最小化。

文章将TTP扩展为一个双目标优化问题，其中第二个目标是最小化总重量。这种扩展是自然的，因为在TTP中，人们可能需要在给定重量的情况下最大化利润，或者在给定利润的情况下最小化重量。注意到即使固定了旅行路线，TTP的目标函数也是一个非单调子模函数，这意味着选择更多的物品可能会降低总利润。文章将双目标TTP数学地定义为如下形式：

(π, ρ) =
arg max f(π, ρ)
arg min ϕ(ρ)
s.t. ϕ (ρ) ≤ C

其中π是一个旅行路线，ρ是一个物品选择方案，f是总利润和总运输成本之差，ϕ是总重量。作为一个双目标优化问题，双目标TTP要求找到一组Pareto最优解，即没有任何可行解能够在两个目标上同时改进的解。

## 3 Prerequisites

(Prompt: Generalize Section "Prerequisites" of this article in Chinese)

这篇文章介绍了混合进化算法的一些前提条件，包括动态规划算法和生成多个旅行路线的方法。**动态规划算法是用于解决背包问题的精确算法**，它可以在给定一个旅行路线的情况下，找到一组Pareto最优的物品选择方案，称为DP前沿。文章利用了DP前沿的完备性和多样性，来生成一系列可能的解。生成多个旅行路线的方法是用于解决旅行商问题的启发式算法，它们可以产生不同质量和多样性的路线。文章分析了五种旅行商问题的算法对TTP解的影响，发现Inver-over算法虽然在旅行商问题上不是最优的，但是在TTP上却能够产生更好的解。文章还利用这些算法来初始化混合进化算法的种群。

### Q: 动态规划是精确算法还是启发式算法？

**动态规划是一种精确算法**，通常用于解决最优化问题。动态规划算法通常具有以下两个特征：

1. 最优子结构：问题的最优解可以由其子问题的最优解递推得到。

2. 重叠子问题：问题的子问题存在重叠，即子问题之间共享某些计算结果。

动态规划算法通常通过建立状态转移方程来递归地解决问题。它在计算过程中会利用已经计算过的子问题的解，避免了重复计算，从而提高了算法的效率。由于动态规划算法是通过一定的数学原理进行推导的，因此可以保证得到问题的最优解。

## 4 A hybrid evolutionary approach

(Prompt: Generalize Section "A hybrid evolutionary approach" of this article in Chinese)
这篇文章提出了一种混合进化算法，将动态规划算法和指标为基础的进化算法相结合，用于解决双目标TTP问题。该算法的个体是旅行路线，而不是完整的TTP解，这样可以减少搜索空间和计算复杂度。该算法的核心是利用动态规划算法为每个旅行路线生成一个DP前沿，然后用指标函数来评估每个前沿对于整体Pareto最优解的贡献。文章设计了两种指标函数：损失表面贡献（Loss of Surface Contribution, LSC）和损失超体积（Loss of Hypervolume, LHV），分别衡量删除一个前沿后，整体Pareto最优解的表面和超体积的损失。文章还研究了八种不同的父代选择机制，来根据指标值选择优良的旅行路线进行交叉和变异操作。文章通过计算实验比较了不同指标和选择机制的组合，发现一些表现较好的组合，如RBS-HAR、AS-BST和FPS。

## 5 Computational Experiments

(Prompt: Generalize Section "Computational Experiments" of this article in Chinese)

## 6 Conclusion

(Prompt: Generalize Section "Conclusion" of this article in Chinese)