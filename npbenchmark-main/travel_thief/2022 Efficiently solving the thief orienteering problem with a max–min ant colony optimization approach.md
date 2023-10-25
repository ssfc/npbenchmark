# Efficiently solving the thief orienteering problem with a max-min ant colony optimization approach

(Prompt: 翻译本文标题)

利用最大-最小蚁群优化方法高效地解决盗贼定向问题

https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2022%20Efficiently%20solving%20the%20thief%20orienteering%20problem%20with%20a%20max%E2%80%93min%20ant%20colony%20optimization%20approach.pdf

https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2022%20Efficiently%20solving%20the%20thief%20orienteering%20problem%20with%20a%20max%E2%80%93min%20ant%20colony%20optimization%20approach.md

Code: https://github.com/jonatasbcchagas/acoplusplus_thop

## Abstract

(Prompt: generalize abstract of this article in Chinese)

我们研究了盗贼定向问题（Thief Orienteering Problem, ThOP），这是一个结合了背包问题和定向问题两个经典组合优化问题的学术多组分问题。在ThOP中，盗贼有一个时间限制，要从一组城市中偷取物品。盗贼在旅途中将物品放入背包，这会降低他的行走速度。盗贼的目标是最大化偷取物品的总利润。在本文中，我们提出了一种将群智能和随机装箱启发式相结合的方法。我们的解决方案在几乎所有的432个基准实例上都优于现有的工作，有显著的改进。

Keywords:  ACO++ => (Thief Orienteering Problem, ThOP)

### Q: Thief Orienteering Problem是不是thief traveling problem? 如果不是有什么区别？

Thief Orienteering Problem (ThOP)和Thief Traveling Problem (TTP)是两个不同的组合优化问题，它们都涉及一个小偷在一组城市中旅行和偷窃物品的情况。它们的主要区别是：

- 在ThOP中，小偷有一个时间限制，必须在规定的时间内从起点城市到达终点城市。小偷**不需要访问所有的城市**，只能从访问过的城市中选择物品。小偷的目标是最大化偷窃物品的总利润。
- 在TTP中，小偷**必须访问所有的城市**，并且可以从任何城市中选择物品。小偷租用了一个背包，每单位时间都要支付一定的费用。小偷的目标是最大化偷窃物品的总利润减去背包租金和旅行时间的乘积。

### Q: 这篇文章发表会议/期刊属于CCF哪类？这篇文章在google scholar引用次数多少？

这篇文章发表在Optimization Letters这个期刊上²，这是一个关于优化理论和应用的国际期刊。根据CCF推荐国际学术刊物目录¹，这个期刊属于计算机科学理论领域的C类期刊。根据Google Scholar³，这篇文章目前有2次引用。

### Q: Optimization Letters是什么期刊？

Optimization Letters不是一个会议，而是一个期刊。根据网页内容，这是一个关于优化理论和应用的国际期刊，发表了一些关于组合优化、非线性优化、随机优化等方面的研究文章。这个期刊属于计算机科学理论领域的C类期刊，根据CCF推荐国际学术刊物目录。

### Q: Introduce authors of this article in Chinese one by one

这篇文章的作者有两位，分别是：

- Jonatas B. C. Chagas¹，他是巴西奥罗普雷托联邦大学计算机科学系的博士，主要研究方向是组合优化、整数规划和元启发式算法。他曾经在国际期刊和会议上发表过多篇关于优化问题的文章。
- Markus Wagner²，他是澳大利亚阿德莱德大学计算机科学学院的副教授，主要研究方向是进化计算、多目标优化和多组件问题。他曾经在国际期刊和会议上发表过多篇关于优化问题的文章，并担任过多个国际会议的程序委员会主席或成员。

## 1 Introduction

(Prompt: Generalize Section "Introduction" of this article in Chinese)

这篇文章研究了一种叫做Thief Orienteering Problem (ThOP)的学术多组件问题，它结合了两个经典的组合优化问题，即背包问题和定向越野问题。在ThOP中，一个小偷有一个**时间限制**，在一组城市中偷取分布在城市中的物品。小偷在旅行的过程中收集物品并存放在背包中，这会降低小偷的旅行速度。小偷的目标是在不超过背包容量和时间限制的情况下，**最大化偷取物品的总利润**。文章介绍了一种基于群智能和随机装箱启发式算法的解决方案，该方案在几乎所有的432个测试实例上都优于现有的工作，并且有显著的改进。

## 2 Problem definition

(Prompt: Generalize Section "Problem definition" of this article in Chinese)

这篇文章首先给出了ThOP的正式定义，介绍了其中涉及的一些符号和变量，例如城市集合、物品集合、背包容量、最大旅行时间、物品的利润和重量、小偷的速度等。然后，文章给出了一个混合整数非线性规划（MINLP）的数学模型，用来描述ThOP的目标函数和约束条件。文章还给出了一种将非线性约束转化为线性约束的方法，以便于使用现有的求解器来求解ThOP。

## 3 Problem-solving methodology

(Prompt: Generalize Section "Problem-solving methodology" of this article in Chinese)

这篇文章提出了一种基于蚁群优化（ACO）的算法来求解ThOP，称为ACO++。该算法分为两个阶段：第一阶段，**使用ACO来构造小偷的路线**，即访问城市的顺序；第二阶段，**使用一个随机装箱启发式算法来确定小偷在每个城市选择哪些物品**。文章还介绍了与之前提出的ACO算法相比，ACO++所做的两个改进：一是让蚂蚁构造的路线不必访问所有城市，而是只访问有利润的城市；二是在每个路线上应用不同的局部搜索算法，以缩短路线的距离。

## 4 Computational study

(Prompt: Generalize Section "Computational study" of this article in Chinese)

这篇文章使用了432个ThOP的测试实例来评估不同算法的性能，这些实例是基于TTP的实例生成的，具有不同的城市数量、物品数量、背包大小、物品类型和最大旅行时间。文章使用了**Irace软件包**来自动调整ACO++和其他基于ACO和BRKGA的算法的参数，并分析了参数对算法性能的影响。文章还比较了ACO++和其他已有的算法（ILS，BRKGA，GA和ACO）在测试实例上的解质量，使用了近似比和统计检验等指标。结果表明，ACO++在几乎所有的测试实例上都能找到最优或接近最优的解，而且比其他算法有显著的优势。

### Q: Irace软件包是什么？

Irace软件包是一个用于自动配置优化算法的工具，它可以根据一组优化问题的实例，找到最合适的算法参数设置。它是基于R语言实现的，使用了迭代竞赛（Iterated Race）方法，这是迭代F-竞赛（Iterated F-race）方法的一个推广。它可以用于配置其他类型的算法，只要它们的性能依赖于参数设置。您可以从CRAN网站下载和安装Irace软件包，并查看相关的文献和教程。

## 5 Conclusions

(Prompt: Generalize Section "Conclusions" of this article in Chinese)

本文提出了一种基于蚁群优化和随机装箱启发式的方法，来解决盗贼定向问题（ThOP）。ThOP是一种学术性的多组件问题，它结合了两个经典的组合优化问题，即背包问题（KP）和定向问题（OP）。在ThOP中，一个盗贼有一个时间限制，要从一组城市中偷取物品。在旅行过程中，盗贼将物品存放在背包中，这反过来降低了旅行速度。盗贼的目标是最大化偷取物品的总利润。本文的方法在432个基准实例中几乎都优于现有的方法，并且有显著的改进。

Keywords:  ACO++ => (Thief Orienteering Problem, ThOP)
