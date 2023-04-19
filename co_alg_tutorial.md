# 丁师兄博士论文

# 2 组合优化问题和启发式算法综述

## 2.1 组合优化问题的定义

最优化问题有三个基本要素：变量 (在求解过 程中选定的基本参数称为变量) 、约束 (对变量取值的各种限制称为约束) 和目标函数 (表示可行方案优劣标准的函数) 。

Comment: 原来离散优化就是组合优化, 对立面是连续优化。

启发式算法是一种近似算法。

### Q: 什么是邻域结构？

邻域结构（Neighborhood Structure）是指在一个给定的数据集或空间中，每个数据点都与其周围的数据点相关联的一种关系。具体来说，对于给定的数据点，它的邻居通常被定义为与其距离在某个阈值内的所有其他数据点，这些数据点形成了该数据点的邻域。邻域结构通常用于描述数据之间的相似性或关联性，并且可以用于许多数据挖掘和机器学习算法中，例如聚类、分类、异常检测等。

在实践中，邻域结构可以根据具体问题的需求来定义。例如，在图像处理中，可以将相邻像素的值作为邻域特征来描述每个像素，以便对图像进行分割或分类。在基于距离的聚类算法中，通常使用欧几里得距离或曼哈顿距离等度量方式来定义邻域结构。而在基于图的聚类算法中，则使用图的连接关系来定义邻域结构。

总之，邻域结构是描述数据点之间相互关联的一种方式，它对于很多数据挖掘和机器学习算法都非常重要。定义合适的邻域结构可以提高算法的效率和准确性，从而帮助我们更好地理解和利用数据。

不同的邻域结构对应的局部最优解也不同。

将启发式算法和元启发式算法统一称为启发式算法。

## 2.2 启发式算法的分类

+ 是否从自然界获取灵感：如**遗传**算法，**模拟退火**算法 (是) **禁忌搜索**算法, **迭代局部搜索**算法 (否)
+ 解的数量: 基于**单一解**的启发式算法 (禁忌搜索算法、模拟退火算法、变邻域搜索算法) , 基于**种群**的启发式算法 (进化算法，粒子群算法) 
+ 目标函数是否动态变化: 大部分不变，但是**引导式局部搜索**算法是变的；
+ 邻域结构的数量: 大多数启发式算法作用于一个邻域结构，而**变邻域搜索**算法使用多个邻域结构。
+ 是否重点依赖于**历史信息**: 不依赖或少依赖历史信息的算法，其每一步的动作是通过当前状态决定的。

### Q: 什么是引导式局部搜索算法？

引导式局部搜索算法（Guided Local Search，GLS）是一种启发式局部搜索算法，通常用于解决组合优化问题。与传统的局部搜索算法相比，GLS在搜索空间中引入了一个额外的“**惩罚函数**”，用于指导搜索过程并帮助算法跳出局部最优解。

具体来说，GLS通常包含以下三个步骤：

1. 生成初始解：GLS通常从一个随机解开始，或者从一个较好的初始解开始。
2. 局部搜索：在局部搜索阶段，GLS使用一个邻域结构来生成一组相邻的解，并通过比较邻域内的解的质量来决定如何选择下一个解。这一过程通常重复多次，直到达到一个局部最优解。
3. 引导式搜索：在引导式搜索阶段，GLS通过应用额外的惩罚函数来“引导”算法朝着更优的方向搜索，以跳出局部最优解。这个惩罚函数通常是基于之前搜索过程中找到的解的质量和分布来计算的，以鼓励算法探索那些尚未被搜索的空间。

引导式局部搜索算法可以有效地解决一些NP难问题，例如旅行商问题、装载问题等。相对于其他启发式算法，GLS的优点是具有较好的局部搜索能力，并且在搜索过程中能够通过引入额外的惩罚函数来更好地指导搜索方向，从而避免局部最优解。

### Q: 什么是适应度地形图（Fitness landscape）？

适应度地形图（Fitness landscape）是用于描述优化问题中适应度函数的几何结构的概念。它可以帮助我们更好地理解不同优化算法的性能和搜索空间的性质。

在适应度地形图中，每个点代表一个可能的解，而该点的高度则表示该解的适应度值。因此，适应度地形图是一个以适应度为高度的三维曲面，其形状和结构取决于优化问题的性质和适应度函数的特征。

适应度地形图可以被用来描述优化问题中的全局最优解和局部最优解的分布情况，以及搜索空间中是否存在各种陷阱和平原等特殊结构。此外，它还可以帮助我们更好地理解不同优化算法的搜索策略，以及它们在不同类型的适应度地形图上的表现。

适应度地形图的概念在生物学和进化计算领域得到了广泛应用。在进化计算中，适应度地形图通常用于研究遗传算法和进化策略等算法的搜索性能和局限性。在优化领域中，适应度地形图也被用于描述各种复杂优化问题中的适应度函数结构，以及优化算法在不同类型的适应度地形图上的表现。

### Q: 一个邻域结构对应一个适应度地形图吗？

一个邻域结构通常不会对应一个唯一的适应度地形图，因为适应度地形图的形状和结构取决于优化问题的性质和适应度函数的特征，而不仅仅取决于邻域结构。

不同的邻域结构可以对应同一个适应度地形图，因为不同的邻域结构可能会导致相同的解空间。例如，在一个优化问题中，可以定义基于交换、插入或倒置操作的不同邻域结构，但是这些邻域结构可能会导致相同的解空间和适应度地形图。

此外，同一个邻域结构可能会对应多个不同的适应度地形图，因为适应度地形图的形状和结构取决于适应度函数的特征，而适应度函数可能因问题不同而有所不同。例如，两个优化问题可能具有相同的邻域结构，但它们的适应度函数可能具有不同的形状和特征，因此它们的适应度地形图也会有所不同。

因此，邻域结构和适应度地形图之间的关系是复杂的，它们之间的关系取决于具体的优化问题和适应度函数的特征。

### Q: 什么是变邻域搜索算法？

变邻域搜索算法（Variable Neighborhood Search，简称VNS）是一种用于求解复杂优化问题的启发式搜索算法。它通过在不同的邻域结构上进行搜索，以尝试避免陷入局部最优解并找到全局最优解。

VNS算法的基本思想是在一个初始解的邻域结构中进行搜索，并不断地尝试将解从当前邻域结构中移动到其他邻域结构中，以寻找更优的解。具体地，VNS算法将搜索过程分为多个阶段，每个阶段使用不同的邻域结构进行搜索，以增加搜索的多样性。

在VNS算法的每个阶段中，搜索将在当前邻域结构下进行一定的迭代次数，然后尝试将解从当前邻域结构中移动到下一个邻域结构中。如果移动后的解更优，则继续在新的邻域结构下进行搜索，否则回到当前邻域结构并继续搜索。

VNS算法中的邻域结构可以是任意类型的，例如基于交换、插入、倒置、翻转等操作的邻域结构。在每个阶段中，可以根据当前解的质量和搜索进展情况来调整邻域结构的类型和参数，以适应不同的优化问题和解空间。

VNS算法在求解复杂优化问题时具有较高的效率和鲁棒性，特别是在大规模和高维度的优化问题中表现优异。它已被广泛应用于各种领域，例如物流、制造业、能源、交通等。

## 2.3 各类常见的启发式算法

将单一解和基于种群的启发式算法结合起 来，设计性能更加优越的混合型启发式算法。

### 2.3.1 局部搜索算法

从初始解 出发，在当前解的邻域中，只要发现了比当前解更好的解，就用该解替换掉当前解， 并反复迭代，直到在当前解的邻域内找不到更好的解。

**首次改进**策略是按一定的顺序依次遍历邻域结构，一旦发现某个解比当前解好，就选择这个解。**最好改进**策略是遍历完整个邻域结构，从中选取一个目标函数值最小的解。

### 2.3.2 模拟退火算法

在搜索的**前期**阶段，算法以较大的概率接受随机解，而在**后期**阶段，算法逐渐收敛为迭代改进算法。

模拟退火算法在求解**排课表**问题、**车辆路由**问题、**并行机调度**等经典组合优化问题上具有突出的性能。

### Q: 什么是模拟退火？

模拟退火（Simulated Annealing，SA）是一种用于求解复杂优化问题的元启发式算法，其灵感来源于材料学中的固体退火过程。

SA算法通过模拟材料在高温下退火时的物理过程，从而在解空间中寻找全局最优解。具体地，SA算法从一个初始解开始，按照一定的概率接受劣解，并随着时间的推移逐渐降低接受劣解的概率，最终在温度降至足够低时停止搜索，得到一个接近全局最优解的解。

SA算法的搜索过程可以用以下几个步骤描述：

1. 随机生成一个初始解；
2. 定义一个初始温度，并设置一个温度衰减函数；
3. 在当前温度下，按照一定的概率接受当前解的邻域解（可以是更优或劣的解），并更新当前解；
4. 降低温度，并根据温度衰减函数计算下一次搜索的温度；
5. 重复步骤3和4，直到温度降至足够低或达到最大迭代次数。

SA算法中的温度起到控制搜索方向和探索程度的作用。在高温时，接受劣解的概率较高，搜索方向更随意，有利于避免陷入局部最优解；在低温时，接受劣解的概率较低，搜索方向更趋于稳定，有利于逐渐收敛到全局最优解。

SA算法具有较强的全局搜索能力和鲁棒性，适用于多种类型的优化问题。它已被广泛应用于图像处理、机器学习、物流、制造业、能源等领域。

### Q: 什么是排课表问题？

排课表问题是指在给定一定的课程、教室和教师的情况下，设计一份合理的课程表，以便每门课都能够被安排到相应的教室和时间段，并且每个教师都能够在合适的时间教授相应的课程。

排课表问题通常涉及多个限制条件，包括但不限于以下几个方面：

1. 课程和教室之间的约束：每门课都需要在指定的教室里上课，而每个教室的容量和设施也不同，因此需要考虑教室的可用性和适宜程度。
2. 教师和课程之间的约束：每个教师只能在特定的时间段内教授特定的课程，并且教师在同一时间只能在一个教室上课，因此需要考虑教师的可用性和适宜程度。
3. 学生和课程之间的约束：每门课都需要在特定的时间段内上课，并且需要考虑学生的时间表和学习负担。

排课表问题在实际中应用广泛，例如在学校、大学、培训机构和企业内部等场合，都需要制定合理的排课表，以便确保课程的顺利进行和教学效果的提高。由于排课表问题的复杂性和实用性，它也成为了一个经典的组合优化问题，吸引了众多学者的关注。

### Q: 什么是车辆路由问题？

车辆路由问题（Vehicle Routing Problem，简称VRP）是一种组合优化问题，旨在确定一组车辆的最优路径，以便满足一组客户的需求。在车辆路由问题中，有一组客户需要从一个或多个中心点得到服务，如物流公司的货物配送、快递公司的包裹派送等。

车辆路由问题的目标是最小化车辆的行驶距离或时间，以减少运营成本，同时满足客户需求。在解决车辆路由问题时，需要考虑多个约束条件，例如车辆的数量、容量、速度、时间窗口等。因此，车辆路由问题是一个NP-hard问题，需要采用启发式算法和元启发式算法等高效算法来求解。

车辆路由问题具有广泛的应用场景，例如货物配送、快递配送、公共交通运输等。通过解决车辆路由问题，可以提高配送效率、降低运营成本、减少环境污染等。

### Q: 车辆路由问题是TSP的推广吗？

车辆路由问题（VRP）与旅行商问题（TSP）有相似之处，但并不是TSP的推广。虽然两个问题都涉及到路线规划和最短路径，但是它们有几个本质的区别：

1. 目标不同：TSP的目标是求解所有点之间的最短路径，而VRP的目标是为一组客户分配服务，并且在满足约束条件的情况下，最小化车辆行驶距离或时间。
2. 约束不同：TSP中的约束条件主要包括每个节点只能访问一次、路径必须形成一个回路等，而VRP中的约束条件主要包括车辆数量、容量、速度、时间窗口等。
3. 算法难度不同：TSP是一个NP-hard问题，但它比VRP更容易求解，因为它不需要考虑车辆容量和时间窗口等约束条件。

因此，虽然TSP和VRP有相似的特点，但它们是两个不同的问题。VRP是TSP的扩展，可以看作是TSP的一种变种问题，但是它更具挑战性，需要更复杂的算法来解决。

### Q: 什么是并行机调度问题？

并行机调度问题是指在一组并行机器上调度一组作业以最小化完成所有作业所需的时间。在这种情况下，每个作业可以在一个或多个机器上并行处理，并且每个机器只能同时处理一个作业。这个问题也称为并行任务调度问题或并行作业调度问题。

并行机调度问题是一个NP-hard问题，需要使用启发式算法或精确算法来解决。它的目标是最小化完成所有作业所需的时间，同时满足各个机器的约束条件，如机器容量、处理速度、启动时间等。这个问题具有广泛的应用场景，如计算机集群、生产线、机场登机口等。

在解决并行机调度问题时，需要考虑多个因素，如调度策略、作业的先后顺序、任务分配、负载均衡等。因此，设计高效的并行机调度算法是一项具有挑战性的任务，它可以帮助优化资源利用率，提高系统效率，降低运营成本。

### 2.3.3 禁忌搜索算法

禁忌搜索算法是通过禁忌掉之前已经遍历过的 解来避免迂回搜索，从而跳出局部最优解的陷阱，实现全局寻优能力。

当发现解重复出现的概率较大，即需要引入一定的疏散性时，增加**禁忌步长**；而当发现较长时间没法改进解，这时需要增强算法的集 15 中性，便减小禁忌步长。

通常算法禁忌的是解的一些**属性**而非解本身。这些属性可以是构成解的关键元素、邻域结构的动作等。相比整 个解而言，这些属性更为简单，在保存、查找和对比的过程中需要的时间和空间都 更小，效率更高。

**解禁**策略能够克服这一缺点，它将符合条件的处于禁忌状态的解或动作移出禁忌表，提前结束其禁忌状态。通常使用的解禁条件是某个解比当前邻域中所有的解都好，而且还比搜寻到的历史最优解更好。

如果存在满足解禁策略的解，则算法优先从中选择一个最好的解。如果不存在， 则从处于非禁忌状态的解中选择一个最好的解。

适用于车间作业调度问题、最大覆盖问题、车辆路由问题 (VRP)。

### Q: 什么是最大覆盖问题?

最大覆盖问题（Maximum Coverage Problem）是一类经典的集合覆盖问题，在数学、计算机科学、运筹学等领域中被广泛研究。它的基本形式可以描述为：

假设有一个包含n个元素的集合U，以及一个由m个集合组成的集合族S={S1, S2, …, Sm}，其中每个集合Si是U的子集。现在要从S中选出k个集合，使得它们的并集包含的元素数最多，即最大化|S1 ∪ S2 ∪ ... ∪ Sk|。

最大覆盖问题可以描述许多实际问题，例如在广告投放中选择一定数量的广告覆盖尽可能多的目标用户，或在基因组学中选择一定数量的基因序列以覆盖尽可能多的基因位点。

### 2.3.4 进化算法

集中性机制：在实际应用过程中，使用改进型算法对个体进行集中性搜索。Comment: evolution + local search

疏散性机制：进化算法中的重点和难点是如何有效避免种群趋于**早熟**。

### Q: 什么是文法基因算法？

文法基因算法（Grammar-Based Genetic Algorithm，简称GGA）是一种基于生物遗传学和形式语言理论的进化算法，用于求解复杂的优化问题。它将传统遗传算法的基因编码方式从比特串扩展到文法，通过对产生式规则和符号集的定义和使用，实现了更加高效的搜索和优化过程。

具体来说，文法基因算法的基本思想是将问题的解表示为一个符号串，其中每个符号都属于一个预定义的符号集。这些符号之间的关系由一组产生式规则来定义。在算法的演化过程中，通过交叉、变异等基因操作对符号串进行改变，并根据预定义的适应度函数对生成的个体进行评价和选择。不同于传统的遗传算法，文法基因算法具有自适应的进化机制，可以自动调整符号集和产生式规则，进一步提高搜索效率和求解质量。

文法基因算法在图像处理、机器学习、智能优化等领域得到了广泛应用，尤其在图像识别和分类、网络优化和预测、控制系统设计等方面具有独特的优势。

# 王卓博士论文

# 2 组合优化和启发式算法基础

## 2.1 组合优化

目标函数值域是实数。

组合优化问题的实例通常 不是由直接给出解空间的每一个解以及对应的目标函数值来定义，而是通过一些间 接的更加紧凑的数据形式来表达。

软约束是指尽量满足但不必一定满足的约束。

### Q: 离散数学的大纲？

离散数学是数学的一个分支，涉及离散量和离散结构的研究，包括集合、图论、逻辑、代数、组合数学等方面。离散数学广泛应用于计算机科学、信息技术、通信工程、运筹学等领域。

以下是离散数学的大致内容：

1. 集合论：集合的基本概念、运算、关系，包括子集、幂集、笛卡尔积等；基数、无限集合、基数的运算等。
2. 图论：图的基本概念、表示方法、性质和应用，包括有向图、无向图、带权图、树、网络流等；图的遍历、连通性、最短路径、最小生成树等问题。
3. 代数和组合数学：集合、关系、函数、代数系统的基本概念和性质；排列组合、二项式定理、离散概率等。
4. 逻辑和证明方法：命题逻辑、谓词逻辑、命题的真值表、命题等价、范式等；证明方法、归纳法、反证法、构造法、数学归纳法等。
5. 离散数学的应用：离散数学在计算机科学、信息技术、通信工程、运筹学等领域的应用。

### Q: 组合优化里有哪些涉及离散数学的内容？

组合优化是一个应用广泛的数学领域，其中许多问题涉及离散数学的内容。以下是一些涉及离散数学的组合优化问题：

1. 图论问题：最短路径、最小生成树、网络流、最大匹配等。
2. 组合数学问题：排列组合、分配问题、分组问题、二项式系数、插板法等。
3. 整数规划问题：整数线性规划、混合整数线性规划、旅行商问题等。
4. 集合覆盖问题：最小集合覆盖、最小顶点覆盖、最大独立集、最小支配集等。
5. 排序问题：最长上升子序列、最长公共子序列、最大子段和、0-1背包等。
6. 线性编程问题：线性规划、对偶理论、单纯形法等。
7. 图的着色问题：顶点着色、边着色、区间着色等。

这些问题涉及到离散数学中的图论、组合数学、线性代数、离散数学等方面的知识，对于解决实际问题有着重要的应用价值。

### Q: 组合优化里有哪些不属于离散数学的内容？

组合优化通常涉及到离散数学的知识，但也可能涉及到其他数学领域的知识。以下是一些可能不属于离散数学范畴的组合优化问题和相关领域：

1. 连续优化问题：一些组合优化问题可以被转化为连续优化问题，例如线性规划、二次规划等。因此，这些问题涉及到了连续数学的知识。
2. 概率论和统计学：一些组合优化问题涉及到概率论和统计学的知识，例如蒙特卡罗方法、随机算法、随机图模型等。
3. 计算机科学：一些组合优化问题涉及到计算机科学的知识，例如图论算法、动态规划算法、搜索算法、近似算法等。
4. 运筹学：组合优化通常与运筹学相关，例如线性规划、整数规划、排队论、供应链管理等。这些问题通常涉及到数学优化理论、经济学和工业工程等领域的知识。

总之，组合优化问题通常是跨学科的，需要涉及多个学科的知识和技能。离散数学只是其中的一个组成部分。

### Q: 运筹学是否属于离散数学？

运筹学（Operations Research）通常被认为是应用数学领域的一部分，而不是严格的离散数学领域。运筹学主要研究在资源有限的条件下，如何进行最优的决策，以达到特定的目标和效果。它通常涉及到数学规划、优化理论、排队论、决策分析等多个学科的交叉。

虽然运筹学并不是离散数学的子领域，但离散数学的方法和技术在运筹学中得到了广泛应用，如线性规划、整数规划、图论等。因此，离散数学在运筹学中发挥着重要作用，但运筹学本身并不被视为严格的离散数学领域。

### Q: 什么是组合优化的判定型问题？

组合优化问题可以分为两种类型：**判定**型问题和**优化**型问题。判定型问题是判断一个问题的解是否存在，通常只需要回答“是”或“否”的问题。而优化型问题则是在所有可能的解中寻找最优解，需要找到最小或最大的目标函数值。

在组合优化中，判定型问题通常被用于寻找某些问题的特定性质，如可行性和可达性。这些问题可以通过回答“是”或“否”的问题来解决。例如，判断一个图是否是连通图，判断一个集合是否有某些性质等。

判定型问题通常可以转化为优化型问题，例如将判断图的连通性问题转化为求最小生成树的问题。但是，对于某些问题，判定型问题比优化型问题更加有效，因为在某些情况下，优化型问题的求解复杂度可能非常高。

## 2.2 计算复杂性理论

给定一个组合优化问题的实例，一种自然直观的方法是搜索这个实例的解空间。

为了统一标准，采用算法需要执 行的简单运算的总步数来作为算法的时间复杂度。

算法复杂性研究主要关 心的是时间复杂度的数量级。

时间复杂度大于多项 式的算法称为指数时间复杂度算法，如 O(2^n)、O(n!)。

### 2.2.1 算法时间复杂度

### Q: 时间复杂度低的算法，空间复杂度一定高吗？空间复杂度低的算法，时间复杂度一定高吗？

时间复杂度低的算法不一定空间复杂度高，空间复杂度低的算法也不一定时间复杂度高。在算法设计中，往往需要在时间复杂度和空间复杂度之间进行权衡，选择更加适合实际应用场景的算法。有时候，可以通过牺牲一些空间复杂度来提高时间效率，也可以通过减少空间复杂度来降低时间复杂度。例如，快速排序算法的时间复杂度为 O(nlogn)，空间复杂度为 O(logn)，而插入排序算法的时间复杂度也为 O(nlogn)，但空间复杂度为  O(1)。因此，对于不同的算法问题，需要综合考虑时间复杂度和空间复杂度，并选择最适合的算法。

### 2.2.2 P 与 NP

计算复杂性理论主要是对判定型问题划分复杂类别。

每一个优化型问题，都可以定义相应的判定型问题。

### Q: 什么是图灵机？

图灵机（Turing  machine）是一种抽象的计算模型，由英国数学家图灵于1936年提出。它是一种理论上的计算模型，用来模拟任何计算机算法或者函数的工作原理。图灵机包括一个无限长的纸带、一个可移动的读写头和一套操作规则，可以对纸带上的信息进行读写和处理，并在其中模拟实际计算机的计算过程。图灵机在理论计算机科学中扮演着非常重要的角色，它为计算机科学的发展提供了重要的理论基础。

### Q: 还有不确定型图灵机吗？

是的，不确定型图灵机是一种计算模型，它与确定型图灵机类似，但具有非确定性转移。在不确定型图灵机中，每个状态可以有多个可能的下一个状态，而不像确定型图灵机只有一个下一个状态。因此，不确定型图灵机可以在相同的时间内处理比确定型图灵机更多的计算问题。

### Q: 也就是说，NP问题可以在多项式时间验证解，但不确定能否在多项式时间求解？

是的，这是NP问题的定义。因为如果一个问题的解可以在多项式时间内验证，那么它的解空间的规模通常非常巨大，可能是指数级别的，这就使得在实际应用中难以在多项式时间内求解。因此，NP问题通常被认为是难以在多项式时间内求解的问题。

### Q: NP问题，NPC问题，NPH问题的关系是怎样的？

NP问题、NPC问题、NPH问题的关系是这样的：

- NP问题是指能在多项式时间内验证一个问题的解的正确性的问题。
- NPC问题是指满足两个条件的问题：（1）它是一个NP问题；（2）所有的NP问题都可以在多项式时间内归约到它。
- NPH问题是指满足NPC问题定义的第二条但不一定要满足第一条的问题，即所有的NP问题都可以在多项式时间内归约到它，但它不一定是一个NP问题。
- P问题是指能在多项式时间内解决的问题，它是NP问题的一个子集。
- NPC问题是NP问题的一个子集，也是NPH问题的一个子集。
- NPH问题不一定是NP问题的子集，也不一定是P问题的子集。

用图形表示他们的关系如下：

![P-NP-NPC-NPH](https://img-blog.csdnimg.cn/20191204202428312.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NDgxMDAxNg==,size_16,color_FFFFFF,t_70)

### Q: 也就是说，NPH问题可能无法在多项式时间验证解？

是的，NPH问题不一定是NP问题，所以它可能无法在多项式时间验证解。例如，停机问题就是一个NPH问题，但不是一个NP问题，因为它无法在有限的时间内判断一个程序是否会停止。

### Q: 什么是停机问题？

停机问题（Halting Problem）是指给定一个程序和输入，判断该程序是否会在有限的时间内停机（结束）的问题。它是图灵机理论中的经典问题，也是不可判定问题的典型例子之一。

具体来说，停机问题可以描述为：给定一个程序P和输入I，判断是否存在一个有限的步骤数，使得程序P在给定输入I的情况下停机。停机问题的难点在于无法确定在一个程序的执行过程中，它是否会进入死循环等无限循环的状态，从而无法停机。

图灵在提出停机问题后，证明了不存在一个通用算法可以解决所有停机问题。也就是说，对于任意一个程序和输入，都无法确定程序是否会停机。这个结论被称为“停机问题的不可判定性定理”，是计算理论中的一个重要结论。

### Q: 图着色问题怎么在多项式时间转化为SAT问题？

图着色问题是一个经典的组合优化问题，其判定型问题是给定一个图和一个整数 $k$，判断是否存在一种顶点着色方案，使得相邻顶点颜色不同，且最多使用 $k$ 种颜色。

将图着色问题转化为 SAT 问题的一种方法是，对于给定的图 $G=(V,E)$，定义变量 $x_{v,c}$ 表示顶点 $v$ 被着以颜色 $c$，其中 $1\leq v\leq |V|$，$1\leq c\leq k$。对于每个顶点 $v$，限制其至少被染上一种颜色，即 $\bigvee_{c=1}^k x_{v,c}$。对于每条边 $(u,v)\in E$，限制其两个端点的颜色不相同，即 $\neg x_{u,c} \vee \neg x_{v,c}$，其中 $1\leq c\leq k$。这样，就可以通过 SAT 求解器来求解图着色问题的解。

以上方法的正确性可以通过以下两个方面进行证明：

1. 当存在一种 $k$ 色可行着色方案时，显然对应的变量赋值满足 SAT 问题的约束条件。
2. 当 SAT 求解器返回有解时，可以根据求解器返回的变量赋值构造出对应的着色方案，并验证其合法性。

这种方法的时间复杂度为 $O(n^2k)$，其中 $n=|V|$ 是顶点数，$k$ 是颜色数。因为 $k$ 是一个常数，所以该方法的时间复杂度是多项式级别的，即可以在多项式时间内将图着色问题转化为 SAT 问题。

## 2.3 启发式算法

启发式算法希望在有限的时间内得到 高质量的解，对最优性不提供保证。

启发式算法多受到了生物行为、人类思维、自然现象的启迪。Comment: 怪不得叫启发式。

构造式算法从空开始构造，在每一迭 代步中，算法选取还未添加的某一元素并确定该元素的取值，通过循环迭代不断扩 展不完整的解直到得到最终的完整解。

常见的两种确定顺序的方法是随机方式和打分标准。
