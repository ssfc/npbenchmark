# Evolutionary computation for multicomponent problems: opportunities and future directions

(Prompt: 翻译本文标题)

进化计算用于多组分问题：机遇与未来方向

https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2019%20Evolutionary%20computation%20for%20multicomponent%20problems--opportunities%20and%20future%20directions.pdf

https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2019%20Evolutionary%20computation%20for%20multicomponent%20problems--opportunities%20and%20future%20directions.md

## Abstract

(Prompt: generalize abstract of this article in Chinese)

本文回顾了过去30年进化计算领域为解决难题而做出的各种尝试，探讨了这些尝试是否仍然符合当初的目标，以及现实世界问题的复杂性是否有所增加。我们给出了一些现代实际问题的例子，讨论了它们为什么难以解决，以及它们与进化计算领域研究的问题之间是否存在不匹配。我们鼓励进化计算社区更多地研究多组分问题，因为这些问题更能反映现实世界的复杂性和依赖性。

### Q: 这篇文章发表会议/期刊属于CCF哪类？这篇文章在google scholar引用次数多少？

Optimization in Industry: Present Practices and Future Scopes; 46 times. 

### Q: Optimization in Industry: Present Practices and Future Scopes是什么期刊？

Optimization in Industry: Present Practices and Future Scopes不是一个期刊，而是一本书。这本书由Shubhabrata Datta和J. Paulo Davim编辑，于2019年由Springer出版¹。这本书介绍了不同的方法来解决工业问题，如产品设计，过程优化，质量提升，生产力提高和成本最小化，并且包含了一些经典的和进化的优化工具在工业问题上的应用案例¹²。

### Q: Springer是什么？

Springer 是一个全球性的学术出版社和科技服务提供商，总部位于德国柏林。该公司成立于1842年，拥有超过 2600 多个期刊和出版物，覆盖了自然科学、医学、工程学、计算机科学、社会科学和人文科学等各个领域。Springer 出版的期刊和书籍是世界各地学者和研究人员最为重要的参考文献之一。此外，Springer 还提供一系列科技服务，包括数据库、在线图书馆、会议等。Springer 出版的一些知名期刊包括 Nature、Science China、Communications in Mathematical Physics、Journal of Business Ethics 等。

### Q: Introduce authors of this article in Chinese one by one

这篇文章的作者有四位，分别是：

- Mohammad Reza Bonyadi，是Rio Tinto公司的数据科学主管顾问，也是昆士兰大学先进成像中心的研究员。他的研究方向是优化算法，机器学习，计算智能等¹²。
- Zbigniew Michalewicz，是阿德莱德大学计算机科学系的教授，也是波兰科学院计算机科学研究所，波兰日本信息技术学院和Complexica公司的兼职教授。他的研究方向是进化计算，人工智能，优化等¹²³。
- Frank Neumann，是阿德莱德大学计算机科学系的教授，也是Optimisation and Logistics研究小组的负责人。他的研究方向是进化计算，组合优化，生物信息学等² 。
- Markus Wagner，是阿德莱德大学计算机科学系的副教授，也是Optimisation and Logistics研究小组的成员。他的研究方向是进化计算，多目标优化，软件工程等² 。

## 1 Motivation

(Prompt: Generalize Section "Motivation" of this article in Chinese) 

这一节的主要动机是探讨进化计算（Evolutionary Computation, EC）领域的研究与实际应用之间的差距，以及如何缩小这一差距。作者指出，过去30年来，EC领域的研究者们致力于设计适合解决难题的优化方法，但是在真实世界中很难找到基于EC的应用。作者分析了造成这种不匹配的原因，其中最主要的是：（a）**实验关注单一组件的基准问题，而忽略了真实世界问题的多组件性和相互依赖性**；（b）运筹学方法在工业界占据主导地位。作者认为，如果EC领域想要发展出强大的真实世界应用，就需要更多地研究多组件问题，并且利用EC技术的灵活性来应对这类问题的复杂性。

## 2 Two Examples

(Prompt: Generalize Section "Two Examples" of this article in Chinese)

这一节给出了两个真实世界的多组件问题的例子，分别是水箱运输问题和葡萄酒供应链问题。水箱运输问题涉及到如何选择水箱，如何打包水箱，如何选择基地，以及如何规划运输路线，以最大化运输的总价值。葡萄酒供应链问题涉及到如何决定葡萄的收获时间，如何安排压榨机，如何分配储存罐，以及如何安排装瓶线，以及时地完成订单并最小化无用的储存。这两个问题都表现出了**不同组件之间的依赖性**和相互影响，因此不能简单地将它们分解为单一组件的问题来求解。

## 3 Lessons Learned - Dependencies

(Prompt: Generalize Section "Lessons Learned - Dependencies" of this article in Chinese)

这一节从两个例子中总结了一些关于真实世界多组件问题的特点，主要是组件之间的**依赖性**。作者指出，依赖性是导致多组件问题难以解决的主要原因之一，因为它会影响问题的数据流，噪声传播，模型复杂度，以及瓶颈组件的出现。作者给出了一个定义来刻画组件之间的依赖性，并用一个图来表示两个例子中的依赖关系。作者还讨论了不同的依赖性会导致不同难度的问题，例如铸铜球拼图和旅行窃贼问题。

## 4 Traveling Thief Problem (TTP)

(Prompt: Generalize Section "Traveling Thief Problem (TTP)" of this article in Chinese)

这一节介绍了一个抽象的多组件问题，叫做旅行窃贼问题（TTP）。这个问题是将旅行商问题（TSP）和背包问题（KP）结合起来，形成一个包含两个组件的问题。一个窃贼要从n个城市中偷走m件物品，每个物品有不同的重量和价值，窃贼有一个容量有限的背包。问题是要找到一个最佳的计划，让窃贼恰好访问每个城市一次，并从这些城市中选择物品，以最大化他的总收益。为了使这两个组件相互依赖，作者假设窃贼的当前速度受到背包的当前重量的影响，也就是说，他选择的物品越多，他移动的速度就越慢。而且，窃贼的收益还要减去他完成旅行所需时间的租金。作者讨论了不同的租金和速度函数对TTP难度的影响，并介绍了一些关于TTP的实验结果和分析研究。

## 5 Discussion and Future Directions

(Prompt: Generalize Section "Discussion and Future Directions" of this article in Chinese)

这一节讨论了一些处理多组件问题的挑战和机遇，主要涉及到问题的建模和求解方法。作者指出，多组件问题可以用单一模型或多组件模型来建模，不同的建模方式有各自的优缺点，需要根据问题的特点和现有的知识来选择。作者认为，**进化计算（Evolutionary Computation, EC）**方法在建模多组件问题方面具有很大的灵活性，可以考虑问题的各种因素，如约束，多目标，噪声等。作者建议使用协同协作进化的方法来求解多组件模型，即用多种种群来分别进化每个组件的解，并根据组件之间的依赖性来评估个体的适应度。作者还提出了一些未来的研究方向，如如何判断和度量组件之间的依赖性，如何利用不同的求解器和集成方法来处理不同的组件，以及如何利用双层或多层优化的思想来处理多层次的多组件问题。

### Q: 什么是协同进化算法？

协同进化算法是一种进化计算方法，与遗传算法和遗传规划算法有关。协同进化算法的主要思想是通过将两个或多个进化计算任务联合起来，从而提高各个任务的效率和性能。具体来说，协同进化算法通常包括两个或多个进化过程，每个进化过程都被设计为解决一个独立的优化问题，但是这些问题之间存在一定的依赖关系。

在协同进化算法中，各个进化过程之间的信息交流可以通过多种方式实现。例如，一些算法通过交叉种群的方式，使得各个进化过程的个体能够互相交换信息。另一些算法通过在进化过程中动态调整参数的方式来实现信息交流。总的来说，协同进化算法是一种非常灵活和强大的优化方法，已被广泛应用于许多领域，包括电力系统、网络优化、人工智能等。

### Q: 再举一个协同进化算法的例子

另一个协同进化算法的例子是协同进化粒子群优化算法（Cooperative Coevolutionary Particle Swarm Optimization，CCPSO）。

在这个算法中，将一个复杂的优化问题分解为多个子问题，并分别通过不同的进化过程进行求解。其中，每个子问题的目标函数由一个特定的粒子群模型进行建模，并使用粒子群优化算法对其进行求解。在协同进化过程中，通过交叉验证等技术将各个子问题的最优解整合成为整个问题的最优解。

与传统的粒子群优化算法相比，CCPSO 能够有效避免陷入局部最优解，提高全局搜索能力和求解精度。CCPSO 算法已被广泛应用于各种优化问题中，例如函数优化、组合优化、机器学习等。