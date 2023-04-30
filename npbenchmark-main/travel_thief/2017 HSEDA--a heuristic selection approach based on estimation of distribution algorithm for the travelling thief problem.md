# HSEDA: a heuristic selection approach based on estimation of distribution algorithm for the travelling thief problem

(Prompt: 翻译本文标题)

HSEDA: 一种基于分布估计算法的启发式选择方法，用于解决旅行商盗贼问题

https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2017%20HSEDA--a%20heuristic%20selection%20approach%20based%20on%20estimation%20of%20distribution%20algorithm%20for%20the%20travelling%20thief%20problem.pdf

https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2017%20HSEDA--a%20heuristic%20selection%20approach%20based%20on%20estimation%20of%20distribution%20algorithm%20for%20the%20travelling%20thief%20problem.md

## Abstract

(Prompt: generalize abstract of this article in Chinese)

本文提出了一种基于分布估计算法的启发式选择方法，用于解决旅行商盗贼问题。旅行商盗贼问题是一种新近提出的组合优化问题，它将旅行商问题和背包问题相结合，考虑了两个子问题之间的相互依赖性。本文的方法利用分布估计算法来进化一系列已知的低层启发式，寻找最适合当前问题实例的启发式组合。本文还使用了一种基于径向基函数网络的替代模型，来快速评估分布估计算法采样的新启发式组合。本文将所提出的方法与三种最先进的算法进行了比较，实验结果表明，本文的方法在大多数中等规模的旅行商盗贼问题实例上具有竞争力，优于其他算法。

Keywords: (heuristic selection approach based on estimation of distribution algorithm, HSEDA) => travelling thief problem

### Q: 这篇文章发表会议/期刊属于CCF哪类？这篇文章在google scholar引用次数多少？

A: 这篇文章发表在GECCO 2017会议上，根据CCF推荐国际学术会议目录²，GECCO属于**人工智能**领域的**C类**会议。根据google scholar⁴，这篇文章截至目前被引用了**32次**。

### Q: GECCO是什么会议？

GECCO是**遗传和进化计算会议**的缩写，它是遗传和进化计算领域最大的同行评审会议，也是ACM遗传和进化计算特别兴趣小组(SIGEVO)的主要会议¹。GECCO涵盖了遗传算法、遗传编程、蚁群优化和群智能、复杂系统、进化组合优化和元启发式、进化机器学习、进化多目标优化、进化数值优化等多个主题²。GECCO每年举办一次，2023年将在葡萄牙里斯本举行³。

### Q: Introduce authors of this article in Chinese one by one 

- **Marcella S. R. Martins**，巴西联邦技术大学电子工程系的助理教授，主要研究方向是优化、进化计算和机器学习¹。
- **Mohamed El Yafrani**，摩洛哥拉巴特穆罕默德五世大学科学学院LRIT实验室的副教授，主要研究方向是元启发式算法、进化计算和机器学习²。
- **Myriam R. B. S. Delgado**，巴西联邦技术大学电子工程系的教授，主要研究方向是优化、进化计算和机器学习³。
- **Markus Wagner**，澳大利亚阿德莱德大学优化与物流系的高级讲师，主要研究方向是进化计算、组合优化和多目标优化⁴。
- **Belaïd Ahiod**，摩洛哥拉巴特穆罕默德五世大学科学学院LRIT实验室的教授，主要研究方向是元启发式算法、进化计算和机器学习²。
- **Ricardo Lüders**，巴西联邦技术大学电子工程系的教授，主要研究方向是优化、进化计算和机器学习³。

## 1 INTRODUCTION

(Prompt: Generalize Section "INTRODUCTION" of this article in Chinese)

这篇文章的第一部分介绍了超启发式算法的基本概念和应用。**超启发式算法**是一种高层搜索技术，可以通过其他启发式算法或机器学习技术来自动设计或选择低层启发式算法，以解决各种优化问题。文章介绍了基于概率图模型的估计分布算法（Estimation of Distribution Algorithms, EDA）作为一种超启发式算法的框架，以及基于径向基函数网络（Radial Basis Function Network, RBFN）的替代模型，用于近似目标函数。文章以旅行商窃贼问题（travelling thief problem, TTP）为例，展示了EDA在解决具有多个相互依赖组件的问题方面的潜力。

### Q: 什么是概率图模型的估计分布算法（Estimation of Distribution Algorithms, EDA）?

概率图模型的估计分布算法（Estimation of Distribution Algorithms, EDA）是一种优化算法，它使用概率分布模型来代替传统优化算法中的搜索和评估步骤。EDA算法基于概率图模型，该模型表示问题的变量之间的条件概率分布，并使用该模型生成新的解决方案。

EDA算法通过观察已知最优解的分布来生成解决方案，然后使用这些解决方案来更新概率模型。它的主要思想是通过学习先前生成的解决方案之间的关联性来生成更好的解决方案。EDA算法在优化问题中具有广泛的应用，如组合优化、连续优化、多目标优化和排队论等。

### Q: 什么是概率图模型？

概率图模型（Probabilistic Graphical Models，简称PGMs）是一种用于描述变量间概率关系的工具。它通过图形化的方式表示各变量之间的依赖关系，以及各变量的概率分布。PGMs 可以被用于估计未知参数、进行推理、生成样本等多种任务。常见的 PGMs 包括贝叶斯网络（Bayesian Networks）和马尔可夫网络（Markov Networks），它们是基于概率论、图论和逻辑学等多学科交叉发展而来的。PGMs 在机器学习、人工智能、自然语言处理、计算机视觉等领域都得到了广泛的应用。

### Q: 什么是贝叶斯网络（Bayesian Networks）？

贝叶斯网络（Bayesian Networks）是一种概率图模型，用于建立随机变量之间的因果关系和条件依赖关系。它的基本思想是将一个大型的概率模型分解成一组小的、易于处理的条件概率分布，然后通过组合这些条件概率分布来计算整个模型的概率分布。

贝叶斯网络以一个有向无环图（Directed Acyclic Graph, DAG）的形式表示变量之间的依赖关系，其中每个节点代表一个随机变量，每条边代表一个条件概率分布。每个节点的状态空间包括该变量的所有可能取值，每个节点的条件概率分布描述了该变量在给定其所有父节点的取值时的条件概率。

贝叶斯网络广泛应用于人工智能、机器学习、数据挖掘和决策支持等领域，它们可以用于概率推理、分类、回归、聚类和因果推断等任务。

### Q: 什么是马尔可夫网络（Markov Networks）？

马尔可夫网络（Markov Networks）是一种概率图模型，它表示了一组变量之间的关系，其中变量可以是离散的或连续的，并且它们之间的关系可以用一个无向图来表示。马尔可夫网络假设每个变量的状态只受其邻居节点的状态影响，这与贝叶斯网络中的假设不同。在马尔可夫网络中，每个节点代表一个变量，每个边表示两个变量之间的依赖关系。具体地，如果两个变量之间有边相连，则这两个变量是条件独立的，即给定它们的邻居节点的状态，它们的状态是独立的。

马尔可夫网络可以用概率分布函数表示。给定一个马尔可夫网络，可以通过定义一个势函数来表示联合概率分布。一个势函数是一个非负函数，它的取值表示了某个变量或者变量组合的重要性或者不重要性。给定一个势函数和一组状态，可以计算这组状态的概率分布。通常情况下，势函数会被赋值为指数函数的形式，因为这种形式的函数具有非常好的性质，比如可以方便地计算概率和条件概率。

马尔可夫网络广泛应用于许多领域，如机器学习、计算机视觉、自然语言处理等。在机器学习中，马尔可夫网络被用来建模序列数据，如文本和语音，以及关系数据，如社交网络和推荐系统。在计算机视觉中，马尔可夫网络被用来建模图像和视频数据，以及对它们进行标记和分类。在自然语言处理中，马尔可夫网络被用来建模语言模型、词义消歧等问题。

### Q: 什么是径向基函数网络（Radial Basis Function Network, RBFN）？

径向基函数网络（Radial Basis Function Network, RBFN）是一种人工神经网络，主要用于模式分类和函数逼近。它的特点是使用径向基函数作为激活函数，具有局部逼近和全局逼近能力。RBFN由三层组成：输入层，径向基函数层和输出层。输入层接受输入数据，径向基函数层计算输入和权重之间的相似性，输出层将径向基函数层的结果进行线性组合并输出。在训练过程中，通过最小化预测输出和实际输出之间的误差，优化网络的权重和偏置。RBFN通常用于非线性分类和回归问题。

## 2 BACKGROUND

(Prompt: Generalize Section "BACKGROUND" of this article in Chinese)

这篇文章的背景部分回顾了超启发式算法在组合优化问题中的应用，介绍了（Estimation of Distribution Algorithms, EDA）的基本概念和（Radial Basis Function Network, RBFN）的替代模型，以及旅行商窃贼问题（TTP）的定义。超启发式算法是一种高层搜索技术，可以通过其他启发式算法或机器学习技术来自动设计或选择低层启发式算法。EDA是一种元启发式方法，使用概率图模型来提取和表示高质量问题解的规律，并根据模型生成新的解。RBFN是一种浅层人工神经网络，使用径向基函数作为激活函数，可以用于近似昂贵的目标函数。TTP是一种新近提出的组合优化问题，将旅行商问题和背包问题结合起来，考虑了两个子问题之间的相互依赖性。

## 3 THE PROPOSED APPROACH

(Prompt: Generalize Section "THE PROPOSED APPROACH" of this article in Chinese)

这篇文章的方法部分详细介绍了 (heuristic selection approach based on estimation of distribution algorithm, HSEDA) 框架的主要特点。HSEDA是一种基于 (Estimation of Distribution Algorithms, EDA) 的启发式选择技术，旨在寻找最佳的启发式组合，以解决TTP问题。HSEDA使用整数向量来表示每个解，向量的元素表示应用的低层启发式算法。HSEDA使用贝叶斯网络作为概率图模型，根据当前最优解的集合来估计网络的结构和参数，并从中采样新的解。为了提高计算效率，HSEDA使用RBFN作为替代模型来近似评估新采样的解，然后再用真实的适应度函数重新评估一部分最优的解。HSEDA通过迭代更新概率模型和生成新的解，直到满足终止条件，最后返回最优解。

## 4 EXPERIMENTS

(Prompt: Generalize Section "EXPERIMENTS" of this article in Chinese)

这篇文章的实验部分展示了 (heuristic selection approach based on estimation of distribution algorithm, HSEDA) 与三种其他的TTP算法（MMAS，MA2B和S5）的比较。实验使用了TTP基准实例库中的135个中等规模的实例，考虑了不同的城市数量、背包类型、每城市物品数量和背包容量等参数。实验为HSEDA设置了一些参数，使用irace包进行离线调优，并使用A-test进行统计分析。实验结果表明，HSEDA在大多数实例上表现出色，超过了其他算法，证明了其在解决TTP问题上的有效性和通用性。

## 5 RESULTS AND DISCUSSION

(Prompt: Generalize Section "RESULTS AND DISCUSSION" of this article in Chinese)

这篇文章的结果和讨论部分分析了HSEDA与其他三种算法在不同的TTP实例上的表现。使用Friedman和Dunn-Sidak的后续检验，作者发现HSEDA与其他算法在多数实例上存在显著的统计差异，且HSEDA在大部分中等规模的实例上优于其他算法。作者还探讨了不同算法的特点和优缺点，以及HSEDA的潜力和局限性。作者指出，HSEDA能够**自动选择合适的启发式组合**，适应不同的TTP实例，但也需要进一步改进其概率模型和替代模型，以提高其效率和鲁棒性。

## 6 CONCLUDING REMARKS

(Prompt: Generalize Section "CONCLUDING REMARKS" of this article in Chinese)

这篇文章的结论部分总结了HSEDA的主要思想和优势，即使用EDA作为一种启发式选择技术，结合RBFN作为一种替代模型，来解决具有多个相互依赖组件的TTP问题。作者还提出了一些未来的研究方向，包括使用不同的概率图模型和替代模型，扩展HSEDA到其他多组件问题，以及考虑动态环境和多目标优化的情况。

Keywords: (heuristic selection approach based on estimation of distribution algorithm, HSEDA) => travelling thief problem