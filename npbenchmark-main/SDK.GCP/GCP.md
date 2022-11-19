1. 一个vertex改变颜色只会改变与它相邻vertex的相邻edge的冲突值，不会改变不与它相邻的vertex和edge的冲突值，这是因为一个vertex是通过星形的edge嵌入到graph中。因此只需计算这个vertex周边冲突值的变化，即可得出整张graph冲突值的变化。(2022年10月21日)
2. Search "DSJC500.5.col site:github.com" in google and we can find many sample answers; (2022年11月17日)
3. Old version of dataset can be seen at: https://github.com/albnner/SmartLab/tree/master/Tabucol_GPH it starts with SOURCE: David Johnson (dsj@research.att.com) 
4. Dataset 125.5 has 17 colors, 250.9 has 72 colors, 500.5 has 49 colors; (2022年11月18日)
5. Our running speed is fast enough, the task now is escape local minimum; after changing random seed it become faster; (2022年11月19日)
6. Su tells me that it takes long time to run big dataset, that's why the running starts at 2022-11-19 09:27 and still running at 2022-11-19 19:09 ; (2022年11月19日) 
7. 
8. My DSJC0500.1.txt is 123 604 301 iterations, 100.438 seconds (seed 1);
9. My DSJC0500.1.txt is 91 729 575 iterations, 76.669 seconds (seed 2);
10. My DSJC0500.1.txt is 119 326 345 iterations, 96.6285 seconds (seed 3); 
11. My DSJC0500.1.txt is 78 869 859 iterations, 64.7048 seconds (seed 4); 
12. My DSJC0500.1.txt is 258 364 945 iterations, 212.171 seconds (seed 5); 
13. My DSJC0500.1.txt is 312 726 838 iterations, 256.678 seconds (seed 6); 
14. My DSJC0500.1.txt is 32 209 388 iterations, 28.5888 seconds (seed 7); 
15. My DSJC0500.1.txt is 107 452 109 iterations, 88.9919 seconds (seed 8); 
16. My DSJC0500.1.txt is 107 452 109 iterations, 88.9919 seconds (seed 8); 
17. My DSJC0500.1.txt is 417 779 635 iterations, 321.543 seconds (seed 9); 
18. My DSJC0500.1.txt is 28 009 744 iterations, 23.5991 seconds (seed 10); 
19. 
20. My DSJC0500.5.txt is 267 474 167 iterations, 565.158 seconds (seed 1); 
21. My DSJC0500.5.txt is 223 680 573 iterations, 502.478 seconds (seed 2); 
22. My DSJC0500.5.txt is 176 934 040 iterations, 374.859 seconds (seed 3); 
23. My DSJC0500.5.txt is 97 163 848 iterations, 205.569 seconds (seed 4); 
24. My DSJC0500.5.txt is 46 633 598 iterations, 99.9977 seconds (seed 5); 
25. My DSJC0500.5.txt is 12 541 308 iterations, 28.7255 seconds (seed 6); 
26. My DSJC0500.5.txt is 32 686 148 iterations, 71.1719 seconds (seed 7); 
27. My DSJC0500.5.txt is 36 928 629 iterations, 78.049 seconds (seed 8); 
28. My DSJC0500.5.txt is 8 581 771 iterations, 18.1217 seconds (seed 9); 
29. My DSJC0500.5.txt is 39 288 630 iterations, 84.2613 seconds (seed 10); 
30. 
31. 
32. My problem is the great variance of result; (2022年11月19日)
33. 
34. Sample of albnner: https://github.com/albnner/SmartLab 
35. albnner: use global parameters; 
36. 
37. Sample of chenfengkg: https://github.com/chenfengkg/tabucol 
38. chenfengkg: use global parameters; 
39. 
40. Sample of CoCoThink: https://github.com/CoCoThink/TabuCol 
41. CoCoThink: NOT use global parameters; (2022年11月18日)
42. Add dataset DSJC125.5.col to CoCoThink: YES, it can run; (2022年11月18日)
43. Test the running time of CoCoThink: 
44. DSJC125.5.col is 178918 iterations, 0.408642 seconds; 
45. DSJC250.9.col is 84698 iterations, 1.34207 seconds; 
46. DSJC500.5.col is 54 756 380 iterations, 1180.34 seconds; 
47. DSJC500.5.col is 48 212 914 iterations, 1033.54 seconds; (replace rand with pseudoRandNumGen) 
48. 
49. Since CoCoThink does not say its running time, try other methods; (2022年11月19日)
50. 
51. Sample of Grasssleeve: https://github.com/Grasssleeve/Graph-Coloring-Problem 
52. Grasssleeve: use global parameters; No response on windows platform; 
53. 
54. Sample of jokerpwn: https://github.com/jokerpwn/heuristic-algorithm-learning 
55. jokerpwn: NOT use global parameters; 
56. 
57. Sample of KailinLi: https://github.com/KailinLi/SmartLab-Training/tree/master/GraphColoring 
58. KailinLi: NOT use global parameters; 
59. 
60. Sample of RyleeRuiYe: https://github.com/RyleeRuiYe/k-coloring 
61. RyleeRuiYe: use global parameters; 
62. 
63. Sample of yuanFang1: https://github.com/yuanFang1/graph_coloring/tree/master/ts_vs/ts_vs 
64. yuanFang1: use global parameters; 
65. 