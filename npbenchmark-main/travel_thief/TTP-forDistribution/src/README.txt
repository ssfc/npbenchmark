Code for the publication "A Comprehensive Benchmark Set and Heuristics for the Traveling Thief Problem"

Download: http://cs.adelaide.edu.au/~optlog/research/ttp.php

Author: Markus Wagner (wagner@acrocon.com)

If you have any questions, please do not hesitate to contact the author.

-- Details --

Driver.java is the main driver file and contains several examples of how the
code can be used.

The objective function is implemented in TTPInstance.evaluate(...). An 
alternative implementation that is based on Bonyadi's Matlab code can be found 
in ttp.Utils.DriverOld.

ttp.Optimisation contains the iterative heuristics random local search (RLS) and
the (1+1) evolutionary algorithm (EA). Via the parameter "mode", the user can
switch between RLS (mode==1) and EA (mode==2). The switch-case-statements can be
extended easily to incorporate additional heuristics.

ttp.Utils contains several helper function, e.g., a generator for Gridengine 
batch jobs.

instances:
- The Chained Lin-Kernighan heuristic tours in the folder "instances" are based 
on the implementation from http://www.tsp.gatech.edu/concorde/downloads/downloads.htm 
- Exemplary, we include the instances based on the TSP instance a280. All others
can be downloaded from http://cs.adelaide.edu.au/~optlog/CEC2014COMP_Instances/

approach1-10000-600000.txt: a Gridengine batch job file that would execute all
9720 TTP instances once using RLS, and a maximum of "10000 iterations no 
improvment" (up to a total time limit of 600000 ms (=10min)).

本文介绍了“Traveling Thief Problem”的综合基准集和启发式算法。

下载：http://cs.adelaide.edu.au/~optlog/research/ttp.php

作者：Markus Wagner (wagner@acrocon.com)

如果您有任何问题，请随时联系作者。

-- 详细信息 --

Driver.java是主驱动程序文件，包含了如何使用代码的几个示例。

目标函数在TTPInstance.evaluate(...)中实现。基于Bonyadi的Matlab代码的替代实现可以在ttp.Utils.DriverOld中找到。

ttp.Optimisation包含了迭代启发式算法随机局部搜索(RLS)和(1+1)进化算法(EA)。通过参数“mode”，用户可以在RLS(mode==1)和EA(mode==2)之间切换。switch-case语句可以轻松扩展以纳入其他启发式算法。

ttp.Utils包含了几个辅助函数，例如Gridengine批处理作业的生成器。

实例：
- 文件夹“instances”中的链式Lin-Kernighan启发式旅行路线是基于http://www.tsp.gatech.edu/concorde/downloads/downloads.htm中的实现。
- 作为示例，我们包含了基于TSP实例a280的实例。所有其他实例都可以从http://cs.adelaide.edu.au/~optlog/CEC2014COMP_Instances/下载。

approach1-10000-600000.txt：Gridengine批处理作业文件，将使用RLS一次执行所有9720个TTP实例，并且最多“10000次迭代没有改进”（总时间限制为600000毫秒（= 10分钟））。