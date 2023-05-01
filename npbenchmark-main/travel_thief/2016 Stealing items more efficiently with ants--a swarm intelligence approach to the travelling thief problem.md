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

## 3 Using ants to steal items

(Prompt: Generalize Section "Using ants to steal items" of this article in Chinese)

## 4 Experimental Study

(Prompt: Generalize Section "Experimental Study" of this article in Chinese)

## 5 Concluding remarks

(Prompt: Generalize Section "Concluding remarks" of this article in Chinese)