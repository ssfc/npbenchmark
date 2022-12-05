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
22. 评估函数: 当前未覆盖节点的数目, 需要最小化; 类似于机器学习中的损失函数; 
23. Page 14, 贪心算法: 迭代地选择一个候选中心, 该中心能覆盖最多的未覆盖节点; 
24. 
25. 
26. Sample of KailinLi: https://github.com/KailinLi/SmartLab-Training/tree/master/PCenter
27. 
28. Interface of SDK PCP: 
29. Function in main.cpp: loadinput, nodeNum, centerNum, coverage, minEdgeLenRank, maxEdgeLenRank, nodesWithDrops; 
30. Function in PCenter.h: nodeNum, centerNum, coverages, nodesWithDrops; 
31. Function in PCenter.cpp: random generator; first coverAllNodesUnderFixedRadius, then reduceRadius and coverAllNodesUnderFixedRadius in for loop; 
32. 