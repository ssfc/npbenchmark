# An effective implementation of the Lin–Kernighan traveling salesman heuristic

(Prompt: 翻译本文标题)

Lin-Kernighan旅行商问题启发式算法的一种有效实现

https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2000%20An%20effective%20implementation%20of%20the%20Lin%E2%80%93Kernighan%20traveling%20salesman%20heuristic.pdf

https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2000%20An%20effective%20implementation%20of%20the%20Lin%E2%80%93Kernighan%20traveling%20salesman%20heuristic.md

## Abstract

(Prompt: generalize abstract of this article in Chinese)

本文介绍了一种基于Lin-Kernighan启发式算法的旅行商问题（TSP）求解方法。该方法对原始算法进行了改进，提高了搜索策略的效率和灵活性。通过计算实验，证明了该方法能够在较短的时间内找到最优或接近最优的解，包括一个13509个城市的问题（目前已知最大的非平凡问题）。

Keywords: an implementation of Lin–Kernighan => traveling salesman problem

### Q: 这篇文章发表会议/期刊属于CCF哪类？这篇文章在google scholar引用次数多少？

这篇文章发表在《欧洲运筹学杂志》（European Journal of Operational Research），这是一份在运筹学领域有影响力的期刊。根据CCF排名，这份期刊属于C类。²³

这篇文章在google scholar的引用次数是2020，截至2023年5月16日。

### Q: European Journal of Operational Research是什么期刊？

《欧洲运筹学杂志》（European Journal of Operational Research）是一份在运筹学领域有影响力的期刊，发表了关于运筹学方法论和决策实践的高质量、原创的论文。¹ 这份期刊包括以下类型的论文：

- 邀请综述，向运筹学读者介绍近年来某一运筹学主题的发展；
- 运筹学的创新应用，描述解决实际问题的新方法；
- 理论与方法论论文，提出对运筹学方法论和理论基础有贡献的原创研究结果；
- 短通讯，如果它们纠正了EJOR在过去5年内发表的论文中的重要错误。

这份期刊由欧洲运筹学协会（EURO）与Elsevier出版社合作出版。¹

### Q: 用中文依次介绍本文作者

本文的作者是Keld Helsgaun，他是丹麦罗斯基勒大学的计算机科学名誉副教授。1 他的研究兴趣包括人工智能、组合优化和运筹学。2 他开发了一种有效的实现林-科尼汉旅行商问题启发式算法（LKH），并在多个国际期刊和会议上发表了相关论文。

## 1. Introduction

(Prompt: Generalize Section "Introduction" of this article in Chinese)

这篇文章介绍了一种实现Lin-Kernighan启发式算法的方法，这是一种用于求解对称旅行商问题的最有效的方法之一。文章首先回顾了旅行商问题的定义和解法，然后详细描述了原始的Lin-Kernighan算法及其改进，最后提出了一个新的修改版本的算法，并报告了其在各种问题实例上的效果。文章指出，新算法通过对搜索策略和候选集的修正，显著提高了效率和解质量，能够找到所有已知最优解，包括一个7397城市的问题（目前已解决的最大非平凡问题实例）。

## 2. The traveling salesman problem

(Prompt: Generalize Section "The traveling salesman problem" of this article in Chinese)

### 2.1 Formulation

(Prompt: Generalize Section "2.1 Formulation" of this article in Chinese)

这一节定义了旅行商问题（traveling salesman problem, TSP），这是一个组合优化中的经典问题。旅行商问题是给定一组城市和每对城市之间的距离，求一条最短的路径，使得从一个城市出发，恰好访问每个城市一次，然后回到出发城市。这个问题可以用数学语言表述为：给定一个代价矩阵C = (cij)，其中cij表示从城市i到城市j的代价，找到一个排列(i1, i2, ..., in)，使得下式最小化：

ci1i2 + ci2i3 + ... + cini1

根据代价矩阵C的性质，可以对问题进行分类：

- 如果cij = cji对所有的i和j都成立，那么问题是对称的；否则，是非对称的。
- 如果满足三角不等式（cik ≤ cij + cjk对所有的i，j和k），那么问题是度量的。
- 如果cij是平面上点之间的欧几里得距离，那么问题是欧几里得的。一个欧几里得问题当然也是对称的和度量的。

### 2.2 Motivation

(Prompt: Generalize Section "2.2 Motivation" of this article in Chinese)

## 3. The Lin-Kernighan algorithm

(Prompt: Generalize Section "The Lin-Kernighan algorithm" of this article in Chinese)

## 4. The modified Lin-Kernighan algorithm

(Prompt: Generalize Section "The modified Lin-Kernighan algorithm" of this article in Chinese)

## 5. Implementation

(Prompt: Generalize Section "Implementation" of this article in Chinese)

## 6. Computational results

(Prompt: Generalize Section "Computational results" of this article in Chinese)

## 7. Conclusions

(Prompt: Generalize Section "Conclusions" of this article in Chinese)