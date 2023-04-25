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