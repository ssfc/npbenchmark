# An effective implementation of the Lin–Kernighan traveling salesman heuristic

(Prompt: 翻译本文标题)

Lin-Kernighan旅行商问题启发式算法的一种有效实现

https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2000%20An%20effective%20implementation%20of%20the%20Lin%E2%80%93Kernighan%20traveling%20salesman%20heuristic.pdf

https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2000%20An%20effective%20implementation%20of%20the%20Lin%E2%80%93Kernighan%20traveling%20salesman%20heuristic.md

SU's suggestion: 初始解用现成的LKH算法（因为自己写太复杂了, 据说上万行）；后面调整，搜索阶段点交换，而不是像LKH一样边交换。

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

这一节介绍了旅行商问题（TSP）的实际意义和应用领域。旅行商问题不仅是一个理论上的难题，也是一个有着广泛应用的问题。例如，在生产计划、物流、晶体学、机器人控制、电路板钻孔、DNA测序等领域，都可以把旅行商问题作为一个子问题来求解。在这些应用中，城市的概念可以表示客户、焊点、DNA片段等，而距离的概念可以表示行驶时间或成本，或者DNA片段之间的相似度。旅行商问题也出现在天文学中，因为天文学家观测多个源时，会希望最小化望远镜在源之间移动的时间；在这种问题中，旅行商问题可以嵌入到一个最优控制问题中。在许多应用中，还可能有额外的约束，如有限的资源或时间窗口。

### 2.3 Solution algorithms

(Prompt: Generalize Section "2.3 Solution algorithms" of this article in Chinese)

这一节介绍了求解旅行商问题（TSP）的两类算法：精确算法和近似算法。

#### 2.3.1 Exact algorithm

精确算法可以保证找到最优解，但是运行时间很长，只能处理规模较小的问题。常用的精确算法有割平面算法和分支定界算法。

#### 2.3.2 Approximate algorithms

近似算法不能保证找到最优解，但是运行时间较短，可以处理规模较大的问题。常用的近似算法有构造算法、改进算法和复合算法。构造算法逐步建立一个可行解，改进算法对一个可行解进行局部优化，复合算法结合了这两种方法。其中，Lin-Kernighan算法是一种非常有效的改进算法，它基于变换l条边来寻找更短的路径。本文主要介绍了Lin-Kernighan算法及其改进版本，并在一些标准实例上进行了实验比较。

## 3. The Lin-Kernighan algorithm

(Prompt: Generalize Section "The Lin-Kernighan algorithm" of this article in Chinese)

### 3.1 The basic algorithm

(Prompt: Generalize Section "3.1 The basic algorithm" of this article in Chinese)

2-opt algorithm; 交叉四个顶点的连接方式

lambda-opt; 替换了lambda条边使路径更短

lambda常设为2或3

LK算法在执行时变换lambda的值。如果r links被考虑，r+1 links也被考虑。

交换边X和Y是逐步构造的。挑选构造零件。

(1) The sequential exchange criterion

(x1, y1, x2, y2, x3, y3) 构成一个链条，两两共享顶点。

(2) The feasibility criterion

(3) The positive gain criterion

every partial sum, Gi , must be positive

(4) The disjunctivity criterion

Algorithm:

1. 生成一个随机的初始路线T。
2. 令i=1，选择t1  // Comment: t是顶点，x和y都是边
3. 选择T中的一条边x1 = (t1,t2) 
4. 选择不在T中的一条边y1=(t2,t3)，使得G1>0，其中G1是交换x1和y1后的路线长度减少量。如果这样做不可能，就跳到步骤12。
5. 令i=i+1
6. 选择T中的一条边xi=(t2i-1,t2i)，使得
   - 如果将t2i和t1连接起来，得到的是一个可行的路线T’，并且
   - xi不等于ys，对于所有s<i。 如果T’是比T更好的路线，就让T=T’，然后跳到步骤2。
7.   (Comment: 如果在r较短的时候就找到了更优解，就开始下一次迭代)
8. Choose yi = (t2i,t2i+1) Ï T such that (a) Gi > 0, (b) yi ¹ xs for all s £ i, and (c) xi+1 exists. If such yi exists, go to Step 5.
9. 如果y2还有没有尝试过的选择，就让i=2，然后跳到步骤7。
10. 如果x2还有没有尝试过的选择，就让i=2，然后跳到步骤6。
11. 如果y1还有没有尝试过的选择，就让i=1，然后跳到步骤4。
12. 如果x1还有没有尝试过的选择，就让i=1，然后跳到步骤3。
13. 如果t1还有没有尝试过的选择，就跳到步骤2。
14. 停止（或者跳到步骤1）。

## 4. The modified Lin-Kernighan algorithm

(Prompt: Generalize Section "The modified Lin-Kernighan algorithm" of this article in Chinese)

## 5. Implementation

(Prompt: Generalize Section "Implementation" of this article in Chinese)

## 6. Computational results

(Prompt: Generalize Section "Computational results" of this article in Chinese)

## 7. Conclusions

(Prompt: Generalize Section "Conclusions" of this article in Chinese)