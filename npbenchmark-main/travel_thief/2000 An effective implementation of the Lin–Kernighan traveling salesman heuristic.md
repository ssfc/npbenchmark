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
3. 选择T中的一条边x1 = (t1,t2)  // Comment: 选择一条连接到旅行路线的链接 x1 = (t1,t2)。当 t1 被选择时，有两种选择 x1。这里的“选择”意味着“选择一个未尝试过的替代方案”。然而，每次找到一个改进的旅行路线时（在步骤 6 中），所有的替代方案都被认为是未尝试过的。
4. 选择不在T中的一条边y1=(t2,t3)，使得G1>0，其中G1是交换x1和y1后的路线长度减少量。如果这样做不可能，就跳到步骤12。
5. 令i=i+1
6. 选择T中的一条边xi=(t2i-1,t2i)，使得
   - 如果将t2i和t1连接起来，得到的是一个可行的路线T’，并且
   - xi不等于ys，对于所有s<i。 如果T’是比T更好的路线，就让T=T’，然后跳到步骤2。
   - Comment: 有两种选择 xi。然而，对于给定的 yi-1 (i ≥ 2)，只有其中一种使得可以“关闭”旅行路线（通过添加 yi）。另一种选择会导致两个断开的子路线。只有一种情况下，这样的不可行的选择是允许的，即 i = 2。图 3.5 显示了这种情况。
7. 选择不在T中的一条边yi=(t2i,t2i+1)，使得
   - Gi>0，其中Gi是交换xi和yi后的路线长度减少量，并且
   - yi不等于xs，对于所有s<=i，以及
   - xi+1存在。 如果这样的yi存在，就跳到步骤5。
8. 如果y2还有没有尝试过的选择，就让i=2，然后跳到步骤7。// Comment: 这些步骤导致回溯。注意，只有在没有找到改进的情况下，才允许回溯，并且只在第 1 和第 2 级允许回溯。
9. 如果x2还有没有尝试过的选择，就让i=2，然后跳到步骤6。
10. 如果y1还有没有尝试过的选择，就让i=1，然后跳到步骤4。
11. 如果x1还有没有尝试过的选择，就让i=1，然后跳到步骤3。
12. 如果t1还有没有尝试过的选择，就跳到步骤2。
13. 停止（或者跳到步骤1）。// Comment: 当所有的 t1 值都被检查过而没有改进时，算法以一个解决方案结束。如果需要，可以在步骤 1 中考虑一个新的随机初始旅行路线。

## 4. The modified Lin-Kernighan algorithm

(Prompt: Generalize Section "The modified Lin-Kernighan algorithm" of this article in Chinese)

Lin和Kernighan的原始算法非常有效。对于最多50个城市的问题，单次试验获得最优解的概率接近100%。对于100个城市的问题，概率下降到20%至30%之间。然而，通过运行几次试验，每次从一个新的随机路径开始，几乎可以确保找到这些问题的最优解。

该算法在一系列问题上进行了评估，其中包括一个包含318个点的钻探问题。由于计算机存储的限制，问题被分成了三个较小的问题。通过分别解决子问题并最后连接三个路径，得到了解决方案。在Lin和Kernighan撰写他们的论文时（1971年），这个问题的最优解是未知的。现在知道最优解后，可以注意到他们的解决方案比最优解高出1.3%。

接下来，介绍了一个修改和扩展版本的算法。这个新算法是对原始算法的显著改进。例如，对于提到的318个城市问题，最优解现在可以在几次试验中找到（大约2次），并且非常快速（在300 MHz G3 Power Macintosh上大约1秒）。总体而言，该算法所达到的解的质量非常令人印象深刻。该算法已能够找到我们能够获取的所有问题实例的最优解，包括一个包含7397个城市的问题（迄今为止解决的最大非平凡问题实例）。

提高效率主要是通过对Lin和Kernighan在限制和引导搜索方面的启发式规则进行修订来实现的。即使他们的启发式规则看起来很自然，但经过批判性分析发现它们存在相当大的缺陷。

### 4.1 Candidate sets

(Prompt: Generalize Section "4.1 Candidate sets" of this article in Chinese)

这一部分介绍了如何使用一种基于最小生成树的敏感度分析的方法，来确定哪些边有可能属于最优旅行路线，从而构建候选集。这种方法使用了一种称为α-近似度的度量，来评估每条边属于最优旅行路线的概率。作者提出了一种高效的算法，来计算所有边的α-近似度，并根据这些值来选择候选集。作者认为，使用α-近似度来指定候选集比使用最近邻居要好得多。

### 4.2 Breaking of tour edges

(Prompt: Generalize Section "4.2 Breaking of tour edges" of this article in Chinese)

这一部分介绍了如何选择要从旅行路线中断开的边，以进行交换。作者指出，原始算法中的规则（规则 6）有缺陷，因为它可能导致忽略一些有利的交换。作者提出了一种新的规则，根据边的α-近似度来确定是否应该断开它。作者认为，这种规则可以更好地保留最优旅行路线中的边，同时避免过度保守。

### 4.3 Basic moves

(Prompt: Generalize Section "4.3 Basic moves" of this article in Chinese)

这一部分介绍了如何选择要加入旅行路线的边，以进行交换。作者指出，原始算法中的规则（规则 8）有缺陷，因为它可能导致忽略一些有利的交换。作者提出了一种新的规则，根据边的α-近似度和长度来确定它们的优先级。作者认为，这种规则可以更好地平衡搜索的广度和深度，同时避免过度贪婪。

### 4.4 Initial tours

(Prompt: Generalize Section "4.4 Initial tours" of this article in Chinese)

这一部分介绍了如何生成初始旅行路线，作为算法的起点。作者指出，初始旅行路线的质量对算法的效果有很大影响。作者提出了一种新的方法，根据α-近似度来构造初始旅行路线。作者认为，这种方法可以生成比最近邻居法更好的初始旅行路线，同时避免过度复杂。

### Q: 什么是α-近似度？

α-近似度是一种用于评估近似算法的度量指标。在计算机科学和优化领域中，很多问题是难以在多项式时间内求解的，因此人们常常使用近似算法来寻找问题的近似解。

α-近似度是指一个近似算法的解与最优解之间的相对误差。具体而言，对于一个最优化问题，如果存在一个近似算法能够在多项式时间内找到一个解，使得该解的目标函数值不超过最优解的目标函数值的 α 倍（α > 1），那么该算法的 α-近似度就是 α。

换句话说，α-近似度衡量了近似算法得到的解与最优解之间的质量差距。当 α 的值越接近于 1，意味着近似算法得到的解越接近于最优解。常见的 α-近似度问题包括最小生成树的近似算法、旅行商问题的近似算法等。

α-近似度的概念在算法设计和性能评估中起着重要的作用，它帮助我们了解近似算法在解决实际问题时的效果和可行性。

### 4.5 Specification of the modified algorithm

(Prompt: Generalize Section "4.5 Specification of the modified algorithm" of this article in Chinese)

这一部分给出了修改后的算法的详细描述，包括伪代码和注释。作者指出，修改后的算法与原始算法的主要区别在于选择断开和加入旅行路线的边的规则。作者使用了α-近似度和其他一些启发式规则来改进搜索策略。作者还介绍了一些其他的改进，例如使用候选集来加速搜索，使用多重启动来增加找到最优解的概率，以及使用后优化来进一步改善解。

## 5. Implementation

(Prompt: Generalize Section "Implementation" of this article in Chinese)

## 6. Computational results

(Prompt: Generalize Section "Computational results" of this article in Chinese)

## 7. Conclusions

(Prompt: Generalize Section "Conclusions" of this article in Chinese)