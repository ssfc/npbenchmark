1. 一个vertex改变颜色只会改变与它相邻vertex的相邻edge的冲突值，不会改变不与它相邻的vertex和edge的冲突值，这是因为一个vertex是通过星形的edge嵌入到graph中。因此只需计算这个vertex周边冲突值的变化，即可得出整张graph冲突值的变化。(2022年10月21日)
2. 邻接表和邻接矩阵一样, 储存的是顶点而不是边; 区别在于邻接矩阵用0101储存, 邻接表用链表的形式储存; (2022年11月30日)
3. best move不一定是正向的, 也可能挑出一个最佳的但效果还是不如现在的solution; (2022年12月10日)
4. Search "DSJC500.5.col site:github.com" in google and we can find many sample answers; (2022年11月17日)
5. Old version of dataset can be seen at: https://github.com/albnner/SmartLab/tree/master/Tabucol_GPH it starts with SOURCE: David Johnson (dsj@research.att.com) 
6. Dataset 125.5 has 17 colors, 250.9 has 72 colors, 500.5 has 49 colors; (2022年11月18日)
7. Our running speed is fast enough, the task now is escape local minimum; after changing random seed it become faster; (2022年11月19日)
8. Su tells me that it takes long time to run big dataset, that's why the running starts at 2022-11-19 09:27 and still running at 2022-11-19 19:09 ; (2022年11月19日) 
9. 
10. My DSJC0500.1.txt is 123 604 301 iterations, 100.438 seconds (seed 1);
11. My DSJC0500.1.txt is 91 729 575 iterations, 76.669 seconds (seed 2);
12. My DSJC0500.1.txt is 119 326 345 iterations, 96.6285 seconds (seed 3); 
13. My DSJC0500.1.txt is 78 869 859 iterations, 64.7048 seconds (seed 4); 
14. My DSJC0500.1.txt is 258 364 945 iterations, 212.171 seconds (seed 5); 
15. My DSJC0500.1.txt is 312 726 838 iterations, 256.678 seconds (seed 6); 
16. My DSJC0500.1.txt is 32 209 388 iterations, 28.5888 seconds (seed 7); 
17. My DSJC0500.1.txt is 107 452 109 iterations, 88.9919 seconds (seed 8); 
18. My DSJC0500.1.txt is 417 779 635 iterations, 321.543 seconds (seed 9); 
19. My DSJC0500.1.txt is 28 009 744 iterations, 23.5991 seconds (seed 10); 
20. 
21. My DSJC0500.5.txt is 267 474 167 iterations, 565.158 seconds (seed 1); 
22. My DSJC0500.5.txt is 223 680 573 iterations, 502.478 seconds (seed 2); 
23. My DSJC0500.5.txt is 176 934 040 iterations, 374.859 seconds (seed 3); 
24. My DSJC0500.5.txt is 97 163 848 iterations, 205.569 seconds (seed 4); 
25. My DSJC0500.5.txt is 46 633 598 iterations, 99.9977 seconds (seed 5); 
26. My DSJC0500.5.txt is 12 541 308 iterations, 28.7255 seconds (seed 6); 
27. My DSJC0500.5.txt is 32 686 148 iterations, 71.1719 seconds (seed 7); 
28. My DSJC0500.5.txt is 36 928 629 iterations, 78.049 seconds (seed 8); 
29. My DSJC0500.5.txt is 8 581 771 iterations, 18.1217 seconds (seed 9); 
30. My DSJC0500.5.txt is 39 288 630 iterations, 84.2613 seconds (seed 10); 
31. 
32. 
33. My problem is the great variance of result; (2022年11月19日)
34. 
35. Sample of albnner: https://github.com/albnner/SmartLab 
36. albnner: use global parameters; have HEA, but not good enough; 
37. 
38. Sample of chenfengkg: https://github.com/chenfengkg/tabucol 
39. chenfengkg: use global parameters; not have HEA; 
40. 
41. Sample of CoCoThink: https://github.com/CoCoThink/TabuCol 
42. CoCoThink: NOT use global parameters; not have HEA; (2022年11月18日)
43. Add dataset DSJC125.5.col to CoCoThink: YES, it can run; (2022年11月18日)
44. Test the running time of CoCoThink: 
45. DSJC125.5.col is 178918 iterations, 0.408642 seconds; 
46. DSJC250.9.col is 84698 iterations, 1.34207 seconds; 
47. DSJC500.5.col is 54 756 380 iterations, 1180.34 seconds; 
48. DSJC500.5.col is 48 212 914 iterations, 1033.54 seconds; (replace rand with pseudoRandNumGen) 
49. 
50. Since CoCoThink does not say its running time, try other methods; (2022年11月19日)
51. 
52. Sample of Grasssleeve: https://github.com/Grasssleeve/Graph-Coloring-Problem 
53. Grasssleeve: use global parameters; No response on windows platform; has HEA, report the result; 感觉这个人没有考清楚情况, 又是新策略旧策略, 一些地方又不清楚; 
54. 
55. Sample of jokerpwn: https://github.com/jokerpwn/heuristic-algorithm-learning 
56. jokerpwn: NOT use global parameters; have HEA, not report result; 
57. 
58. Sample of KailinLi: https://github.com/KailinLi/SmartLab-Training/tree/master/GraphColoring 
59. KailinLi: NOT use global parameters; he said he had HEA, we still cannot find; 
60. 
61. Sample of RyleeRuiYe: https://github.com/RyleeRuiYe/k-coloring 
62. RyleeRuiYe: use global parameters; has HEA, not report result; RuiYe's program generate an extremely big file and cannot output result; 
63. 
64. Sample of yuanFang1: https://github.com/yuanFang1/graph_coloring/tree/master/ts_vs/ts_vs 
65. yuanFang1: use global parameters; has HEA, report the result; WTF? Segmentation fault (core dumped) 
66. 