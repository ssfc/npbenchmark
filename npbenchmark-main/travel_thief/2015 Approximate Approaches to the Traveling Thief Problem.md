# Approximate Approaches to the Traveling Thief Problem

(Prompt: 翻译本文标题)

旅行小偷问题的近似方法

https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2015%20Approximate%20Approaches%20to%20the%20Traveling%20Thief%20Problem.pdf

https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2015%20Approximate%20Approaches%20to%20the%20Traveling%20Thief%20Problem.md

## Abstract

(Prompt: generalize abstract of this article in Chinese)

本研究讨论了最近提出的旅行小偷问题（TTP），它将经典的旅行商问题（TSP）和0-1背包问题（KP）结合起来。这个问题包括一组城市，每个城市都有一些有重量和利润的可用物品。它涉及寻找一种城市访问的排列方式和选择拿走哪些物品的决策。一个被选择的物品以增加运输成本为代价，为总利润做出贡献。目标是最大化最终的利润。我们提出了一些针对特定问题的装箱策略，它们是基于由链式林-科尼汉启发式算法得到的TSP解决方案的。我们在一组基准实例上进行了研究，证明了我们的方法与近似的混合整数规划方法和文献中的最先进的启发式方法相比，具有快速和高效的优势。

Keywords: Chained Lin-Kernighan heuristic => Traveling Thief Problem

### Q: 这篇文章发表会议/期刊属于CCF哪类？这篇文章在google scholar引用次数多少？

GECCO会议是遗传和进化计算领域的顶级会议，属于CCF C类。这篇文章在google scholar的引用次数是65。

### Q: GECCO是什么会议？

GECCO是**遗传和进化计算会议**的缩写，它是遗传和进化计算领域最大的同行评审会议，也是ACM遗传和进化计算特别兴趣小组(SIGEVO)的主要会议¹。GECCO涵盖了遗传算法、遗传编程、蚁群优化和群智能、复杂系统、进化组合优化和元启发式、进化机器学习、进化多目标优化、进化数值优化等多个主题²。GECCO每年举办一次，2023年将在葡萄牙里斯本举行³。

### Q: Introduce authors of this article in Chinese one by one

这篇文章的作者有四位，分别是：

- **Hayden Faulkner**，是澳大利亚阿德莱德大学计算机科学学院的博士生，主要研究方向是优化和物流²。
- **Sergey Polyakovskiy**，是澳大利亚阿德莱德大学计算机科学学院的博士后研究员，主要研究方向是组合优化、元启发式算法和多目标优化³。
- **Tom Schultz**，是澳大利亚阿德莱德大学计算机科学学院的副教授，主要研究方向是进化计算、机器学习和人工智能⁴。
- **Markus Wagner**，是澳大利亚阿德莱德大学计算机科学学院的高级讲师，主要研究方向是进化计算、优化和搜索。

## 1. INTRODUCTION

(Prompt: Generalize Section 1 of this article in Chinese)

他们在这篇文章中研究了一个新提出的组合优化问题，叫做旅行商偷窃问题（Traveling Thief Problem, TTP）。这个问题结合了经典的旅行商问题（Traveling Salesman Problem, TSP）和0-1背包问题（0-1 Knapsack Problem, KP）。这个问题包括一组城市，每个城市有一些可用的物品，每个物品有自己的重量和利润。问题的目标是寻找一个城市的排列顺序和一个选择物品的方案，使得总利润最大化。作者提出了一些针对这个问题的启发式打包策略，并在一组基准实例上与一个近似的混合整数规划方法和文献中的最先进的启发式方法进行了比较。

## 2. TRAVELING THIEF PROBLEM

(Prompt: Generalize Section "TRAVELING THIEF PROBLEM" of this article in Chinese)

这篇文章的“旅行商偷窃问题”部分是对这个问题的定义和描述。旅行商偷窃问题（Traveling Thief Problem, TTP）是一个新提出的组合优化问题，它结合了经典的旅行商问题（TSP）和0-1背包问题（KP）。这个问题包括一组城市，每个城市有一些可用的物品，每个物品有自己的重量和利润。问题的目标是寻找一个城市的排列顺序和一个选择物品的方案，使得总利润最大化。作者给出了这个问题的数学模型和目标函数，并举了一个简单的数值例子来说明这个问题的特点和难点。作者还指出了这个问题与单独的TSP或KP之间的相互影响和制约关系。

## 3. LOCAL SEARCH ROUTINES

(Prompt: Generalize Section "LOCAL SEARCH ROUTINES" of this article in Chinese)

## 4. LOCAL SEARCH ALGORITHMS

(Prompt: Generalize Section "LOCAL SEARCH ALGORITHMS" of this article in Chinese)

## 5. PACKING ITEMS WITH A MIP BASED APPROACH

(Prompt: Generalize Section "PACKING ITEMS WITH A MIP BASED APPROACH" of this article in Chinese)

## 6. EXPERIMENTAL INVESTIGATIONS

(Prompt: Generalize Section "EXPERIMENTAL INVESTIGATIONS" of this article in Chinese)

## 7. CONCLUDING REMARKS

(Prompt: Generalize Section "CONCLUDING REMARKS" of this article in Chinese)

