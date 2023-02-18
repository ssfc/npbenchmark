1. Training website: https://suzhouxing.github.io/techive/2020/12/22/Contest-2020pCenter/ 
2. 对于所有NP难问题, brutal force方法都是一种可行的思路, 因此, 尝试实现穷尽所有可能性的排列组合方法; 案例见leetcode: https://leetcode.com/problems/permutations/ 
3. 示例: 
4. 输入的算例文件格式: 
5. 4(节点数N, 0, 1, 2, 3) 2(中心数P)
6. 2(顶点0覆盖的元素数量)
7. 0 3(顶点0覆盖的元素编号, 按从近到远顺序排列)
8. 所覆盖的节点按距离由近到远排序; 不然缩半径时操作会麻烦点, 按距离直接删末尾的就行; 
9. 2(顶点1覆盖的元素数量)
10. 1 2(顶点1覆盖的元素编号)
11. 3(顶点2覆盖的元素数量)
12. 1 2 3(顶点2覆盖的元素编号)
13. 2(顶点3覆盖的元素数量)
14. 0 2(顶点3覆盖的元素编号)
15. 5 3(覆盖半径边长序号的上界和下界)
16. 2(表示本次缩小半径将导致2个元素不再被某个集合覆盖)	2(集合2将新增一个无法覆盖的元素, 由于每个集合可覆盖的元素已按从近到远的顺序排序, 所以是3) 3(集合3将新增一个无法覆盖的元素, 由于每个集合可覆盖的元素已按从近到远的顺序排序, 所以是2)
17. 2(表示本次缩小半径将导致2个元素不再被某个集合覆盖)	1(集合1将新增一个无法覆盖的元素, 由于每个集合可覆盖的元素已按从近到远的顺序排序, 所以是2) 2(集合2将新增一个无法覆盖的元素, 由于每个集合可覆盖的元素已按从近到远的顺序排序, 所以是2)
18. 输出的解文件格式: 
19. 输出 P 个用空白字符 (建议使用换行符) 分隔的整数, 分别表示挑选出的 P 个中心 (集合). 
20. 0 (节点0是中心) 
21. 2 (节点2是中心) 
22. Illustration of slides: 
23. 评估函数: 当前未覆盖节点的数目, 需要最小化; 类似于机器学习中的损失函数; 
24. Page 14, 贪心算法: 迭代地选择一个候选中心, 该中心能覆盖最多的未覆盖节点; 
25. C, 所有中心集合; 
26. V, 所有节点集合; 
27. Ci, 表示在当前覆盖半径内能够服务节点𝑖的一组候选中心集合; 
28. Vj, 表示在当前覆盖半径内候选中心𝑗可以服务的节点集合, Vj={i∈𝑉│j∈Ci}。
29. 
30. 
31. Sample of ChocoFairy: https://github.com/ChocoFairy/P-Center 
32. has PCB3038; (2023年2月18日)
33. 
34. Sample of Edward-Elric233: https://github.com/Edward-Elric233/PCenter/tree/603ee061276e4f99e9e76e57f4cecc1dd16b8aa6
35. 因为算法多次需要计算集合的交集、并集、势、遍历，因此设计一个集合数据结构是算法的核心。作者直接使用boost::dynamic_bitset作为实现集合的数据结构。
36. 关于boost::dynamic_bitset常用操作, 详见 https://izualzhy.cn/boost-dynamic-bitset#25-%E9%9B%86%E5%90%88%E6%93%8D%E4%BD%9C 
37. 
38. Sample of KailinLi: https://github.com/KailinLi/SmartLab-Training/tree/master/PCenter
39. only has pmed; (2023年2月18日)
40. 
41. Sample of zywang624: https://github.com/zywang624/smartlab 
42. has PCB3038; (2023年2月18日)
43. 为了实现集合的交并操作, 王子扬自己写了一个MY_SET: https://github.com/zywang624/smartlab/blob/main/P-center/code/MY_SET 
44. 
45. Interface of SDK PCP: 
46. Function in main.cpp: loadinput, nodeNum, centerNum, coverage, minEdgeLenRank, maxEdgeLenRank, nodesWithDrops; 
47. Function in PCenter.h: nodeNum, centerNum, coverages, nodesWithDrops; 
48. Function in PCenter.cpp: random generator; first coverAllNodesUnderFixedRadius, then reduceRadius and coverAllNodesUnderFixedRadius in for loop; 
49. 