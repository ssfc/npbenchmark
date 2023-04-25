# A weighted-sum method for solving the bi-objective traveling thief problem

(Prompt: 翻译本文标题)

用加权和法求解双目标旅行窃贼问题

https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2022%20A%20weighted-sum%20method%20for%20solving%20the%20bi-objective%20traveling%20thief%20problem.pdf

https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2022%20A%20weighted-sum%20method%20for%20solving%20the%20bi-objective%20traveling%20thief%20problem.md

https://github.com/jonatasbcchagas/wsm_bittp

## Abstract

(Prompt: generalize abstract of this article in Chinese)

许多实际优化问题有多个相互作用的组成部分。每个部分都可能是一个NP难问题，并且它们可能相互冲突，即一个部分的最优解不一定是另一个部分的最优解。这对于单目标公式化来说是一个挑战，因为每个部分对整体解质量的影响可能因实例而异。在本文中，我们研究了旅行窃贼问题的双目标公式化，它由旅行商问题和背包问题组成。我们提出了一种加权和法，利用现有启发式的随机化版本，超过了近期竞赛中9个实例中的6个的参赛者，并且找到了379个单目标问题实例的新的最优解。

Keywords: weighted-sum method => bi-objective traveling thief problem

### Q: bi-objective traveling thief problem和traveling thief problem有什么区别？

bi-objective traveling thief problem (BITTP)是一种多目标优化问题，它是在traveling thief problem (TTP)的基础上，将其单一目标分解为两个目标：最大化收集物品的利润和最小化旅行的时间。这两个目标是相互冲突的，因为优化一个目标不一定能优化另一个目标。因此，BITTP的求解需要找到一组帕累托最优解，即在目标空间中没有被其他解支配的解。BITTP是一种具有挑战性的多组件问题，它结合了两个经典的NP难问题：旅行商问题（TSP）和背包问题（KP）。BITTP的研究可以帮助理解多组件问题的交互和权衡关系，也可以为实际应用提供启发。

### Q: 这篇文章发表会议/期刊属于CCF哪类？这篇文章在google scholar引用次数多少？

这篇文章发表在Computers & Operations Research (COR) 期刊上，这是一本国际性的期刊，主要涉及计算机和运筹学技术在各个领域的应用问题。根据网络搜索结果²，这本期刊在CCF推荐期刊列表中属于C类，也就是说它是CCF认为值得计算机界研究者们发表研究成果的一个推荐期刊。根据google scholar³，这篇文章截至目前被引用了11次。

### Q: Computers & Operations Research是什么期刊？

Computers & Operations Research (COR) 是一本国际性的期刊，主要涉及计算机和运筹学技术在各个领域的应用问题。根据网络搜索结果¹，这本期刊的影响因子（Impact Factor）是3.424，属于中等水平的期刊。这本期刊的文章涵盖了多种优化方法，如线性规划、非线性规划、组合优化、随机优化、模拟优化、元启发式算法等，以及它们在交通、经济、物流、调度、生产、生态等领域的应用。这本期刊也接受一些综述性质的文章，介绍某个领域的最新进展和研究方向。

### Q: Introduce authors of this article in Chinese one by one

这篇文章的作者有两位，分别是：

- Jonatas B. C. Chagas，是巴西奥罗普雷托联邦大学（Universidade Federal de Ouro Preto）的计算机系教授，也是维索萨联邦大学（Universidade Federal de Viçosa）的信息系教授。他的研究方向是组合优化、整数规划、元启发式算法等，主要应用于交通、物流、调度等领域。他在国际期刊和会议上发表了多篇论文，也参与了一些竞赛和项目。
- Markus Wagner，是澳大利亚阿德莱德大学（The University of Adelaide）的计算机科学学院教授，也是澳大利亚研究委员会未来研究员（ARC Future Fellow）。他的研究方向是进化计算、优化建模、多目标优化等，主要应用于生态、能源、交通等领域。他在国际期刊和会议上发表了多篇论文，也组织了一些竞赛和研讨会。

## 1. Introduction

(Prompt: Generalize Section "Introduction" of this article in Chinese)

本文介绍了一种基于加权和方法的启发式算法，用于解决双目标旅行窃贼问题（BITTP）。BITTP是一种复杂的组合优化问题，它结合了旅行商问题（TSP）和背包问题（KP），并考虑了两者之间的相互影响。本文的主要贡献是：（1）将多目标问题分解为多个单目标问题，利用随机化的启发式策略求解；（2）利用有效的局部搜索算子进行探索和利用，平衡了解空间的多样性和质量；（3）对算法的参数进行了调优和分析，发现了不同参数对算法性能的影响；（4）与文献中的其他算法进行了广泛的比较，证明了本文算法的有效性和优越性。

## 2. Problem definition

(Prompt: Generalize Section "Problem definition" of this article in Chinese)

本文的问题定义是双目标旅行窃贼问题（BITTP），它是一种将旅行商问题（TSP）和背包问题（KP）结合起来的组合优化问题。BITTP的目标是在访问所有城市的同时，最大化收集物品的利润和最小化旅行时间。这两个目标是相互冲突的，因为优化一个目标不一定能优化另一个目标。因此，不存在一个同时优化两个目标的单一解，而是存在一组非劣解，即在目标值上不被任何其他解支配的解。本文用数学公式和约束条件来描述BITTP的可行解空间和目标函数。

## 3. Problem-solving methodology

(Prompt: Generalize Section "Problem-solving methodology" of this article in Chinese)

## 4. Computational experiments

(Prompt: Generalize Section "Computational experiments" of this article in Chinese)

## 5. Conclusions

(Prompt: Generalize Section "Conclusions" of this article in Chinese)

