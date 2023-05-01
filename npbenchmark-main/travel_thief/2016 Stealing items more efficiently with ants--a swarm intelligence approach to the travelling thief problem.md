# Stealing items more efficiently with ants: a swarm intelligence approach to the travelling thief problem

(Prompt: 翻译本文标题)

用蚂蚁更高效地偷东西：一种基于群体智能的旅行小偷问题的解决方法

https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2016%20Stealing%20items%20more%20efficiently%20with%20ants--a%20swarm%20intelligence%20approach%20to%20the%20travelling%20thief%20problem.pdf

https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2017%20Exact%20Approaches%20for%20the%20Travelling%20Thief%20Problem.md

## Abstract

(Prompt: generalize abstract of this article in Chinese)

**本文介绍了一种基于蚁群优化的群体智能方法，用于解决旅行商偷窃问题（TTP）。** TTP是一种组合优化问题，将旅行商问题（TSP）和背包问题（KP）相结合，两者之间存在相互作用。本文的方法利用蚂蚁构建不同的TSP路径，并为每条路径使用启发式算法生成一个KP方案，从而得到一个TTP解。然后，使用局部搜索算法对最优解进行进一步改进。本文通过实验表明，该方法在多达250个城市和2000个物品的实例上优于现有的方法，有时可以提高10%以上。**本文探讨了TTP的难度特征，并提出了未来的研究方向。**

Keywords: swarm intelligence based on ant colony optimization => travelling thief problem

### Q: 这篇文章发表会议/期刊属于CCF哪类？这篇文章在google scholar引用次数多少？

- 这篇文章发表在ACM Genetic and Evolutionary Computation Conference (GECCO) 2016上，这是进化计算领域最好的会议之一，但国内认可度不高，CCF的排名给了C¹。
- 这篇文章在google scholar上的引用次数为48。

### Q: GECCO是什么会议？

GECCO是**遗传和进化计算会议**的缩写，它是遗传和进化计算领域最大的同行评审会议，也是ACM遗传和进化计算特别兴趣小组(SIGEVO)的主要会议¹。GECCO涵盖了遗传算法、遗传编程、蚁群优化和群智能、复杂系统、进化组合优化和元启发式、进化机器学习、进化多目标优化、进化数值优化等多个主题²。GECCO每年举办一次，2023年将在葡萄牙里斯本举行³。

### Q: Introduce authors of this article in Chinese one by one

**Markus Wagner**，他是阿德莱德大学计算机科学学院的高级讲师。他的研究兴趣包括进化计算、优化、搜索、元启发式、超启发式、进化机器人、进化艺术和创造力。他曾担任过多个国际会议和期刊的程序委员会成员、主席或编辑³。

## 1 Introduction

(Prompt: Generalize Section "Introduction" of this article in Chinese)

这篇文章的“简介”部分介绍了旅行窃贼问题（TTP）的定义和相关研究，以及本文的主要贡献和目的。TTP是一个结合了旅行商问题（TSP）和背包问题（KP）的组合优化问题，其中两个子问题之间存在相互影响。本文提出了一种基于蚁群优化（ACO）的群智能方法来解决TTP的旅行部分，然后利用一种快速有效的启发式算法来构造每个旅行方案的背包计划。本文通过实验研究表明，该方法在多达250个城市和2000个物品的TTP实例上优于现有方法，有时甚至超过10%。本文还讨论了TTP的难度特征和未来的研究方向。

## 2 Traveling Thief Problem

(Prompt: Generalize Section "Traveling Thief Problem" of this article in Chinese)

这篇文章的“旅行窃贼问题”部分详细描述了TTP的定义和数学模型。TTP是一个将TSP和KP两个NP难问题相互联系的组合优化问题。TTP的目标是为一个窃贼提供一个旅行路线和一个背包计划，使得窃贼在访问所有给定城市的同时，能够从每个城市中选择一些物品，最大化自己的利润。TTP的难点在于两个子问题之间的相互影响：选择更多的物品会增加背包的重量，从而降低窃贼的速度和增加旅行时间，而旅行时间又会影响背包的租金。因此，单独优化TSP或KP的最优解并不一定能得到TTP的最优解。作者还介绍了TTP的一些相关研究，包括不同类型的启发式算法和实例特征分析。

## 3 Using ants to steal items

(Prompt: Generalize Section "Using ants to steal items" of this article in Chinese)

这篇文章的“利用蚂蚁偷东西”部分介绍了一种基于蚁群优化的方法来解决TTP的旅行路线部分。蚁群优化是一种受自然界蚂蚁行为启发的随机搜索算法，它可以用于解决各种组合优化问题。文章的基本思想是让蚂蚁在一个构造图上进行随机游走，根据图上边的信息素值来影响它们的选择。在优化过程中，信息素值会根据找到的好解来更新，从而引导蚂蚁找到更好的解。在TTP中，蚂蚁负责创建旅行路线，然后对每个路线使用PackIterative启发式算法来生成背包计划，并根据TTP的目标值来评估每个解。文章还使用了一些局部搜索和缓存技术来提高算法的效率和效果。文章通过实验表明，该方法在多种规模和类型的TTP实例上都能超越现有的启发式算法，有时甚至能提高10%以上。

## 4 Experimental Study

(Prompt: Generalize Section "Experimental Study" of this article in Chinese)

- 本节首先描述了一般的实验设置和蚁群优化方法的配置。
- 然后，将我们的方法与文献中的最新方法进行了比较，并讨论了结果。
- 我们的方法在具有250个城市和2000个物品的实例上优于现有方法，有时超过10%。它实现了这一点，因为它比现有方法更少地关注好的TSP巡回路，而更多地关注好的TTP巡回路。

## 5 Concluding remarks

(Prompt: Generalize Section "Concluding remarks" of this article in Chinese)

- 我们的蚁群优化方法虽然不是“一劳永逸”的方法，但在具有250个城市和2000个物品的实例上优于现有方法，有时超过10%。它实现了这一点，因为它比现有方法更少地关注好的TSP巡回路，而更多地关注好的TTP巡回路。
- 我们研究了解决方案的增强，即TTP特定的局部搜索。这在一般情况下是有效的，然而，在较大的实例上，它太耗时，因此对性能不利，因为它减少了算法在固定时间预算内可以考虑的巡回路的数量。这使我们回到了一般问题。目前，TTP的搜索空间似乎非常难以导航。我们理解研究人员可能会倾向于使用构造启发式和爬山法来处理大型实例。然而，我们建议专注于小型实例，因为在这些实例上仍然可以实现大幅度的性能提升，正如我们的研究所显示的。
- 在未来，也许对简单方法在简单实例上进行计算复杂性分析可以帮助我们理解具有相互作用组件的问题。此外，对实例特征进行分析，其中不同组件的影响程度有所不同，可能有助于了解相互作用如何影响算法性能。

Keywords: swarm intelligence based on ant colony optimization => travelling thief problem