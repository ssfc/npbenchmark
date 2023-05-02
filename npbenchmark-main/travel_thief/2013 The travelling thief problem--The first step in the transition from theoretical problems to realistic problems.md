# The travelling thief problem: the first step in the  transition from theoretical problems to realistic  problems

(Prompt: 翻译本文标题)

旅行窃贼问题：从理论问题到现实问题的第一步

https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2013%20The%20travelling%20thief%20problem--The%20first%20step%20in%20the%20transition%20from%20theoretical%20problems%20to%20realistic%20problems.pdf

https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2013%20The%20travelling%20thief%20problem--The%20first%20step%20in%20the%20transition%20from%20theoretical%20problems%20to%20realistic%20problems.md

## Abstract

(Prompt: generalize abstract of this article in Chinese)

本文提出了一个新的基准问题，叫做旅行窃贼问题 (travelling thief problem)，它是两个著名的优化问题，即背包问题和旅行商问题的组合。本文介绍了现实问题的两个主要特征：组合性和相互依赖性。我们认为现实问题通常由两个或多个相互依赖的子问题组成。这种相互依赖性导致了现实问题的复杂性，而当前的基准问题缺乏这种复杂性。本文**定义了一些参数**来生成旅行窃贼问题的两个实例，并详细讨论了这两个实例中相互依赖性引起的复杂性。最后，给出了生成这两个实例的过程。

Keywords: 旅行窃贼问题 (travelling thief problem)

### Q: 这篇文章发表会议/期刊属于CCF哪类？这篇文章在google scholar引用次数多少？

2013年IEEE进化计算大会（CEC）属于CCF C类会议⁴。根据谷歌学术搜索结果，这篇文章在谷歌学术上的引用次数是153。

### Q: IEEE Congress on Evolutionary Computation是什么会议？

IEEE进化计算大会（CEC）是一个国际性的会议，它汇集了来自全球的进化计算和计算智能领域的研究者和实践者。会议的内容包括主题报告、常规和特别会议、教程、竞赛、工作坊和小组讨论¹。该会议由IEEE计算智能学会（IEEE CIS）主办，每年举办一次，不同的年份在不同的国家或地区举行²。该会议是进化计算领域最好的会议之一，但也是盛会型，录取率较高，文章质量参差不齐³。该会议在CCF排名中属于C类会议⁴。

### Q: Introduce authors of this article in Chinese one by one

- Mohammad Reza Bonyadi 是阿德莱德大学计算机科学学院的一名研究生，他的研究方向是元启发式算法和实际问题的优化。
- Zbigniew Michalewicz 是 SolveIT Software 的创始人和首席科学家，也是阿德莱德大学计算机科学学院的教授，以及波兰科学院和波兰日本信息技术学院的客座教授。他是人工智能和优化领域的国际知名专家，著有多本畅销书籍。
- Luigi Barone 是 SolveIT Software 的高级研究员，他的研究兴趣包括元启发式算法、进化计算、机器学习和数据挖掘。他在国际期刊和会议上发表了多篇论文。

## I. INTRODUCTION

(Prompt: Generalize Section "INTRODUCTION" of this article in Chinese)

这篇文章讨论了元启发式方法在解决实际问题时的适用性和局限性，指出了当前的基准问题与真实世界问题之间存在的差距。文章认为，真实世界问题的复杂性不仅来自于问题的规模，还来自于问题的组合和相互依赖性。文章引入了一个新的基准问题，叫做旅行商小偷问题，它是由背包问题和旅行商问题组合而成的，并且这两个子问题之间有不同的相互依赖关系。文章介绍了两种旅行商小偷问题的模型，并讨论了它们的特点和复杂度。最后，文章给出了生成这两种模型的实例的方法。

### Q: 这篇文章有没有给出travelling thief problem的解决方案？还是仅仅提出了这个问题？

(Does this article give a solution to the travelling thief problem? Or just raise this question?)

这篇文章没有给出旅行商小偷问题的解决方案，而只是把它作为一个新的**基准问题**来介绍，它反映了真实世界问题的特征。文章还讨论了由于两个子问题之间的**相互依赖**而产生的一些挑战和复杂性，并建议需要设计新的算法来有效地解决这个问题。文章**没有提出或比较**任何具体的算法来解决旅行商小偷问题，而只是提供了生成问题实例的方法和一些**示例数据集**。

### Q: 什么是benchmark problem?

基准问题是一种用来**评估和比较不同优化算法性能的标准问题**。基准问题通常是一些经典的难解问题，如旅行商问题或背包问题，它们有一些已知的特征和难度。基准问题可以帮助研究者测试和改进他们的算法，也可以帮助用户选择合适的算法来解决他们的实际问题。

## II. TRAVELLING THIEF PROBLEM

(Prompt: Generalize Section "TRAVELLING THIEF PROBLEM" of this article in Chinese)

这一部分介绍了一个新的基准问题，叫做旅行商小偷问题，它是由背包问题和旅行商问题组合而成的。旅行商小偷问题有两个子问题，一个是找到一个**最佳的旅行路线**，另一个是**从每个城市中选择一些物品放入背包**。这两个子问题之间有不同的相互依赖关系，比如**旅行的速度**和**背包的重量**有关，**物品的价值**随着**旅行的时间**而变化。这种相互依赖使得单独解决每个子问题并不有效，而需要同时考虑两个子问题。文章定义了两种旅行商小偷问题的模型，分别有一个目标和两个目标，并给出了相应的参数和目标函数。

## III. MODEL 1: TTP1

(Prompt: Generalize Section "MODEL 1: TTP1" of this article in Chinese)

这一部分介绍了旅行商小偷问题的第一种模型，它有一个目标，就是**最大化总收益**。为了使两个子问题相互依赖，引入了两个新的参数：旅行的速度和背包的租金。**旅行的速度**与**背包的重量**有关，背包越重，速度越慢。**背包的租金**与**旅行的时间**有关，时间越长，租金越高。文章给出了一个具体的例子，并用图表和伪代码说明了如何计算目标函数。文章还分析了两个子问题之间的相互影响，指出单独解决每个子问题并不能保证得到整个问题的最优解。

## IV. MODEL 2: TTP2

(Prompt: Generalize Section "MODEL 2: TTP2" of this article in Chinese)

这一部分介绍了旅行商小偷问题的第二种模型，它有两个目标，就是最大化总价值和最小化旅行时间。为了使两个子问题相互依赖，引入了三个新的参数：旅行的速度、物品的价值随时间下降和一个常数C。**旅行的速度**与**背包的重量**有关，背包越重，速度越慢。**物品的价值随时间下降**，取决于物品在背包中的时间和原始价值。常数C影响了价值下降的速率。文章给出了一个具体的例子，并用图表和伪代码说明了如何计算目标函数。文章还分析了两个子问题之间的相互影响，指出即使找到最短的旅行路线或最优的物品选择方案，也不一定能找到帕累托前沿上的解。因此，这两个子问题需要同时考虑。

### Q: "物品的价值随时间下降"是额外加的设定吗？

是的，这是TTP2模型中的一个设定，用来增加两个子问题之间的相互依赖性。这个设定意味着小偷需要在最短的时间内完成旅行，否则他从城市中偷来的物品会失去价值。这个设定也反映了一些真实世界问题中的情况，比如一些物品有保质期或者需求变化。

## V. PROPOSED GENERATION PROCEDURE

(Prompt: Generalize Section "PROPOSED GENERATION PROCEDURE" of this article in Chinese)

这一部分介绍了如何生成旅行商小偷问题的两种实例。首先，给出了生成TSP和KP子问题的参数的方法，包括城市的数量、距离矩阵、物品的数量、重量、价值和可用性。然后，给出了生成TTP1和TTP2的参数的方法，包括速度范围、背包租金、价值下降率和常数C。最后，给出了一个算法来生成TTP1和TTP2的实例，并提供了一些示例数据。文章还讨论了如何评估不同的算法在解决旅行商小偷问题上的效果，提出了一些评价指标和建议。

## VI. CONCLUSION

(Prompt: Generalize Section "CONCLUSION" of this article in Chinese)

这篇文章提出了一种新的基准问题，叫做旅行商小偷问题 (travelling thief problem)，它是由两个经典的NP难问题组合而成的。这个问题更接近真实世界的问题，因为它包含了两个特征：子问题的组合和相互依赖。文章介绍了两种不同的旅行商小偷问题模型 (TTP1 & TTP2)，分别有不同的参数和目标函数。文章还给出了生成这两种模型实例的方法，并提供了一些示例数据。文章指出，由于子问题之间的相互依赖，单独解决每个子问题并不能保证得到整个问题的最优解，因此需要设计新的算法来同时考虑两个子问题。文章认为，旅行商小偷问题可以作为一种有效的基准问题，用来评估和比较不同的优化算法在解决复杂的真实世界问题上的性能。

Keywords: 旅行窃贼问题 (travelling thief problem)