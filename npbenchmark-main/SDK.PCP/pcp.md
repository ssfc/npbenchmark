1. Training website: https://suzhouxing.github.io/techive/2020/12/22/Contest-2020pCenter/ 
2. 对于所有NP难问题, brutal force方法都是一种可行的思路, 因此, 尝试实现穷尽所有可能性的排列组合方法; 案例见leetcode: https://leetcode.com/problems/permutations/ 
3. 示例: 
4. Input: 
5. 4(节点数, 0, 1, 2, 3) 2(中心数)
6. 2(顶点0覆盖的元素数量)
7. 0 3(顶点0覆盖的元素编号, 按从近到远顺序排列)
8. 2(顶点1覆盖的元素数量)
9. 1 2(顶点1覆盖的元素编号)
10. 3(顶点2覆盖的元素数量)
11. 1 2 3(顶点2覆盖的元素编号)
12. 2(顶点3覆盖的元素数量)
13. 0 2(顶点3覆盖的元素编号)
14. 5 3(覆盖半径边长序号的上界和下界)
15. 2(表示本次缩小半径将导致2个元素不再被某个集合覆盖)	2(集合2将新增一个无法覆盖的元素, 由于每个集合可覆盖的元素已按从近到远的顺序排序, 所以是3) 3(集合3将新增一个无法覆盖的元素, 由于每个集合可覆盖的元素已按从近到远的顺序排序, 所以是2)
16. 2(表示本次缩小半径将导致2个元素不再被某个集合覆盖)	1(集合1将新增一个无法覆盖的元素, 由于每个集合可覆盖的元素已按从近到远的顺序排序, 所以是2) 2(集合2将新增一个无法覆盖的元素, 由于每个集合可覆盖的元素已按从近到远的顺序排序, 所以是2)
17. Output: 
18. 
19. 
20. Sample of KailinLi: https://github.com/KailinLi/SmartLab-Training/tree/master/PCenter
21. 
22. Interface of SDK PCP: 
23. Function in main.cpp: loadinput, nodeNum, centerNum, coverage, minEdgeLenRank, maxEdgeLenRank, nodesWithDrops; 
24. Function in PCenter.h: nodeNum, centerNum, coverages, nodesWithDrops; 
25. Function in PCenter.cpp: random generator; first coverAllNodesUnderFixedRadius, then reduceRadius and coverAllNodesUnderFixedRadius in for loop; 
26. 