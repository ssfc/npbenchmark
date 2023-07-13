# enginbaglayici (c++)

https://github.com/enginbaglayici/ConflictBasedSearch

## Problems

这个仓库的问题是没有解释输入instances中各个数字的含义。

# GavinPHR (python)

https://github.com/GavinPHR/Multi-Agent-Path-Finding

这个是CBS方法下star最多的repository. 而且有可视化工具。

具体instances其实就是visualization文件夹中的scenario1.yaml。

## Problem

CV库安装不上。

这个repository要计算agent的体积，这个是否是普遍现象？

### Q: 一般MAPF问题中agent的体积（或者说直径）是否计算？

MAPF问题中agent的体积（或者说直径）是否计算取决于具体的问题模型和假设。不同的MAPF变体可能有不同的对agent体积的考虑方式。例如：

- 在经典的MAPF问题中，通常假设agent是点状的，即没有体积，只占据图中的一个顶点。因此，agent之间的冲突是指两个或多个agent在同一个时间步中占据同一个顶点，或者在相邻的顶点之间交换位置¹²。
- 在基于网格的MAPF问题中，通常假设agent是方形的，即有一定的体积，占据图中的一个或多个单元格。因此，agent之间的冲突是指两个或多个agent在同一个时间步中占据同一个或相邻的单元格，或者在相邻的单元格之间交换位置³ 。（Comment: 显然这是一个基于网格的MAPF, 都在scenario1.yaml中设定GRID_SIZE了，所以才要进一步设定ROBOT_RADIUS）
- 在连续空间中的MAPF问题中，通常假设agent是圆形或其他形状的，即有一定的体积，占据图中的一个连续区域。因此，agent之间的冲突是指两个或多个agent在同一个时间步中相互接触或重叠 。

Comment: 大多数MAPF是网格地图，agent只占用一格不算体积。

源: 与必应的对话， 2023/7/12
(1) *多代理寻径：定义、变体和基准 - 知乎. https://zhuanlan.zhihu.com/p/339098595.
(2) 技术解析 | 多智能体路径规划（MAPF） - 知乎. https://zhuanlan.zhihu.com/p/388982360.
(3) MAPF笔记1_无法解出的问题的博客-CSDN博客. https://blog.csdn.net/Vincent_Ethan_Lyq/article/details/119322458.

# gavincangan (python)

https://github.com/gavincangan/multiagent-pathfinding

## 问题

我日，居然是用python2写的。

# Jiaoyang-Li (cpp)

https://github.com/Jiaoyang-Li/CBSH2

不错，在Linux环境下能跑了。原来作者贴心地准备了cmakeList.txt。

## 问题

实现的不是传统CBS，而是CBSH

# roboticist-whf (matlab)

https://github.com/roboticist-whf/Multi-agent-Path-Finding-in-Warehouse

## 问题

matlab写的转化不方便吧，还得去安装matlab. 

# gloriyo (python)

https://github.com/gloriyo/MAPF-ICBS

哈哈哈终于可以运行了。(2023年7月12日)

## 问题

# PathPlanning (c++)

https://github.com/PathPlanning/Continuous-CBS

## 问题

实现的不是传统CBS, 而是CCBS。

# roboticist-whf (matlab)

https://github.com/roboticist-whf/multi-agent-path-finding

## 问题

matlab写的转化不方便吧，还得去安装matlab. 这个人是不是特别喜欢用matlab? 

# baharsevket

https://github.com/baharsevket/Conflict-Based-Search-for-Multi-Agent-Pathfinding

这个project的读文件格式和enginbaglayici (c++)雷同，或许可以帮助吾人理解c++实现的CBS。

## 问题
