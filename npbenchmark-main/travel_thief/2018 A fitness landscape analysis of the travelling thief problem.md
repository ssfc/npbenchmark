# A fitness landscape analysis of the Travelling Thief Problem

(Prompt: 翻译本文标题)

旅行窃贼问题的适应度景观分析

https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2018%20A%20fitness%20landscape%20analysis%20of%20the%20travelling%20thief%20problem.pdf

https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2018%20A%20fitness%20landscape%20analysis%20of%20the%20travelling%20thief%20problem.md

## Abstract

(Prompt: generalize abstract of this article in Chinese)

局部最优网络是一种模型，旨在理解组合景观的结构和特性。适应度景观被探索为一个图，其中节点表示局部最优（或吸引盆），边表示它们之间的连通性。在本文中，我们使用这种表示来研究一个组合优化问题，它有两个相互依赖的组成部分，称为旅行窃贼问题（TTP）。目标是使用基本的局部搜索启发式方法来理解TTP的搜索空间结构，并区分最具影响力的问题特征。我们创建了一大批可枚举的TTP实例，并使用两种爬山法变体为每个实例生成一个局部最优网络。我们研究了两个问题特征，即背包容量和利润-重量相关性。我们的见解不仅对于设计景观感知的局部搜索启发式方法有用，而且对于更好地理解特定启发式方法难以解决的TTP原因也有用。

Keywords: LOCAL SEARCH HEURISTICS => fitness landscape => Travelling Thief Problem

### Q: 这篇文章发表会议/期刊属于CCF哪类？这篇文章在google scholar引用次数多少？

这篇文章发表在GECCO 2018会议上，这是一个遗传和进化计算领域的国际会议。根据CCF 2021年人工智能领域的评级²，GECCO属于C类会议。根据Google Scholar的查询结果⁴，这篇文章在Google Scholar上被引用了28次。

### Q: GECCO是什么会议？

GECCO是遗传和进化计算会议（Genetic and Evolutionary Computation Conference）的缩写，是遗传和进化计算领域的一个国际会议，自1999年以来每年举办一次。该会议涵盖了遗传算法、遗传编程、蚁群优化和群智能、复杂系统、进化组合优化和元启发式、进化机器学习、进化多目标优化、进化数值优化、神经进化、实际应用、基于搜索的软件工程、理论、混合等主题¹²³⁴⁵⁶。

### Q: Introduce authors of this article in Chinese one by one

这篇文章的作者有以下几位：

- Mohamed El Yafrani，是摩洛哥拉巴特穆罕默德五世大学科学学院LRIT URAC 29实验室的研究员，也是丹麦奥尔堡大学计算机科学系的客座教授¹²。
- Marcella S. R. Martins，是巴西巴拉那联邦技术大学（UTFPR）的教授，主要研究方向是进化计算、组合优化和机器学习³⁴。
- Mehdi El Krari，是摩洛哥拉巴特穆罕默德五世大学科学学院LRIT URAC 29实验室的研究员，也是澳大利亚阿德莱德大学优化与物流研究中心的博士后研究员，主要研究方向是进化计算、组合优化和机器学习⁵ 。
- Markus Wagner，是澳大利亚阿德莱德大学优化与物流研究中心的高级讲师，主要研究方向是进化计算、组合优化和机器学习 。
- Myriam R. B. S. Delgado，是巴西巴拉那联邦技术大学（UTFPR）的教授，主要研究方向是进化计算、组合优化和机器学习 。
- Belaïd Ahiod，是摩洛哥拉巴特穆罕默德五世大学科学学院LRIT URAC 29实验室的教授，主要研究方向是人工智能、数据挖掘和机器学习 。
- Ricardo Lüders，是巴西巴拉那联邦技术大学（UTFPR）的教授，主要研究方向是进化计算、组合优化和机器学习 。

## 1 MOTIVATION

(Prompt: Generalize Section "MOTIVATION" of this article in Chinese)

本文的目的是使用基本的局部搜索启发式算法来研究一个组合优化问题的搜索空间结构，该问题具有两个相互依赖的组件，称为旅行商窃贼问题（TTP）。TTP是一个复杂的问题，反映了现实世界优化问题的一些重要方面，如复合结构和相互依赖性。作者通过分析局部最优网络（Local Optima Networks, LON）的特征，来探索TTP实例的难度和影响因素。LON是一种简化的适应度景观模型，将适应度景观表示为连接局部最优的图。作者认为，这些见解不仅有助于设计基于景观的局部搜索启发式算法，而且有助于更好地理解具有相互依赖组件的复杂问题的搜索空间结构。

## 2 BACKGROUND AND RELATED WORKS

(Prompt: Generalize Section "BACKGROUND AND RELATED WORKS" of this article in Chinese)

本节介绍了旅行商窃贼问题的数学表达式，并简要回顾了适应度景观分析的相关研究。适应度景观分析是一种用于研究组合优化问题的搜索空间结构和特征的方法，可以帮助预测启发式搜索算法的性能。局部最优网络（Local Optima Networks, LON）是一种简化的适应度景观模型，将局部最优点和它们之间的连接关系表示为一个图。本文使用两种基于爬山法的局部搜索算法来生成旅行商窃贼问题的局部最优网络，并使用图论的指标来分析它们的拓扑结构。

## 3 LOCAL SEARCH HEURISTICS

(Prompt: Generalize Section "LOCAL SEARCH HEURISTICS" of this article in Chinese)

本节介绍了一个基本的局部搜索框架，用于解决旅行商窃贼问题。该框架是一个确定性的爬山法，只用于探索问题的搜索空间结构。该框架可以根据选择的邻域操作符有多种实现。本文考虑了两种局部搜索变体，一种使用2-OPT邻域作为旅行商问题的邻域操作符，另一种使用插入操作符。在两种变体中，都使用一位翻转操作符作为背包问题的邻域操作符。

## 4 EXPERIMENTAL SETTING

(Prompt: Generalize Section "EXPERIMENTAL SETTING" of this article in Chinese)

本节介绍了实验的设计和参数。旅行商窃贼问题的实例可以根据以下特征进行分类：城市数量、每个城市的物品数量、物品的重量和利润之间的相关性、背包容量等级。本文主要关注两个非平凡的特征，即背包容量和重量-利润相关性，并根据这两个特征对问题进行分类。为了生成局部最优网络并识别吸引盆，我们使用了7个城市和6个物品（每个城市一个，除了起始城市）的小型实例。我们使用了一个专门的实例生成器来产生可枚举的实例，并根据[20]中的指导进行了设计。对于每个类别，我们生成了100个样本，并分析了它们的适应度景观。

## 5 RESULTS AND ANALYSIS

(Prompt: Generalize Section "RESULTS AND ANALYSIS" of this article in Chinese)

本节分析了使用局部搜索启发式算法为旅行商窃贼问题生成的局部最优网络，以了解搜索空间的结构。此外，还研究了吸引盆及其与一些局部最优网络特性之间的关系，以寻找更多关于搜索难度的信息。首先，通过观察局部最优网络的各种图形指标，如顶点数、边数、平均度数、聚类系数等，可以初步判断搜索难度。结果表明，背包容量越大，搜索难度越低。其次，通过绘制累积度分布函数，并使用Kolmogorov-Smirnov检验来评估幂律和指数模型的适用性，可以发现局部最优网络具有一些真实网络的特征，如小世界性质和无标度性质。这意味着大多数局部最优解只有少量的连接，而少数局部最优解有很多的连接。这对于设计高效的局部搜索算法是有帮助的。最后，通过计算吸引盆的大小、深度和逃逸能力，并与局部最优网络的其他特征进行相关分析，可以进一步了解不同问题特征对搜索空间结构的影响。

## 6 CONCLUSION

(Prompt: Generalize Section "CONCLUSION" of this article in Chinese)