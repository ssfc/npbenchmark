# Fast heuristics for the multiple traveling thieves problem

(Prompt: 翻译本文标题)

多旅行窃贼问题的快速启发式算法

https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2016%20Fast%20heuristics%20for%20the%20multiple%20traveling%20thieves%20problem.pdf

https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2016%20Fast%20heuristics%20for%20the%20multiple%20traveling%20thieves%20problem.md

## Abstract

(Prompt: generalize abstract of this article in Chinese)

本文讨论了旅行窃贼问题的现有形式的不足之处，并提出了一个放宽的问题版本，允许多个窃贼在不同的城市旅行，目的是最大化他们的总利润。旅行窃贼问题是一个具有挑战性的组合优化问题，近年来受到了广泛的关注。本文还提出了一些快速启发式算法来求解新提出的多旅行窃贼问题，并进行了评估。

Keywords: Fast heuristics => (multiple traveling thieves problem, MTTP)

### Q: 这篇文章发表会议/期刊属于CCF哪类？这篇文章在google scholar引用次数多少？

发表在2016年的GECCO会议上。GECCO是遗传和进化计算领域的国际会议，根据CCF排名³，它属于C类会议。根据谷歌学术⁸，这篇文章在2023年5月1日时被引用了23次。

### Q: GECCO是什么会议？

GECCO是**遗传和进化计算会议**的缩写，它是遗传和进化计算领域最大的同行评审会议，也是ACM遗传和进化计算特别兴趣小组(SIGEVO)的主要会议¹。GECCO涵盖了遗传算法、遗传编程、蚁群优化和群智能、复杂系统、进化组合优化和元启发式、进化机器学习、进化多目标优化、进化数值优化等多个主题²。GECCO每年举办一次，2023年将在葡萄牙里斯本举行³。

### Q: Introduce authors of this article in Chinese one by one

这篇文章的作者有两位，分别是：

- **Shelvin Chand**，他出生于斐济的劳托卡，分别于2013年和2014年在南太平洋大学获得了计算机科学和信息系统的学士学位和计算机科学的硕士学位。2018年，他在澳大利亚新南威尔士大学堪培拉分校获得了计算机科学的博士学位。目前，他是澳大利亚联邦科学与工业研究组织（CSIRO）机器人与自主系统组的博士后研究员。他的研究兴趣包括进化机器人和计算创造力¹²。
- **Markus Wagner**，他是阿德莱德大学计算机科学学院的高级讲师。他的研究兴趣包括进化计算、优化、搜索、元启发式、超启发式、进化机器人、进化艺术和创造力。他曾担任过多个国际会议和期刊的程序委员会成员、主席或编辑³。

## 1. INTRODUCTION

(Prompt: Generalize Section "INTRODUCTION" of this article in Chinese)

这篇文章介绍了一种新的组合优化问题，叫做多旅行窃贼问题（MTTP），它是旅行窃贼问题（TTP）的一个变体。TTP是一个将旅行商问题（TSP）和背包问题（KP）结合起来的挑战性问题，其中一个窃贼需要在访问所有城市的同时，选择一些有价值的物品带走。MTTP放宽了TTP的两个假设：只有一个窃贼和必须访问所有城市。MTTP允许多个窃贼参与，且不一定要访问每个城市，从而使问题更接近真实场景。作者讨论了MTTP的定义和特点，并提出了一些启发式算法来求解这个新问题。作者还利用现有的TTP实例来评估算法的性能，并分析了不同因素对结果的影响。

## 2. TRAVELING THIEF PROBLEM

(Prompt: Generalize Section "TRAVELING THIEF PROBLEM" of this article in Chinese)

## 3. PROPOSING A NEW VARIANT OF THE TRAVELING THIEF PROBLEM

(Prompt: Generalize Section "PROPOSING A NEW VARIANT OF THE TRAVELING THIEF PROBLEM" of this article in Chinese)

## 4. MTTP LOCAL SEARCH ROUTINES

(Prompt: Generalize Section "MTTP LOCAL SEARCH ROUTINES" of this article in Chinese)

## 5. LOCAL SEARCH ALGORITHMS

(Prompt: Generalize Section "LOCAL SEARCH ALGORITHMS" of this article in Chinese)

## 6. CONCLUDING REMARKS

(Prompt: Generalize Section "CONCLUDING REMARKS" of this article in Chinese)