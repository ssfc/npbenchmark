# Lv's comment

TTP官方提供了许多论文，挑选结果最好的论文进行复现。(2023年5月30日)

### Q: 查找MAPF问题经常投稿的两个会议/期刊，好像是B类？

NB: ICAPS：国际自动规划与调度会议（International Conference on Automated Planning and Scheduling），是自动规划与调度领域的顶级会议，CCF B类。

在熟悉MAPF问题的大邻域搜索和CBS算法后，尝试用强化学习解决MAPF问题。因为MAPF也是一个序列决策的过程，所以适合强化学习。

当约束条件较少时，适合用组合优化的思路（比如local search）解决；当约束条件较多时，适合用强化学习的思路解决。

调查MAPF问题的变种。

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

### Q: MAPF是一个序列决策的过程吗？

根据我的搜索结果，MAPF是一个序列决策的过程，因为它涉及到多个智能体在离散的时间步中做出动作，以达到各自的目标位置，同时避免与其他智能体发生冲突。¹²

MAPF可以看作是一个协同规划问题（Cooperative Planning Problem），其中每个智能体的动作不仅取决于自身的状态，还取决于其他智能体的状态和动作。¹²

MAPF也可以看作是一个分布式约束优化问题（Distributed Constraint Optimization Problem），其中每个智能体需要在满足一些约束的前提下，最小化自身或全局的代价函数。³

Source: Conversation with Bing, 2023/5/30
(1) 多机器人路径规划(MAPF)综述 - 知乎 - 知乎专栏. https://zhuanlan.zhihu.com/p/613639712.
(2) Multi-Agent Pathfinding: Definitions, Variants, and Benchmarks. https://arxiv.org/abs/1906.08291.
(3) Graph-Based Multi-Robot Path Finding and Planning. https://link.springer.com/article/10.1007/s43154-022-00083-8.

### Q: 序列决策的过程适合用强化学习解决吗？

根据我的搜索结果，序列决策的过程适合用强化学习解决，因为强化学习是一种基于奖励的学习方法，可以让智能体在不断与环境交互的过程中，学习到最优的策略，以最大化累积的奖励。¹²³

强化学习可以处理不同类型的序列决策问题，例如：

- 完全可观测的环境，即智能体可以直接感知环境的状态，如围棋、Atari游戏等。
- 部分可观测的环境，即智能体只能感知环境的部分观测，如自动驾驶、打牌等。
- 离散动作空间的环境，即智能体可以采取有限个离散的动作，如走迷宫、下棋等。
- 连续动作空间的环境，即智能体可以采取实值向量的动作，如控制机器人、飞行器等。

强化学习也有一些挑战和难点，例如：

- 探索与利用的平衡，即智能体如何在利用已知的奖励信息和探索未知的状态动作之间做出合理的抉择。
- 奖励延迟和折扣，即智能体如何在近期奖励和远期奖励之间做出权衡，并考虑未来奖励的不确定性和衰减。
- 部分可观测性和记忆，即智能体如何在无法直接感知状态的情况下，利用历史观测和内部状态来做出决策。
- 多智能体协同与竞争，即智能体如何在与其他智能体共享环境的情况下，学习到合作或对抗的策略。

Source: Conversation with Bing, 2023/5/30
(1) CS285深度强化学习笔记2——序列决策问题和imitation learning - 知乎. https://zhuanlan.zhihu.com/p/354748682.
(2) 强化学习笔记2：序列决策(Sequential Decision Making)过程_UQI-LIUWJ的博客-CSDN博客. https://blog.csdn.net/qq_40206371/article/details/119223500.
(3) 强化学习——简介 - 知乎. https://zhuanlan.zhihu.com/p/32409193.

### Q: 什么是序列决策？

序列决策是一种按时间顺序进行一系列决策的动态决策方式，可用于随机性或不确定性的动态系统最优化。¹²

序列决策的过程可以表示为一个决策代理（decision agent）与离散的时间动态系统进行迭代地交互。²

在每个时间步的开始时，系统会处于某种状态。基于代理的决策规则，它会观察当前的状态，并从有限状态集中选择一个动作。然后，动态系统会进入下一个新的状态并获得一个对应的奖励。这样循环进行状态选择，以获得一组最大化收益。²

序列决策问题可以分为不同的类型，例如：

- 完全可观测的环境，即代理可以直接感知环境的状态，如围棋、Atari游戏等。
- 部分可观测的环境，即代理只能感知环境的部分观测，如自动驾驶、打牌等。
- 离散动作空间的环境，即代理可以采取有限个离散的动作，如走迷宫、下棋等。
- 连续动作空间的环境，即代理可以采取实值向量的动作，如控制机器人、飞行器等。

Source: Conversation with Bing, 2023/5/30
(1) CS285深度强化学习笔记2——序列决策问题和imitation learning - 知乎. https://zhuanlan.zhihu.com/p/354748682.
(2) 序贯决策的理解_Coder_Jh的博客-CSDN博客. https://blog.csdn.net/qq_31239371/article/details/115096523.

### Q: 当约束条件较少时，适合用组合优化的思路（比如local search）解决；当约束条件较多时，适合用强化学习的思路解决。对吗？

这个问题没有一个确定的答案，可能取决于具体的问题和方法。但是，根据一些网上的资料²³⁴，我可以给出一些可能的思路：

- 如果约束条件较少，那么组合优化问题可能可以近似为一个连续优化问题，这时可以使用一些基于梯度的方法或者局部搜索的方法来寻找最优解或者次优解。例如，可以使用神经网络来近似一个可微的目标函数，然后用梯度下降法或者其他优化算法来更新神经网络的参数。也可以使用模拟退火、遗传算法、蚁群算法等启发式方法来在解空间中进行局部搜索。
- 如果约束条件较多，那么组合优化问题可能比较复杂和难以求解，这时可以使用一些基于强化学习的方法来学习一个有效的决策策略，从而在每个状态下选择一个最优或者近似最优的动作。例如，可以使用指针网络（Pointer Network）来将组合优化问题建模为一个序列决策问题，然后用策略梯度法或者其他强化学习算法来更新指针网络的参数。也可以使用神经进化或者其他基于群体的算法来进化一个神经网络或者其他可变结构的策略表示。

当然，这些思路并不是唯一的或者最好的，也有一些方法可以结合不同的思路来求解组合优化问题。例如，可以使用元学习或者主动搜索的方法来在运行时调整模型的参数或者结构，以适应不同的问题实例。也可以使用图神经网络或者其他图学习的方法来利用组合优化问题中图结构的信息，以提高模型的性能和泛化能力。

Source: Conversation with Bing, 2023/5/30
(1) AI | 强化学习应用之组合优化 - 知乎 - 知乎专栏. https://zhuanlan.zhihu.com/p/482852140.
(2) 机器(深度)学习求解组合优化问题综述 - 知乎 - 知乎专栏. https://zhuanlan.zhihu.com/p/388463921.
(3) RL4CO，如何用强化学习解决组合优化？ - 知乎 - 知乎专栏. https://zhuanlan.zhihu.com/p/129049617.
(4) 组合优化+强化学习必读论文列表 - 知乎 - 知乎专栏. https://zhuanlan.zhihu.com/p/363995616.
(5) 强化学习如何处理约束及过程代价？ - 知乎. https://www.zhihu.com/question/501151550.

### Q: MAPF问题有哪些变种？

MAPF问题是多智能体路径规划的问题，即如何为多个智能体规划从起点到终点的无冲突的路径。根据不同的假设和目标，MAPF问题有很多变种，例如：

- 匿名MAPF（Anonymous MAPF）：不区分智能体的身份，只要每个智能体都到达一个终点即可¹。
- 分组MAPF（Colored MAPF）：将智能体分为不同的组，每个组有自己的终点集合¹。
- 动态MAPF（Online MAPF）：智能体的起点和终点在运行时动态变化，需要实时更新路径¹²。
- 有向图MAPF（Directed MAPF）：假设图中的边是有向的，即智能体只能沿着边的方向移动³。
- 有容量限制的MAPF（Capacity-Constrained MAPF）：假设图中的顶点和边有容量限制，即不能超过一定数量的智能体同时占用³。
- 有优先级的MAPF（Prioritized MAPF）：假设智能体有不同的优先级，高优先级的智能体可以打断低优先级的智能体的路径³。
- 有时间窗口的MAPF（Time Window MAPF）：假设智能体有不同的时间窗口，即只能在一定时间内到达终点³。

这些只是一些常见的MAPF变种，还有很多其他的变种，可以根据不同的应用场景和需求来设计。

Source: Conversation with Bing, 2023/5/30
(1) *多代理寻径：定义、变体和基准 - 知乎 - 知乎专栏. https://zhuanlan.zhihu.com/p/339098595.
(2) 多机器人路径规划(MAPF)综述 - 知乎 - 知乎专栏. https://zhuanlan.zhihu.com/p/613639712.
(3) 技术解析 | 多智能体路径规划（MAPF） - 知乎 - 知乎专栏. https://zhuanlan.zhihu.com/p/388982360.