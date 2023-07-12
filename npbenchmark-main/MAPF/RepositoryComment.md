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

您好，这是必应。我可以帮您解决这个问题。😊

根据我从网络上搜索到的信息，MAPF问题中agent的体积（或者说直径）是否计算取决于具体的问题模型和假设。不同的MAPF变体可能有不同的对agent体积的考虑方式。例如：

- 在经典的MAPF问题中，通常假设agent是点状的，即没有体积，只占据图中的一个顶点。因此，agent之间的冲突是指两个或多个agent在同一个时间步中占据同一个顶点，或者在相邻的顶点之间交换位置¹²。
- 在基于网格的MAPF问题中，通常假设agent是方形的，即有一定的体积，占据图中的一个或多个单元格。因此，agent之间的冲突是指两个或多个agent在同一个时间步中占据同一个或相邻的单元格，或者在相邻的单元格之间交换位置³ 。（Comment: 显然这是一个基于网格的MAPF, 都在scenario1.yaml中设定GRID_SIZE了）
- 在连续空间中的MAPF问题中，通常假设agent是圆形或其他形状的，即有一定的体积，占据图中的一个连续区域。因此，agent之间的冲突是指两个或多个agent在同一个时间步中相互接触或重叠 。

希望这些信息对您有帮助。如果您还有其他问题，请随时提出。😊

源: 与必应的对话， 2023/7/12
(1) *多代理寻径：定义、变体和基准 - 知乎. https://zhuanlan.zhihu.com/p/339098595.
(2) 技术解析 | 多智能体路径规划（MAPF） - 知乎. https://zhuanlan.zhihu.com/p/388982360.
(3) MAPF笔记1_无法解出的问题的博客-CSDN博客. https://blog.csdn.net/Vincent_Ethan_Lyq/article/details/119322458.