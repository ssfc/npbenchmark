# A hyperheuristic approach based on low-level heuristics for the travelling thief problem

(Prompt: 翻译本文标题)

基于低级启发式的超启发式方法，用于旅行窃贼问题

https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2018%20A%20hyperheuristic%20approach%20based%20on%20low-level%20heuristics%20for%20the%20travelling%20thief%20problem.pdf

https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2018%20A%20hyperheuristic%20approach%20based%20on%20low-level%20heuristics%20for%20the%20travelling%20thief%20problem.md

## Abstract

(Prompt: generalize abstract of this article in Chinese)

本文研究了使用超启发式方法来解决旅行窃贼问题（TTP）。TTP是一个多组件问题，它由旅行商问题和背包问题组合而成。本文探讨了使用自动在线启发式选择来寻找不同已知启发式的最佳组合。为此，我们提出了一种基于遗传编程的超启发式方法，称为GPHS*，并将其与最先进的算法进行了比较。实验结果表明，该方法在小型和中型TTP实例上具有竞争力。

Keywords: (genetic programming based hyper-heuristic, GPHS*) => travelling thief problem

### Q: 这篇文章发表会议/期刊属于CCF哪类？这篇文章在google scholar引用次数多少？

这篇文章发表在Genetic Programming and Evolvable Machines这个期刊上，这个期刊没有被CCF收录，所以没有CCF分类。这篇文章在google scholar上的引用次数是35次。

### Q: Genetic Programming and Evolvable Machines是什么期刊？

- 这是一本由Springer出版的期刊，主要报道软件和硬件自动进化方面的创新和重要进展。该期刊涵盖了硬件实现、人工生命、分子计算和新兴计算技术等领域。
- 该期刊在2021年的SCImago Journal Rank (SJR)为0.403，五年影响因子为2.194。它在计算机科学应用、硬件和架构、软件和理论计算机科学等领域分别排名Q3和Q2。

### Q: Introduce authors of this article in Chinese one by one

以下是关于文章的作者的信息：

- Mohamed El Yafrani，摩洛哥拉巴特穆罕默德五世大学LRIT实验室的研究员，主要研究方向是元启发式算法、超启发式算法和进化计算。
- Marcella Martins，巴西巴拉那联邦技术大学的教授，主要研究方向是进化计算、机器学习和优化。
- Markus Wagner，澳大利亚阿德莱德大学计算机科学学院的高级讲师，主要研究方向是进化计算、优化和人工智能。
- Belaïd Ahiod，摩洛哥拉巴特穆罕默德五世大学LRIT实验室的教授，主要研究方向是元启发式算法、超启发式算法和进化计算。
- Myriam Delgado，巴西巴拉那联邦技术大学的教授，主要研究方向是进化计算、机器学习和优化。
- Ricardo Lüders，巴西巴拉那联邦技术大学的教授，主要研究方向是进化计算、机器学习和优化。

## 1 Introduction

(Prompt: Generalize Section "Introduction" of this article in Chinese)

这篇文章研究了超启发式算法在解决旅行商窃贼问题（TTP）中的应用。TTP是一个多组件问题，它由旅行商问题（TSP）和背包问题（KP）组合而成。这种问题更接近于真实的优化问题，因为它们通常包含多个相互依赖的子问题。文章提出了一种基于遗传编程（Genetic Programming, GP）的超启发式算法，称为GPHS*，它能够自动在线地选择低级启发式算法（Low Level Heuristics, LLH），以便找到不同已知启发式算法的最佳组合。文章通过在TTP库的一个子集上进行深入的实验研究，展示了该方法与现有算法的竞争性能。文章还介绍了超启发式算法在组合优化问题中的相关工作，以及TTP及其现有算法的简要历史。

## 2 On the use of hyper-heuristics in combinatorial optimization

(Prompt: Generalize Section "On the use of hyper-heuristics in combinatorial optimization" of this article in Chinese)

这一节介绍了超启发式算法的背景和相关工作。超启发式算法是一类高级搜索技术，旨在自动化启发式设计过程，提高搜索方法的通用性。超启发式算法可以选择或生成低级启发式算法（Low Level Heuristics, LLH），以解决一系列优化问题。它们**在启发式空间上进行搜索，而不是直接在解空间上进行搜索**。超启发式算法可以分为两大类：**启发式选择**方法和**启发式生成**方法。前者从一组预先存在的、通常是广为人知的启发式算法中选择一个适合当前问题状态的LLH；后者使用已知启发式算法的组件，自动设计新的LLH。超启发式算法还可以根据LLH是从头构造解还是修改现有解，以及是否使用在线或离线学习机制，进行进一步的分类。遗传编程（Genetic Programming, GP）是超启发式算法中最常用的方法之一，通常用于启发式生成方法。本文则探索了GP作为启发式选择方法的可能性，并将其应用于旅行商窃贼问题（TTP），这是一个相对新颖的NP难问题。TTP提供了相互依赖的子问题，这是真实应用中经常遇到的问题特征。

### Q: 什么是超启发式？

超启发式（Hyper-heuristics）是一种高级启发式搜索方法，它通过组合多个启发式算法来解决复杂的优化问题。超启发式算法不直接针对特定问题设计解决方案，而是通过**自动选择、调整和组合现有启发式算法**来生成高质量解决方案。

超启发式算法通常包括两个主要阶段。首先，它们使用一组基本启发式算法来生成多个可能的解决方案。然后，这些解决方案被组合成更好的解决方案，以产生更好的结果。这种方法在解决多种优化问题方面非常有用，因为不同的启发式算法对于不同的问题具有不同的适应性和效率。

超启发式算法可以应用于各种不同的问题领域，例如调度、路由、布局、网络设计、机器学习、数据挖掘等。它们已被证明可以有效地解决许多复杂的优化问题，而且通常比单一启发式算法更具有灵活性和鲁棒性。

### Q: 举一个超启发式的例子

一个经典的超启发式算法例子是"HyFlex"框架。HyFlex框架由多个启发式算法组成，这些算法被设计用于解决特定类型的问题。HyFlex框架使用一个元启发式（metaheuristic）算法来选择最合适的启发式算法来解决给定的问题实例。

例如，在解决车间调度问题时，HyFlex框架可能会使用贪心启发式算法、遗传算法、模拟退火算法等多个启发式算法来生成多个解决方案。然后，元启发式算法会选择最好的解决方案，或者根据不同的策略进行解决方案的组合和迭代优化，以生成更好的解决方案。

HyFlex框架可以适用于多种优化问题，例如布局问题、调度问题、路径规划问题等。这种超启发式算法可以提高问题求解的效率和准确性，同时也具有很强的通用性和灵活性。

### Q: 什么是遗传编程？

遗传编程（Genetic Programming）是一种基于遗传算法的机器学习方法，通过对符号表达式的进化来生成计算机程序。它是遗传算法的一种扩展，与遗传算法类似，使用进化操作（如选择、交叉和变异）来不断改进个体的适应性，最终得到最佳解决方案。

在遗传编程中，个体是符号表达式，如树形结构或线性表达式。这些表达式表示计算机程序，可以通过自适应进化来生成最佳解决方案。遗传编程中的进化操作通常包括以下步骤：

1. 初始化：生成一个随机种群。

2. 选择：从种群中选择一部分适应性较好的个体作为父代。

3. 交叉：对父代个体进行交叉操作，生成新的后代个体。

4. 变异：对后代个体进行变异操作，生成更多的个体。

5. 评估：对新个体进行评估，计算适应度并选择下一代个体。

6. 终止：当达到预设的停止条件时，终止进化过程并输出最佳解决方案。

遗传编程可以应用于各种不同的问题领域，如预测、分类、数据挖掘、机器学习等。它具有自适应、高效、灵活的优点，能够生成高度优化的计算机程序。

### Q: 举一个遗传编程的例子

一个典型的遗传编程的例子是使用该技术来生成计算机程序解决回归问题。这里以一个简单的回归问题为例子，来说明遗传编程的应用。

假设我们有一些数据点（x, y），需要拟合一个曲线来预测新的数据点。我们可以使用遗传编程来生成一个计算机程序来完成这个任务。我们可以使用符号表达式作为个体，并使用进化操作来生成和改进这些表达式。例如，我们可以定义一个符号集，其中包括加减乘除等运算符，以及变量x。

在初始化时，我们随机生成一些符号表达式作为种群。在每一代中，我们使用适应性函数来评估每个符号表达式的适应性，例如均方误差（MSE）。

在选择操作中，我们可以使用一些进化策略，如轮盘赌选择、竞赛选择等，来选择适应性较好的个体作为父代。在交叉操作中，我们可以使用一些常见的方法来交换个体的符号子树。在变异操作中，我们可以使用一些随机方法，如随机改变符号子树、随机添加或删除运算符等，来生成更多的个体。

通过不断重复这些进化操作，直到达到预设的停止条件为止，我们可以得到一个最佳的符号表达式，作为生成的计算机程序来完成回归任务。这个程序可以用来预测新的数据点，同时也可以提供洞见和理解数据的方法。

## 3 The travelling thief problem (TTP)

(Prompt: Generalize Section "The travelling thief problem (TTP)" of this article in Chinese)

## 4 The proposed approach

(Prompt: Generalize Section "The proposed approach" of this article in Chinese)

## 5 Experiments and discussion

(Prompt: Generalize Section "Experiments and discussion" of this article in Chinese)

## 6 Conclusions and future work

(Prompt: Generalize Section "Conclusions and future work" of this article in Chinese)