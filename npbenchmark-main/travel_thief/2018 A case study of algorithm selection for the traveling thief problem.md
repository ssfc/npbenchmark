# A case study of algorithm selection for the traveling thief problem

(Prompt: 翻译本文标题)

旅行窃贼问题的算法选择案例研究

https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2018%20A%20case%20study%20of%20algorithm%20selection%20for%20the%20traveling%20thief%20problem.pdf

https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2018%20A%20case%20study%20of%20algorithm%20selection%20for%20the%20traveling%20thief%20problem.md

## Abstract

(Prompt: generalize abstract of this article in Chinese)

这篇文章研究了旅行窃贼问题（traveling thief problem, TTP）的算法选择问题。TTP是一个组合优化问题，它结合了两个子问题：旅行商问题（TSP）和背包问题（KP）。作者提出了四个贡献：首先，他们创建了一个全面的数据集，包含了21种TTP算法在9720个TTP实例上的性能数据；其次，他们定义了55个TTP实例特征，可以用来选择每个实例的最佳算法；第三，他们利用这些算法和特征构建了第一个TTP算法组合，明显优于单一最佳算法；最后，他们分析了哪些算法对组合的性能贡献最大。

Keywords:  dataset; characteristics;  AutoFolio => traveling thief problem

### Q: 这篇文章发表会议/期刊属于CCF哪类？这篇文章在google scholar引用次数多少？

- 这篇文章发表的期刊是Journal of Heuristics，它属于中国计算机学会（CCF）推荐的国际学术刊物目录中的B类期刊¹。
- 这篇文章在google scholar上的引用次数是68。

### Q: Journal of Heuristics是什么期刊？

- Journal of Heuristics是一本国际期刊，主要发表关于启发式算法理论和应用的研究文章。它涵盖了人工智能、运筹学、工程、商业和社会问题等领域的启发式优化方法，包括元启发式、超启发式和数学启发式等¹。
- 这个期刊的主编是美国布朗大学的Pascal Van Hentenryck教授，他是人工智能规划、调度、设计、约束满足和博弈论等方面的专家¹。
- 这个期刊的出版商是Springer，它采用混合出版模式，支持开放获取（Open Access）¹。

### Q: Introduce authors of this article in Chinese one by one

这篇文章的作者如下：

- **Markus Wagner**是澳大利亚莫纳什大学信息技术学院的副教授/研究员，主要从事启发式优化算法的研究，特别是进化计算和元启发式算法。他曾在哈尔滨工业大学、布朗大学等高校任教或访问¹²。
- **Marius Lindauer**是德国弗莱堡大学计算机科学系的副教授，主要从事自动化机器学习、算法选择和配置、元启发式算法等领域的研究。他是AutoFolio、Auto-sklearn等开源软件的开发者³。
- **Mustafa Mısır**是中国南京航空航天大学机器学习与计算智能研究所的教授，主要从事元启发式算法、超启发式算法、自适应优化等领域的研究。他曾在比利时鲁汶大学、新加坡国立大学等高校工作。
- **Samadhi Nallaperuma**是英国谢菲尔德大学计算机科学系的讲师，主要从事启发式优化算法、软件工程、搜索问题等领域的研究。她曾在澳大利亚阿德莱德大学、新西兰坎特伯雷大学等高校工作。
- **Frank Hutter**是德国弗莱堡大学计算机科学系的教授，也是加拿大多伦多大学维克托·韦斯特维尔人工智能研究所的首席科学家，主要从事自动化机器学习、元启发式算法、深度强化学习等领域的研究。他是AutoFolio、Auto-sklearn、SMAC等开源软件的开发者。

## 1 Introduction

(Prompt: Generalize Section 1 of this article in Chinese)

第一部分介绍了旅行商窃贼问题（travelling thief problem, TTP）的定义和动机。TTP是一个由旅行商问题（TSP）和背包问题（KP）组合而成的多组件问题，可以模拟现实中存在的多个相互影响的子问题。作者回顾了TTP的历史发展和已有的求解算法，指出不同的算法在不同的实例上有不同的表现，因此需要利用算法选择技术来根据每个实例选择最佳算法。作者也介绍了算法选择问题的一般框架和常用方法，以及如何将其应用于TTP问题。

## 2 The travelling thief problem (TTP)

(Prompt: Generalize Section "The travelling thief problem (TTP)" of this article in Chinese)

这一部分介绍了TTP的定义和形式化。TTP是一个将TSP和KP两个经典的组合优化问题结合起来的多组件问题，可以模拟现实中存在的多个相互影响的子问题。TTP的目标是在给定一组城市和每个城市中分布的物品的情况下，找到一条旅行路线和一个装包方案，使得窃贼在访问所有城市并偷取部分物品后，能够获得最大的利润。TTP的难点在于TSP和KP两个子问题之间存在着复杂的相互依赖关系，单独优化其中一个子问题并不能保证得到整个问题的最优解。作者也介绍了TTP的动机，即为了系统地研究两个难题之间的交互效应，从而有助于更有效地解决实际问题。

## 3 Algorithm Selection

(Prompt: Generalize Section "Algorithm Selection" of this article in Chinese)

这一部分介绍了算法选择的问题和方法。算法选择的问题是要找到一个从问题实例到算法的映射，使得对于每个实例，都能选择一个表现最好的算法。算法选择的方法通常是利用机器学习技术，根据一些描述实例特征的数值，学习一个从特征空间到算法的模型。作者比较了几种不同的算法选择方法，包括回归、分类、聚类和推荐系统等，并使用了一个**自动化的算法选择系统AutoFolio**，它可以根据不同的算法选择场景，自动搜索最合适的算法选择方法和参数设置。作者在TTP的9720个实例上进行了实验，结果表明，使用算法选择可以显著提高TTP问题的求解效果，比单一最佳算法有了近90%的性能提升。

### Q: 用中文介绍AutoFolio

AutoFolio 是一款基于自动化技术的自动化算法选择工具，用于优化在多个实例集合上的算法性能。它是由德国马普学会计算智能研究所的研究员开发的，主要用于解决NP难题等问题。

AutoFolio 通过使用一个学习模型来自动地选择最优算法及其参数，从而提高算法的性能。它可以处理多种类型的算法（如SAT、QBF等），并且能够适应不同的问题领域和实例集合。

AutoFolio 的工作原理基于以下几个步骤：

1. 收集和预处理实例集合数据
2. 选择一组算法并在实例集合上进行评估
3. 基于学习模型预测最佳算法和参数
4. 在实例集合上测试预测的最佳算法和参数

通过不断重复上述步骤，AutoFolio 可以不断地提高算法的性能，并找到最佳的算法和参数组合。

AutoFolio 在学术界和工业界都有广泛的应用，已经被证明是一种高效、可靠的自动化算法选择工具。

## 4 Benchmarking of TTP Algorithms

(Prompt: Generalize Section "Benchmarking of TTP Algorithms" of this article in Chinese)

这一部分介绍了作者对21种TTP算法在9720个TTP实例上的实验设置和结果。作者使用了原始的TTP实例集，它包含了不同规模、不同类型、不同特征的实例，以及每个实例上各个算法的目标值。作者**将目标值映射到[0,1]区间**，以便进行比较，并计算了每个算法的平均性能。作者发现，没有一个算法在所有实例上都占优，而且不同的算法在不同大小的实例上有不同的表现。作者还发现，一些算法之间有很强的相关性，而一些算法则相对互补。作者将实验数据集公开分享，以便其他研究者使用。

## 5 Instance Features for the TTP

(Prompt: Generalize Section "Instance Features for the TTP" of this article in Chinese)

这一部分介绍了作者为TTP实例定义的55个特征，其中47个是从以前的TSP研究中借鉴的，分为七类，包括**距离特征、模式特征、聚类特征、最近邻距离特征、质心特征、最小生成树特征和角度特征**。另外八个特征是与TTP相关的，包括**背包容量、背包类型、物品总数、每个城市的物品数、城市数、租金比率、最小旅行速度和最大旅行速度**。这些特征可以用来选择每个实例上表现最好的算法。作者指出，未来的研究应该考虑更多的TTP特定的特征。

## 6 Experimental Study of Algorithm Selection on TTP

(Prompt: Generalize Section "Experimental Study of Algorithm Selection on TTP" of this article in Chinese)

这一部分介绍了作者使用不同的算法选择方法来构建TTP算法组合的实验过程和结果。作者使用了一个包含9720个TTP实例、21个TTP算法和55个实例特征的数据集，**利用机器学习技术来学习从特征空间到算法的映射**。作者参考了现有的算法选择系统，如SATzilla、ISAC、3S等，并使用了AutoFolio系统来自动寻找最合适的算法选择方法和参数设置。作者发现，使用算法选择方法可以显著提高TTP问题的求解性能，比单一最佳算法有近90%的性能提升。作者还分析了不同算法在组合中的贡献度，发现有些算法在某些实例上表现出色，而在其他实例上则很差，因此需要考虑算法之间的互补性。

## 7 Analysis of Algorithm Complementarity with Shapley Values

(Prompt: Generalize Section "Analysis of Algorithm Complementarity with Shapley Values" of this article in Chinese)

这一部分介绍了作者使用Shapley值来评估不同算法在组合中的贡献度的方法和结果。Shapley值是一种博弈论中的概念，用于衡量一个玩家在一个合作博弈中的平均边际贡献。作者将每个算法视为一个玩家，将每个实例的最优性能视为博弈的收益，然后计算每个算法在所有可能的子集组合中的平均边际贡献。作者发现，Shapley值与算法的单独性能有很强的相关性，但也存在一些例外。例如，CS2SA算法虽然单独性能很差，但在组合中有较高的Shapley值，说明它在某些实例上有很好的互补性。作者还发现，Shapley值可以用来选择最佳的算法组合，即选择Shapley值最高的k个算法作为组合，可以获得接近最优的性能。

## 8 Analysis of Feature Importance and Their Calculation Time

(Prompt: Generalize Section "Analysis of Feature Importance and Their Calculation Time" of this article in Chinese)

文章的“特征重要性及其计算时间分析”部分讨论了如何衡量不同特征对于预测给定TTP实例的最佳算法的相对重要性。作者使用了一种叫做置换特征重要性的方法，它随机打乱一个特征的值，观察算法选择器的性能变化程度。性能下降得越多，特征就越重要。作者还比较了不同特征的计算时间，发现有些特征比其他特征更耗时。他们建议可以使用特征选择来减少特征的数量，提高算法选择的效率。

## 9  Concluding Remarks

(Prompt: Generalize Section " Concluding Remarks" of this article in Chinese)

文章的“结论”部分总结了本研究的主要贡献和发现。作者指出，他们创建了一个全面的TTP实例、特征和算法性能的数据集，可以用于进一步的算法选择和组合生成的研究。他们还报告说，他们构建了第一个TTP算法组合，它在每个实例上明显优于单个最佳算法。此外，他们还使用了不同的方法和指标，分析了特征的重要性和计算时间，以及算法的互补性和贡献度。他们最后强调了TTP和算法选择方面的一些开放性挑战和未来的工作方向。

Keywords:  dataset; characteristics;  AutoFolio => traveling thief problem