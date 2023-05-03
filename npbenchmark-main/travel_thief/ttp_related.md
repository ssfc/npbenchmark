# Travelling thief problem related papers

### Q: adelaide university是个啥学校？

你好，这是必应。根据我的搜索结果，adelaide university是澳大利亚南澳大利亚州首府阿德莱德市的一所公立大学，也是澳大利亚八大名校之一¹。它成立于1874年，是澳大利亚第三所大学，也是南半球最古老的大学之一¹。它在2023年的泰晤士高等教育世界排名中位居第88位²，在多个学科领域享有国际声誉¹。

Source: Conversation with Bing, 2023/4/25
(1) The University of Adelaide. https://www.adelaide.edu.au/front/international.html.
(2) Current Students | University of Adelaide. https://www.adelaide.edu.au/student/.
(3) International Students | University of Adelaide. http://international.adelaide.edu.au/.

## Travelling thief problem: a transition from theoretical problems to realistic problems

实际的优化问题通常由相互作用的多个问题组成。为了解决这类问题，理解和处理这些相互作用是很重要的。但是，目前的研究文献缺乏系统的方法来处理这些**相互依赖**的问题。
因此，引入了一个新的问题，叫做旅行商偷窃问题（Travelling thief problem, TTP），它更接近实际问题。事实上，这个新问题中包含了实际问题的子组件之间的相互依赖性（这在以前的基准问题中被忽略了）。它是两个最突出的组合优化问题的结合，即旅行商问题（TSP）和背包问题（KP）。这两个问题都在许多理论和实验研究中得到了考虑，已经有非常有效的求解器，可以在各种基准上表现良好。需要注意的是，TTP与绿色物流领域的许多有容量限制的车辆路径规划问题不同。在我们的情况下，我们不仅在路径规划问题中增加了一个负载依赖特性，而且还增加了一个NP难的优化问题，即决定要打包哪些物品。

### Q: This repository has been archived by the owner on Nov 10, 2021. It is now read-only. 是什么意思？

这句话的意思是，这个仓库（repository）已经被所有者（owner）在2021年11月10日归档（archived），现在只能读取（read-only），不能修改或添加内容。仓库是指在GitHub上存储代码或项目的地方，归档是指将仓库设置为只读模式，以表明它不再维护或更新。

| The travelling  thief problem: the first step in the   transition from theoretical problems to realistic problems | [https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2013%20The%20travelling%20thief%20problem--The%20first%20step%20in%20the%20transition%20from%20theoretical%20problems%20to%20realistic%20problems.md](https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2013 The travelling thief problem--The first step in the transition from theoretical problems to realistic problems.md) |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| A Comprehensive Benchmark Set and  Heuristics for the Traveling Thief Problem | [https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2014%20A%20Comprehensive%20Benchmark%20Set%20and%20Heuristics%20for%20the%20Traveling%20Thief%20Problem.md](https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2014 A Comprehensive Benchmark Set and Heuristics for the Traveling Thief Problem.md) |
| Approximate Approaches to the Traveling  Thief Problem       | [https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2015%20Approximate%20Approaches%20to%20the%20Traveling%20Thief%20Problem.md](https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2015 Approximate Approaches to the Traveling Thief Problem.md) |
| Stealing items more efficiently with  ants: a swarm intelligence approach to the travelling thief problem | [https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2017%20Exact%20Approaches%20for%20the%20Travelling%20Thief%20Problem.md](https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2017 Exact Approaches for the Travelling Thief Problem.md) |
| Exact Approaches for the Travelling Thief  Problem           | [https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2017%20Exact%20Approaches%20for%20the%20Travelling%20Thief%20Problem.md](https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2017 Exact Approaches for the Travelling Thief Problem.md) |
| HSEDA: a heuristic selection approach  based on estimation of distribution algorithm for the travelling thief  problem | [https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2017%20HSEDA--a%20heuristic%20selection%20approach%20based%20on%20estimation%20of%20distribution%20algorithm%20for%20the%20travelling%20thief%20problem.md](https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2017 HSEDA--a heuristic selection approach based on estimation of distribution algorithm for the travelling thief problem.md) |
| Multi-objectiveness in the  single-objective traveling thief problem | [https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2017%20HSEDA--a%20heuristic%20selection%20approach%20based%20on%20estimation%20of%20distribution%20algorithm%20for%20the%20travelling%20thief%20problem.md](https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2017 HSEDA--a heuristic selection approach based on estimation of distribution algorithm for the travelling thief problem.md) |
| A case study of algorithm selection for  the traveling thief problem | [https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2018%20A%20case%20study%20of%20algorithm%20selection%20for%20the%20traveling%20thief%20problem.md](https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2018 A case study of algorithm selection for the traveling thief problem.md) |
| A fitness landscape analysis of the  Travelling Thief Problem | [https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2018%20A%20fitness%20landscape%20analysis%20of%20the%20travelling%20thief%20problem.md](https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2018 A fitness landscape analysis of the travelling thief problem.md) |
| A hyperheuristic approach based on  low-level heuristics for the travelling thief problem | [https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2018%20A%20hyperheuristic%20approach%20based%20on%20low-level%20heuristics%20for%20the%20travelling%20thief%20problem.md](https://github.com/ssfc/npbenchmark/blob/main/npbenchmark-main/travel_thief/2018 A hyperheuristic approach based on low-level heuristics for the travelling thief problem.md) |
