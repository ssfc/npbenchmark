1. 一个vertex改变颜色只会改变与它相邻vertex的相邻edge的冲突值，不会改变不与它相邻的vertex和edge的冲突值，这是因为一个vertex是通过星形的edge嵌入到graph中。因此只需计算这个vertex周边冲突值的变化，即可得出整张graph冲突值的变化。(2022年10月21日)
2. Search "DSJC500.5.col site:github.com" in google and we can find many sample answers; (2022年11月17日)
3. Old version of dataset can be seen at: https://github.com/albnner/SmartLab/tree/master/Tabucol_GPH it starts with SOURCE: David Johnson (dsj@research.att.com) 
4. Dataset 125.5 has 17 colors, 250.9 has 72 colors, 500.5 has 49 colors; (2022年11月18日)
5. 
6. Sample of albnner: https://github.com/albnner/SmartLab 
7. albnner: use global parameters; 
8. 
9. Sample of chenfengkg: https://github.com/chenfengkg/tabucol 
10. chenfengkg: use global parameters; 
11. 
12. Sample of CoCoThink: https://github.com/CoCoThink/TabuCol 
13. CoCoThink: NOT use global parameters; (2022年11月18日)
14. Add dataset DSJC125.5.col to CoCoThink: YES, it can run; (2022年11月18日)
15. Test the running time of CoCoThink: 
16. DSJC125.5.col is 178918 iterations, 0.408642 seconds; 
17. DSJC250.9.col is 84698 iterations, 1.34207 seconds; 
18. DSJC500.5.col is 54756380 iterations, 1180.34 seconds; far worse than me; 
19. 
20. Sample of Grasssleeve: https://github.com/Grasssleeve/Graph-Coloring-Problem 
21. Grasssleeve: use global parameters; No response on windows platform; 
22. 
23. Sample of jokerpwn: https://github.com/jokerpwn/heuristic-algorithm-learning 
24. jokerpwn: NOT use global parameters; 
25. 
26. Sample of KailinLi: https://github.com/KailinLi/SmartLab-Training/tree/master/GraphColoring 
27. KailinLi: NOT use global parameters; 
28. 
29. Sample of RyleeRuiYe: https://github.com/RyleeRuiYe/k-coloring 
30. RyleeRuiYe: use global parameters; 
31. 
32. Sample of yuanFang1: https://github.com/yuanFang1/graph_coloring/tree/master/ts_vs/ts_vs 
33. yuanFang1: use global parameters; 
34. 