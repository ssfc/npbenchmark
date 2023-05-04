# The Travelling Thief Problem

[TTP - GECCO 2023 (google.com)](https://sites.google.com/view/ttp-gecco2023/home?pli=1)

## Motivation

实际的优化问题通常包含**多个相互作用的NP难组合优化**问题。这样的多组件优化问题不仅因为包含的难度大的优化问题而难以解决，而且特别是因为不同组件之间的相互依赖。相互依赖使得决策变得复杂，因为每个子问题都会影响其他子问题的解决方案的质量和可行性。当一个子问题通过解决方案构造过程改变另一个子问题使用的数据时，这种影响可能更强。多组件问题的例子有：受装载约束的车辆路径问题，最大化材料利用率同时遵守生产计划，以及在港口重新定位集装箱同时最小化船舶的空闲时间。

这个竞赛的目的是为从事多组件优化问题的计算智能研究者提供一个平台。这个竞赛的主要关注点是**TSP和Knapsack问题的组合**。但是，我们计划在未来几年将这种竞赛形式扩展到更复杂的问题组合（这些问题在过去几十年通常是单独处理的）。

这个竞赛使用的一系列基准测试遵循了“旅行窃贼问题”（Mohammad Reza Bonyadi, Zbigniew Michalewicz, Luigi Barone: free PDF, "The travelling thief problem: The first step in the transition from theoretical problems to realistic problems" (IEEE PDF)）的思想 (already added in mysql)。

https://github.com/ssfc/papers/blob/main/2013%20The%20travelling%20thief%20problem--The%20first%20step%20in%20the%20transition%20from%20theoretical%20problems%20to%20realistic%20problems.md

欧几里得2D旅行推销员问题的实例与0-1背包问题的实例相结合，以反映真实世界问题的一些方面；例如，背包中物品的**总重量影响旅行者的行进速度**。这种引入的相互依赖使得TTP与有容量限制的车辆路径问题实例不同，后者不存在这种相互依赖。关于我们的适应度函数和基准测试实例是如何创建的技术细节，请参见手册（免费PDF）（与“A comprehensive benchmark set and heuristics for the traveling thief problem” (ACM PDF)相同）。

https://github.com/ssfc/papers/blob/main/2014%20A%20comprehensive%20benchmark%20set%20and%20heuristics%20for%20the%20traveling%20thief%20problem.md

## Tracks

将有许多赛道，因为可用的实例（从 https://dl.acm.org/doi/10.1145/2576768.2598249，http://cs.adelaide.edu.au/~optlog/CEC2014COMP_InstancesNew/）变化很大，并且跨越多个数量级。

**赛道1：**分析考生代码产生的结果。无需代码。

**Track** **1.1**：三个小型实例：a280_n279_bounded-strongly-corr_01.ttp、a280_n1395_uncorr-similar-weights_05.ttp、a280_n2790_uncorr_10.ttp

**Track 1.2**：三个中型实例：fnl4461_n4460_bounded-strongly-corr_01.ttp、fnl4461_n22300_uncorr-similar-weights_05.ttp、fnl4461_n44600_uncorr_10.ttp

**Track **1.3**：三个大型实例：pla33810_n33809_bounded-strongly-corr_01.ttp、pla33810_n169045_uncorr-similar-weights_05.ttp、pla33810_n338090_uncorr_10.ttp

请为每个实例仅提供 1 个文件，这意味着您最多提交 9 个文件。

**赛道 2：**代码执行。计算资源：1核（云计算CPU），16GiB RAM，10分钟执行时间。

**轨道** **2.1：**三个小实例

**分会场** **2.2..** 三个中型实例

**分会场** **2.3：**三个大型实例

### Q: 页面中的track是什么意思？

页面中的track是指竞赛的不同类别，根据问题的规模和提交方式进行划分。每个track有不同的评价标准和奖励。

##  Prize

马库斯·瓦格纳提供了1000澳元的现金奖励。阿德里亚诺·托雷斯提供了500澳元的现金奖励，并且还负责与Track 2相关的云计算基础设施的费用。目前正在寻找更多的赞助商。

##  Conference Participation

##  Instances and Code

##  Technical Details and Submission

##  Online leaderboard

##  Important Dates

##  Related Competitions

##  Competition Organizers