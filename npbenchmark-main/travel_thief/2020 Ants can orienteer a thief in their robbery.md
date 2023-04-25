# Ants can orienteer a thief in their robbery

(Prompt: 翻译本文标题)

蚂蚁可以给小偷定向

https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2020%20Ants%20can%20orienteer%20a%20thief%20in%20their%20robbery.pdf

https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2020%20Ants%20can%20orienteer%20a%20thief%20in%20their%20robbery.md

https://github.com/jonatasbcchagas/aco_thop

## Abstract

(Prompt: generalize abstract of this article in Chinese)

盗贼定向问题（ThOP）是一种多组分问题，结合了两种经典的组合优化问题：定向问题和背包问题。 ThOP具有挑战性，因为它具有给定的时间限制和组件之间的交互。 我们提出了一种基于蚁群优化算法的方法，并提出了一种新的打包启发式方法，以单独和交互地处理问题组件。 我们的方法在超过90％的基准实例上优于现有工作，平均改进超过300％。

Key: ACO => Thief Orienteering Problem

### Q: 这篇文章发表会议/期刊属于CCF哪类？这篇文章在google scholar引用次数多少？

这篇文章发表在2020年的Operations Research Letters期刊上，该期刊属于CCF计算机科学与工程学科的B类期刊¹。根据网上搜索结果，这篇文章在Google Scholar上的引用次数为6次²。

### Q: Operations Research Letters是什么期刊？

Operations Research Letters是一份双月刊的同行评审学术期刊，涵盖了运筹学的各个方面。它于1981年创刊，由Elsevier出版。现任主编是Amy Ward¹。该期刊的目标是快速审查和发表运筹学和分析领域的短文。除了限制在八页以内，质量、原创性、相关性和清晰度是选择发表文章的唯一标准²。

### Q: Introduce authors of this article in Chinese one by one

这篇文章的作者有两位，分别是：

- Jonatas B. C. Chagas，是巴西奥罗普雷托联邦大学（Universidade Federal de Ouro Preto）的计算机系教授，也是维索萨联邦大学（Universidade Federal de Viçosa）的信息系教授。他的研究方向是组合优化、整数规划、元启发式算法等，主要应用于交通、物流、调度等领域。他在国际期刊和会议上发表了多篇论文，也参与了一些竞赛和项目。
- Markus Wagner，是澳大利亚阿德莱德大学（The University of Adelaide）的计算机科学学院教授，也是澳大利亚研究委员会未来研究员（ARC Future Fellow）。他的研究方向是进化计算、优化建模、多目标优化等，主要应用于生态、能源、交通等领域。他在国际期刊和会议上发表了多篇论文，也组织了一些竞赛和研讨会。

## 1 Introduction

(Prompt: Generalize Section "Introduction" of this article in Chinese)

这篇文章是一篇研究小偷定向问题（ThOP）的论文，小偷定向问题是一种结合了定向问题（OP）和背包问题（KP）的多组分问题。小偷定向问题具有挑战性，因为它涉及到给定时间限制和问题组件之间的相互作用。作者提出了一种基于蚁群优化（ACO）的两阶段群智能方法，以及一种新的贪心启发式算法，分别用于解决问题的路径和装包部分。作者通过自动算法配置和广泛的实例测试来评估他们的方法的性能，并与现有的工作进行了比较。作者发现他们的方法在超过90%的基准实例上优于现有的工作，平均改进了超过300%。

## 2 Problem description

(Prompt: Generalize Section "Problem description" of this article in Chinese)

这篇文章详细介绍了小偷定向问题（ThOP）的定义和特点。小偷定向问题是一种涉及到一位小偷在一组城市中偷取物品的组合优化问题。小偷必须从起始城市出发，访问一些城市，偷取一些物品，然后在给定的时间限制内到达终点城市。小偷有一个有限容量的背包，每个物品有一个利润和一个重量。小偷的速度与背包的重量成反比。小偷的目标是在满足背包容量和时间限制的条件下，最大化所偷物品的总利润。作者指出，小偷定向问题与旅行商问题（TSP）和背包问题（KP）有关，但也有一些重要的区别，使得它更具实际意义。作者通过一些例子说明了小偷定向问题的复杂性和挑战性。

## 3 Stealing items with ants

(Prompt: Generalize Section "Stealing items with ants" of this article in Chinese)

## 4 Computational experiments

(Prompt: Generalize Section "Computational experiments" of this article in Chinese)

## 5 Concluding remarks

(Prompt: Generalize Section "Concluding remarks" of this article in Chinese)