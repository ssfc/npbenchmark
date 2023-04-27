# The dynamic travelling thief problem--Benchmarks and performance of evolutionary algorithms

(Prompt: 翻译本文标题)

动态旅行窃贼问题：进化算法的基准和性能

https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2020%20The%20dynamic%20travelling%20thief%20problem--Benchmarks%20and%20performance%20of%20evolutionary%20algorithms.pdf

https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2020%20The%20dynamic%20travelling%20thief%20problem--Benchmarks%20and%20performance%20of%20evolutionary%20algorithms.md

## Abstract

(Prompt: generalize abstract of this article in Chinese)

许多实际优化问题涉及动态和随机的因素。虽然在诸如供应链优化和物流等固有动态领域，存在多个相互作用的组件问题，但大多数关于动态问题的研究都集中在单组件问题上。本文基于旅行窃贼问题，定义了一些场景，以便研究**动态变化**对子组件的影响。我们对72个场景和七种算法进行了研究，结果表明——根据实例、变化的大小和算法组合的不同——重新开始优化或继续使用之前有效的解决方案，两者都有可能是更好的选择。

Keywords: Heuristics => Dynamic Travelling Thief Problem

### Q: 这篇文章发表会议/期刊属于CCF哪类？这篇文章在google scholar引用次数多少？

- 这篇文章发表在ICONIP会议上，ICONIP是国际神经网络信息处理会议的缩写，是人工智能领域的一个重要会议。根据CCF推荐的人工智能领域国际学术会议目录²，ICONIP属于C类会议。
- 这篇文章在Google Scholar上的引用次数为3次³。

### Q: ICONIP是什么会议？

- ICONIP是国际神经网络信息处理会议的缩写，是人工智能领域的一个重要会议。它是亚太神经网络学会（APNNS）组织的年度旗舰会议，旨在促进神经网络和相关领域的研究者、科学家和工业专业人士在亚太地区的交流和合作¹²³。
- 根据CCF推荐的人工智能领域国际学术会议目录²，ICONIP属于C类会议。
- 这篇文章在Google Scholar上的引用次数为3次⁴。

### Q: Introduce authors of this article in Chinese one by one

- **Ragav Sachdeva**，是澳大利亚阿德莱德大学计算机科学学院的博士研究生，主要研究方向是进化计算和动态优化¹²。
- **Frank Neumann**，是澳大利亚阿德莱德大学计算机科学学院的教授，主要研究方向是进化计算、组合优化和生物信息学³ 。
- **Markus Wagner**，是澳大利亚阿德莱德大学计算机科学学院的副教授，主要研究方向是进化计算、多目标优化和实际问题 。

## 1 Introduction

(Prompt: Generalize Section "Introduction" of this article in Chinese)

第一节介绍了动态优化问题的研究背景和动机。文章指出，现实世界中的许多优化问题都涉及动态和随机的组件，而进化算法由于能够适应环境变化而在处理这类问题上具有优势。文章回顾了目前对动态适应度函数和动态约束的研究，以及旅行窃贼问题（TTP）的定义和变体。文章提出了**动态旅行窃贼问题（DynTTP）**的概念，即在TTP中引入物品或城市的可用性变化，从而增加问题的复杂性和实际性。文章概述了本文的主要贡献和结构。

## 2 The Travelling Thief Problem and its Variants

(Prompt: Generalize Section "The Travelling Thief Problem and its Variants" of this article in Chinese)

第二节介绍了旅行窃贼问题（TTP）及其变体的定义和特征。文章详细描述了TTP的组成部分，即旅行商问题（TSP）和背包问题（KP），以及它们之间的相互依赖关系，即小偷的速度取决于背包的重量。文章给出了TTP的解码方式和目标函数，以及一个简单的例子。文章还回顾了TTP的相关文献，包括单目标和多目标的版本，以及不同的算法和评价方法。文章最后提出了动态旅行窃贼问题（DynTTP）的设计思路，即在TTP中引入物品或城市的可用性变化，并给出了两种动态场景的定义。

## 3 Dynamic Travelling Thief Problem (DynTTP)

(Prompt: Generalize Section "Dynamic Travelling Thief Problem (DynTTP)" of this article in Chinese)

第三节介绍了动态旅行窃贼问题（DynTTP）的具体设置和实验设计。文章分别讨论了两种动态场景，即物品可用性切换和城市可用性切换，以及它们对TTP解的影响和处理方法。文章定义了一些重要的概念，如干扰、时期、恢复和重建。文章设计了不同的实验场景，包括不同的干扰比例、时期长度和算法选择。文章还说明了如何使用随机数生成器来保证不同算法在同一实例上观察到相同的动态变化。

## 4 Heuristics for the DynTTP

(Prompt: Generalize Section "Heuristics for the DynTTP" of this article in Chinese)

第四节介绍了用于动态旅行窃贼问题（DynTTP）的七种启发式算法，分别针对物品可用性切换和城市可用性切换两种动态场景。文章分别描述了每种算法的基本思想和实现细节，以及它们在恢复或重建TTP解时的优缺点。文章使用了一些通用的或问题相关的算法构件，如Bitflip、PackIterative、Insertion、CLK和REAm等。文章还讨论了不同算法之间的概念差异和相似性。

## 5 Computational Investigation

(Prompt: Generalize Section "Computational Investigation" of this article in Chinese)

第五节介绍了动态旅行窃贼问题（DynTTP）的计算实验和结果分析。文章使用了九个不同规模和特征的TTP基准实例，以及四种不同的干扰比例，来测试七种启发式算法在十个时期内的表现。文章使用了热图和统计测试来展示和比较不同算法在不同场景下的相对优劣。文章发现，干扰事件对算法性能有显著影响，而不同的情况需要不同的方法。文章还指出了一些有趣的现象和规律，以及未来的研究方向。

## 6 Outlook

(Prompt: Generalize Section "Outlook" of this article in Chinese)

本文的展望部分介绍了未来对动态旅行窃贼问题（DynTTP）的可能的研究方向。文章提出了一些改进和扩展的想法，如使用更具问题特性的算法，使用多目标和协同进化的方法，以及平衡解质量和应对干扰事件的能力。文章还提到了本文的研究与另一篇独立进行的关于多目标动态TTP变体的研究的关系，并表示感谢澳大利亚研究委员会的资助。

Keywords: Heuristics => Dynamic Travelling Thief Problem