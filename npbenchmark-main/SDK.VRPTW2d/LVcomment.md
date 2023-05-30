# Lv's comment

TTP官方提供了许多论文，挑选结果最好的论文进行复现。(2023年5月30日)

### Q: 查找MAPF问题经常投稿的两个会议/期刊，好像是B类？

NB: ICAPS：国际自动规划与调度会议（International Conference on Automated Planning and Scheduling），是自动规划与调度领域的顶级会议，CCF B类。

在熟悉MAPF问题的大邻域搜索和CBS算法后，尝试用强化学习解决MAPF问题。

### Q: 解决MAPF问题的大邻域搜索方法是什么？

根据我的搜索结果，解决MAPF问题的大邻域搜索方法（Large Neighborhood Search，LNS）是一种基于局部搜索的元启发式算法，可以快速地从一个初始解中修复冲突，得到一个可行解或者一个更优解。⁴

LNS的基本思想是：在每次迭代中，随机地选择一些冲突的智能体，并重新规划它们的路径，以减少或消除冲突。这样，可以在一个较大的邻域中搜索新的解，而不是只在一个较小的邻域中进行微调。⁴⁵⁶

LNS有多种变体，例如MAPF-LNS和MAPF-LNS2，它们分别使用不同的智能体选择策略和路径规划算法。⁴⁵⁶

Source: Conversation with Bing, 2023/5/30
(1) Anytime Multi-Agent Path Finding via Large Neighborhood Search. https://aamas.csc.liv.ac.uk/Proceedings/aamas2021/pdfs/p1581.pdf.
(2) MAPF-LNS2: Fast Repairing for Multi-Agent Path Finding via Large ... - AAAI. https://aaai.org/papers/10256-mapf-lns2-fast-repairing-for-multi-agent-path-finding-via-large-neighborhood-search/.
(3) MAPF-LNS2: fast repairing for Multi-Agent Path Finding via large .... https://research.monash.edu/en/publications/mapf-lns2-fast-repairing-for-multi-agent-path-finding-via-large-n.
(4) Google Maps. https://maps.google.ca/.
(5) My Maps – About – Google Maps. https://www.google.com/maps/about/mymaps/.
(6) Google Maps - Apps on Google Play. https://play.google.com/store/apps/details?id=com.google.android.apps.maps.

### Q: 解决MAPF问题的CBS方法是什么？

根据我的搜索结果，解决MAPF问题的CBS方法（Conflict-Based Search，基于冲突的搜索）是一种最优的混合MAPF算法，由两层搜索过程组成：¹²

- 底层搜索：为每个智能体搜索出一条有效路径，可以使用任意的单智能体路径规划算法，例如A*算法。
- 上层搜索：负责检查路径之间是否有冲突，并选择其中代价最小的分支重新进行底层路径规划，直到找到一个无冲突的全局解或者证明不存在这样的解。

上层搜索使用一种称为约束树（Constraint Tree）的数据结构来存储和管理不同的分支。每个节点包含以下信息：¹²

- 约束（Constraints）：根据冲突生成的限制条件，例如(a_i, v, t)表示在时刻t，智能体a_i不能占据顶点v。
- 全局路径（Solution）：根据当前约束计算出来的每个智能体的路径的集合。
- 全局代价（Cost）：根据全局路径计算出来的总代价，例如总行走时间或总行走距离。

CBS算法的流程如下：¹²

- 初始化约束树为一个空节点。
- 从约束树中选择一个代价最小的叶子节点，如果没有这样的节点，则说明不存在无冲突的全局解，算法终止。
- 根据该节点的约束，为每个智能体进行底层路径规划，得到一个全局路径，并计算其全局代价。
- 检查全局路径是否有冲突，如果没有，则说明找到了一个无冲突的全局最优解，算法终止。
- 如果有冲突，则选择一个冲突(a_i, a_j, v, t)，并生成两个新的约束(a_i, v, t)和(a_j, v, t)。
- 为每个新约束创建一个新的节点，并将其作为当前节点的子节点加入到约束树中，继承当前节点原有的约束，并添加新约束。
- 回到第二步，重复以上过程。

Source: Conversation with Bing, 2023/5/30
(1) CBS(Conflict-Based Search)多机器人路径规划 - 知乎 - 知乎专栏. https://zhuanlan.zhihu.com/p/611361907.
(2) 技术解析 | 多智能体路径规划（MAPF） - 知乎 - 知乎专栏. https://zhuanlan.zhihu.com/p/388982360.
(3) CBS基于冲突的搜索算法流程和核心 - CSDN博客. https://blog.csdn.net/m0_58135773/article/details/124699226.