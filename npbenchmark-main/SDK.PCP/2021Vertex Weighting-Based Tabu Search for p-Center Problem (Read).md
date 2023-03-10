# Vertex Weighting-Based Tabu Search for p-Center Problem
## Abstract
p中心问题包括从一组候选项中选择p个中心，以最大程度地减少**任何客户端与其指定中心之间的最大距离**。将p中心问题转化为一系列覆盖子问题，并提出了一种基于顶点加权的禁忌搜索（VWTS）算法来解决这些问题。**顶点权重技术**和**禁忌搜索**策略。

## 1 Introduction
1. 如果中心为客户服务，则存在服务弧，最长服务弧的长度为覆盖半径。 这个问题的目的是使覆盖半径最小。(2023年2月9日)
2. VWTS结合了顶点加权技术和禁忌搜索; 将p中心问题转换为一系列集合覆盖问题。对于每个可能的覆盖半径r，它检查覆盖范围r内的任何中心是否可以服务每个客户。(2023年2月16日)

## 2 Problem Description and Transformation
1.	Soluton = {X(哪些点被选为中心), Y(每个点被哪个中心服务), r(最小化最大服务半径)}
2. Eq 1: 
3. (PC) min r: 
4. PC: P-center; 
5. r: upper bound of the covering radius; 
6. 
7. Eq 2: 
8. sum(j属于C, xj) <= p 
9. j: 中心序号; 
10. C: 中心集; 
11. x(j): center j在最终解就是1, 不在就是0; 
12. p: 最终解的中心数;  
13. Meaning: 指示某个center是否在最终解中; (2023年2月18日)
14. 
15. Eq 3: 
16. sum(j属于C, yij) = 1 , i属于V; 
17. i: 顶点序号;
18. V: 顶点集; 
19. j: 中心序号; 
20. C: 中心集; 
21. y(i, j): 顶点i被中心j服务就是1, 否则就是0; 
22. Meaning: 指示某个顶点是否被某个中心选择; (2023年2月9日)
23. 
24. Eq 4: 
25. yij <= xj, i属于V, j属于C; 
26. i: 顶点序号; 
27. V: 顶点集; 
28. j: 中心序号;
29. C: 中心集; 
30. 如果某个中心被某个顶点选择, 则它必然在解中; (2023年2月9日)
31. 
32. Eq 5: 
33. sum(j属于C, dij*yij <= r), i属于V;
34. i: 顶点序号; 
35. V: 顶点集; 
36. j: 中心序号; 
37. C: 中心集; 
38. d(i, j): 顶点i和中心j的距离; 
39. y(i, j): 顶点i被中心j服务就是1, 否则就是0; 
40. Meaning: 某个顶点和它服务中心的距离小于等于r; (2023年2月9日)
41. 
42. Eq 6: 
43. xj, yij属于{0, 1}, r属于R+, i属于V, j属于C; 
44. i: 顶点序号; 
45. V: 顶点集; 
46. j: 中心序号;
47. C: 中心集; 
48. x(j): 中心j在最终解就是1, 不在就是0; 
49. y(i, j): 顶点i被中心j服务就是1, 否则就是0; 
50. r: 最大服务半径; 
51. 
52. 显然，最佳覆盖半径必须与某个边dij的长度相同。 因此，令Γ={r1，r2，...，rk} 是不同边长的有序列表; 寻求最小边长rq; (2023年2月9日)
53. 
54. Eq 7: 
55. (SCq)  min sum(i属于V, ui); 
56. SC: set cover problem; 
57. q: 目前半径在边长集合中的序号; 
58. i: 顶点序号; 
59. V: 顶点集; 
60. u(i): 如果顶点i没有被任何中心覆盖就是1, 否则就是0; u是uncovered的首字母; 
61. 打开等式中所示的 p个中心来最小化未发现的客户的数量; (2023年2月9日)
62. 
63. Eq 8: 
64. sum(j属于C && dij<=rq, xj) >= 1-ui, i属于V; 
65. i: 顶点序号; 
66. V: 顶点集; 
67. j: 中心序号; 
68. C: 中心集; 
69. d(i, j): 顶点i和中心j的距离; 
70. rq: 当前最大服务半径; 
71. x(j): 中心j在最终解就是1, 不在就是0; 
72. u(i): 如果顶点i没有被任何中心覆盖就是1, 否则就是0; 
73. Meaning: 如果顶点i被覆盖, 则在以顶点i为圆心的最大服务半径内, 至少有一个中心; 
74. 需要遍历不同的边长列表Γ来检查每个可能的半径; (2023年2月9日)
75. 
76. Eq 9: 
77. sum(j属于C, xj) = p; 
78. j: 中心序号; 
79. C: 中心集; 
80. x(j): 中心j在最终解就是1, 不在就是0; 
81. p: 选择的center数量; 
82. 
83. Eq 10: 
84. xj属于{0, 1}, ui属于{0, 1}, i属于V, j属于C;
85. j: 中心序号; 
86. x(j): 中心j在最终解就是1, 不在就是0; 
87. i: 顶点序号; 
88. u(i): 如果顶点i没有被任何中心覆盖就是1, 否则就是0; 
89. Meaning: 解释变量含义; (2023年2月18日)
90. 

## 3 Vertex Weighting-Based Tabu Search
```c++
// Algorithm 1; 
// Input symbol: 
// G: graph; 
// p: center number;
// rq: covering radius; 

// Output symbol:
// X*: history best solution; 

// A1 LINE 1: /* (Section 3.1) */
X <- init(G, p, rq);
// X: initial solution generated;
// G: graph; 
// p: centers;
// rq: 最小化的最大服务半径; 
// Meaning: generates an initial solution X by a greedy algorithm; (2023年2月10日)
 
// A1 LINE 2:
X* <- X,
// X*: history best solution; 
// X: initial solution generated (a set of centers);
 
X_prev <- X; 
// X_prev: solution of the previous iteration; 
// X: initial solution generated;

tabu list TL <- NULL; 
// TL: tabu list; 
 
iter <- 1;
// iter: current iteration; 

// A1 LINE 3: /* (Section 3.2) */
for all i属于V do
    Vertex weights wi <- 1, 
// i: 顶点序号; 
// wi: vertex weights; 
// V: vertex set;  

// A1 LINE 4: 
while termination condition is not met do
// Meaning: iteratively improves the incumbent solution by a tabu search procedure; (2023年2月10日)

// A1 LINE 5: 
(i, j) <- FindPair(X_prev, TL, iter) /* (Algorithm 2) */
// (i, j): pair swapped
// X_prev: solution of the previous iteration; 
// TL: tabu list;
// iter: current iteration; 
// Meaning: find_move; evaluates the neighborhood of the current solution and records the best neighborhood move while respecting their tabu states; (2023年2月10日)
 
// A1 LINE 6: 
MakeMove(i, j) /* (Algorithm 4) */
// (i,j): pair moved found in the previous; 
// Meaning: makes the best move; (2023年2月10日)

// A1 LINE 7: 
if |U(X)| < |U(X*)| then
// X: current solution; 
// |U(X)|: the set of clients uncovered by X; 
// X*: history best solution; 
// |U(X*)|: the set of clients uncovered by X*; 
// Meaning: If the current solution X improves the best solution found so far
 
// A1 LINE 8: 
X* <- X
// X*: history best solution; 
// X: current solution; 
// Meaning: then X* is updated with X; 

// A1 LINE 9: 
else if |U(X)| >= |U(X_prev)| then
// X: current solution; 
// |U(X)|: the set of clients uncovered by X; 
// X_prev: solution of the previous iteration; 
// |U(X')|: the set of clients uncovered by X'; 
// Meaning: the best move returned by function FindPair() cannot reduce the number of uncovered clients; (2023年2月17日)
 
// A1 LINE 10: 
/* (Section 3.2) */
for v belongs to U(X) do 
    wv <- wv + 1
// wv: weight of vertex that uncovered by X;
// Meaning 1: If stagnation (停滞) occurs, the weight of each uncovered client is adjusted; (2023年2月17日)
// Meaning 2: when the tabu search is **trapped in local optimal** solution X, the VWTS algorithm increases the weight wi of each uncovered client i属于U(X) by one unit; (2023年2月18日)
// Meaning 3: prevent vertices from being repeatedly uncovered and diversify the search in an adaptive manner; (2023年2月18日)

// A1 LINE 11:
end if /* more uncovered clients than last solution */
 
// A1 LINE 12: 
TL <- (i, j) /* update tabu list (Section 3.4) */
// TL: tabu list; 
// (i, j): pairs found; 
// Meaning: update tabu list; (2023年2月17日)

// A1 LINE 13: 
X_prev <- X, iter <- iter + 1;
// X_prev: solution of the previous iteration; 
// X: current solution; 

// A1 LINE 14:
end while; 
// Meaning: when the specified termination condition is met, the algorithm terminates and returns the best solution X*; (2023年2月17日)
```

### 3.1 Initialization
1. X: initial solution generated (a set of centers);
2. j: 中心序号; 
3. V: 顶点集; 
4. V(j): the **set** of vertex that center j can serve; 
5. Comment: 这里做了一个交叉, 中心序号却去对应顶点集; (2023年2月18日)
6. i: 顶点序号; 
7. C: 中心集; 
8. C(i): the **set** of centers which are able to serve client i; 
9. Comment: 这里做了一个交叉, 顶点序号却去对应中心集; (2023年2月18日)
10. U(X) = V - Union(j属于X, Vj); 
11. U(X): the set of vertex that are not served in solution X; 
12. V: 顶点集; 
13. V(j): the **set** of vertex that center j can serve; 
14. The constructive heuristic opens centers one by one under a maximal coverage principle; 
15. j = argmax(j属于C-X, |Union(Vj, U(x))|)
16. j: 中心序号, C-X代表解之外的中心序号; 
17. C: 中心集; 
18. X: current solution; 
19. V(j): the **set** of vertex that center j can serve; 
20. U(X): the set of vertex that are not served in solution X; 
21. If there are multiple candidate centers covering the same number of uncovered clients, ties are broken randomly; (2023年2月18日)

### 3.2 Vertex Weighting Technique
1. The vertex weighting technique helps the search to escape from the local optima by altering the objective function. (2023年2月17日)
2. Eq 11: 
3. (SCqw)  min f(X) = sum(i属于V, wi*ui) 
4. SCqw: 带有权重的集合覆盖; 
5. X: 挑选的中心集; 
6. f(X): 需要优化的目标函数
7. i: 顶点序号; 
8. V: 顶点集; 
9. w(i): 顶点i的权重; 
10. u(i): 如果顶点i没有被任何中心覆盖就是1, 否则就是0; u是uncovered的首字母; 
11. Meaning: 这个f(X)就是Algorithm 2中需要优化的目标函数; (2023年3月2日)
12. 
13. Eq 8': 
14. sum(j属于C && dij<=rq, xj) >= 1-ui, i属于V; 
15. i: 顶点序号; 
16. V: 顶点集; 
17. j: 中心序号; 
18. C: 中心集; 
19. d(i, j): 顶点i和中心j的距离; 
20. rq: 当前最大服务半径; 
21. x(j): 中心j在最终解就是1, 不在就是0; 
22. u(i): 如果顶点i没有被任何中心覆盖就是1, 否则就是0; 
23. Meaning: 如果顶点i被覆盖, 则在以顶点i为圆心的最大服务半径内, 至少有一个中心; 
24. 需要遍历不同的边长列表Γ来检查每个可能的半径; (2023年2月9日)
25. 
26. Eq 9': 
27. sum(j属于C, xj) = p; 
28. j: 中心序号; 
29. C: 中心集; 
30. x(j): 中心j在最终解就是1, 不在就是0; 
31. p: 选择的center数量; 
32. 
33. Eq 10': 
34. xj属于{0, 1}, ui属于{0, 1}, i属于V, j属于C;
35. j: 中心序号; 
36. x(j): 中心j在最终解就是1, 不在就是0; 
37. i: 顶点序号; 
38. u(i): 如果顶点i没有被任何中心覆盖就是1, 否则就是0; 
39. Meaning: 解释变量含义; (2023年2月18日) 
40. 
41. If the VWTS algorithm keeps failing to cover a client, it implies that this vertex is hard to cover and we should treat it with higher priority.

### 3.3 Neighborhood Structure and Evaluation
1. X直和swap(i, j) = X U {i} - {j}; 
2. i: center opened; i属于C-X; 
3. j: center closed; j属于X; 
4. Meaning: i in, j out; (2023年2月19日)
5. use a neighborhood sampling strategy and an incremental evaluation technique to accelerate the find_move; 
6. i covers some uncovered vertices in U(X); 
7. Improve find_move Strategy 1: randomly picks a vertex k属于U(X), and only evaluates neighborhood moves Swap(i, j) where i属于Ck and j属于X; 
8. Improve find_move Strategy 2: VWTS incrementally evaluates all neighborhood moves by storing and maintaining delta_j , the objective value increase (decrease) by closing (opening) center j; 
9. sum of the weights of the vertices which can only be served by center j; 
10. 由于j不属于X, 所以X-{j} = X; 
11. Meaning of delta_j: 
12. (1) 如果j不属于X, 那么把j并入X后, covered的增量, uncovered的减量; (越大越好); 
13. (2) 如果j属于X, 那么把j删除后, covered的减量, uncovered的增量; (越小越好); (2023年2月19日)
14. objective function f(X): 目前尚未覆盖到的顶点的数量, 目标是优化到0; (2023年2月19日)
```c++
// Algorithm 2: Find the best swap pair
// A2 LINE 1: 
function find_pair(X, TL, iter)
// X: current solution; 
// TL: tabu list; 
// iter: current iteration; 
 
// A2 LINE 2: 
The set of best swap moves M <- NULL; 
// M: the set of best swap moves;
 
// A2 LINE 3: 
The best objective value obj <- +INF;
// Meaning: objective value should be optimized to zero, so start with infinity; (2023年2月19日)

// A2 LINE 4: 
v <- a randomly picked uncovered vertex in U(X); 
 
// A2 LINE 5: 
for j属于C do
    delta_j_prev <- delta_j, 
// delta_j_prev: previous move; 
// delta_j: current move; 
// j: 中心序号; 
// C: 中心集; 
// Meaning: backup before trial moves; 
 
// A2 LINE 6: 
for all i属于Cv do /* Cv: candidates covering v */
// i: Cv中的中心序号; 
// Comment: 在前面的算法和表达式中, i用来表示顶点序号, 但是这里表示swap_in的中心序号; 
// Cv: 覆盖顶点v的中心集合; 
 
// A2 LINE 7: 
TryToOpenCenter(i) /* (Algorithm 3) */
// Meaning 1: tries to open each candidate center which covers vertex k
// Meaning 2: The sub-routine TryToOpenCenter(i) keeps each delta_j up-to-date to accelerate the calculation of the objective function f(X 直和 Swap(i, j)); 

// A2 LINE 8: 
for all j 属于 X do /* evaluate closing center j */
// j: 中心序号; 
// Comment: 打算swap_out的中心序号; 
// X: 当前解; 

// A2 LINE 9: 
if {i, j}交TL = NULL then 
// i: center swap in; 
// j: center swap out; 
// TL: tabu list; 
// Meaning: not tabu move; 

// A2 LINE 10: 
if f(X直和Swap(i, j)) < obj then
// X: current solution; 
// i: center swap in; 
// j: center swap out; 
// obj: num of vertices that has not been covered; 
// Meaning: this move is better than history best; 
 
// A2 LINE 11: 
obj <- f(X直和Swap(i, j))
// obj: num of vertices that has not been covered; 
// f(): objective function; 
// X: current center set; 
// i: center swap in; 
// j: center swap out; 
 
// A2 LINE 12: 
M <- {Swap(i, j)} 
// M: the set of best swap moves;
// i: center swap in; 
// j: center swap out;
// Meaning: change best move to (i, j); 
 
// A2 LINE 13: 
else if f(X直和Swap(i, j)) == obj then
// f(): objective function; 
// X: current center set; 
// i: center swap in; 
// j: center swap out; 
 
// A2 LINE 14: 
M <- M U (i, j)
// M: the set of best swap moves;
// i: center swap in; 
// j: center swap out;
// Meaning: equal move list; 
 
// A2 LINE 15: 
end if
 
// A2 LINE 16: 
end if
 
// A2 LINE 17: 
end for 
 
// A2 LINE 18: 
for j属于C do
    delta_j <- delta_j_previous, 
// Meaning: restore after trial moves; 

// A2 LINE 19: 
end for
 
// A2 LINE 20: 
return a randomly picked move in M
// M: the set of best swap moves; 
 
// A2 LINE 21: 
end function
```
124. |X 交 Cv| >= 1
125. Meaning: covering a client v which has already been covered; (2023年2月19日)
126. closing the center which serves a client v will not worsen the objective value if there are multiple centers which can cover vertex v; |X 交 Cv| >= 2

```c++
// **Algorithm 3** Open a center virtually
// A3 LINE 1: 
function TRY_TO_OPEN_CENTER(i)
// i: center swap in; 
 
// A3 LINE 2: 
for all v属于Vi do: 
// v: 顶点名称; 
// i: 中心序号; i在前面表示顶点序号, 这里却表示中心序号; 
// V(i): the **set** of vertex that center i can serve;

// A3 LINE 3: 
if |X 交 Cv| = 1 then
// X: current center set; 
// Cv: 覆盖顶点v的中心集合; 
// |X 交 Cv|: number of centers covering v in X; 

// A3 LINE 4 && LINE 5: 
for l 属于 X交Cv: 
    delta_l <- delta_l - wv, 
// l: X交Cv里面的中心; 现有解中覆盖V的中心; 
// X: current center set; 
// Cv: 覆盖顶点v的中心集合; 
// delta_l: 既然l属于X, 那么把l删除后, covered的减量, uncovered的增量; 
// Meaning 1: cancel penalty for making v uncovered; 
// Meaning 2: l was the only center covering v but it will not be the only one if i opens so closing l does not make v uncovered; 
// Meaning 3: closing center l will not make client v uncovered once center i is opened, because i also covers client v. 
// Comment: 由于|X 交 Cv| = 1, 所以这里面的循环只有一个数, 复杂度O(1). 

// A3 LINE 6: 
end if

// A3 LINE 7: 
end for
  
// A3 LINE 8: 
end function; 
```

```c++
// Algorithm 4 Make a swap move
// Comment: 注意到make_move中只计算每个中心权重的变化, 而不计算作为整体的f(X)
// A4 LINE 1: 
function make_move(i, j)
// i: center swapped in; 
// j: center swapped out; 

// A4 LINE 2: 
for all v属于Vi do 
// V(i): the **set** of vertex that center i can serve;
// Meaning: consequences of opening i

// A4 LINE 3: 
if |X 交 Cv| = 1 then /* (Algorithm 3) */
// X: current center set; 
// Cv: center set covering vertex v; 
// Meaning: 如果即将加入X的中心i所覆盖的顶点v刚好也被另外一个X中的中心覆盖; 

// A4 LINE 4: 
for l 属于 X交Cv: 
    delta_l <- delta_l - wv, 
// l: X交Cv里面的中心; 现有解中覆盖V的中心; 
// delta_l: 既然l属于X, 那么把l删除后, covered的减量, uncovered的增量; (在里面越小越好); 
// Meaning: cancel penalty for deleting center l; 
// Comment: 由于|X 交 Cv| = 1, 所以这里面的循环只有一个数, 复杂度O(1). 
// Comment: 虽然中心l是当前中心集X中独一无二覆盖顶点v的, 但是由于swapped in的中心i也覆盖v, 所以它不再是不可或缺的了, 价值要减小. 这里减去的其实是LINE 14增加的量. 

// A4 LINE 5: 
else if |X 交 Cv| = 0 then
// X: current center set; 
// Cv: center set covering vertex v; 
// Meaning: 如果即将加入X的中心i所覆盖的顶点v无法被X包含的中心们覆盖; 

// A4 LINE 6:
for l 属于 Cv-{i}: 
    delta_l <- delta_l - wv
// l: Cv中除i以外的中心; 由于|X 交 Cv| = 0, 所以l不在X中; 
// Cv: 覆盖顶点v的中心集合; 
// delta_l: 既然l不属于X, 那么把l并入X后, covered的增量, uncovered的减量; (在外面越大越好); 
// Meaning: cancel reward for adding center l; 
// Comment: 虽然不在X的中心l能够覆盖顶点v而X中的其他中心都不行, 但是由于swapped in的中心i也覆盖v, 所以它不再是必须加入的了, 价值要减小.

// A4 LINE 7: 
end if

// A4 LINE 8:
end for

// A4 LINE 9: 
X <- X U {i} - {j}
// X: current center set; 
// i: center swapped in;
// j: center swapped out; 
// 加入的时候是先计算中心权重变化再加入, 删除的时候则是先删除再计算中心权重变化; (2023年3月2日) 

// A4 LINE 10:
for all v 属于 Vj do 
// vj: vertex covered by center j; 
// Meaning: consequences of closing j

// A4 LINE 11: 
if |X 交 Cv| = 0 then
// X: current center set; 
// Cv: center set covering vertex v; 
// Meaning: 如果X中没有能够覆盖顶点v的中心; 

// A4 LINE 12: 
for l 属于 Cv - {j} do
    delta_l <- delta_l + wv;  
// l: Cv中除j以外的中心; 由于|X 交 Cv| = 0, 所以l不在X中; 
// Cv: 覆盖顶点v的中心集合; 
// delta_l: 既然l不属于X, 那么把l并入X后, covered的增量, uncovered的减量; (在外面越大越好); 
// Meaning: add reward for adding center l; 因为X中现在谁也不能覆盖顶点v, 所以能够覆盖顶点v的中心l价值要增加; 
// Comment: 此时的X已经把j删除了, 见LINE 9; 

// A4 LINE 13: 
else if |X 交 Cv| = 1 then
// X: current center set; 
// Cv: center set covering vertex v; 
// Meaning: 如果已经被踢出X的中心j所覆盖的顶点v刚好也被另外一个X中的中心覆盖; 

// A4 LINE 14: 
for l 属于 X 交 Cv do
    delta_l <- delta_l + wv; 
// l: X交Cv里面的中心; 现有解中覆盖v的中心; 
// delta_l: 既然l属于X, 那么把l删除后, covered的减量, uncovered的增量; (在里面越小越好); 
// Meaning: add penalty for deleting center l; 
// Comment: 由于|X 交 Cv| = 1, 所以这里面的循环只有一个数, 复杂度O(1). 
// Comment: 由于中心l是当前中心集X中独一无二覆盖顶点v的, 所以它的价值要增加.  

// A4 LINE 15: 
end if

// A4 LINE 16:
end for

// A4 LINE 17:
end function
```

### 3.4 Tabu Search
1. The tabu strategy prevents closing newly opened centers or reopening newly closed ones immediately. (Comment: pair (center_in, center_out) in the tabu list) 
2. fix the parameter of the tabu tenure in the tabu strategy to one iteration
3. Swap(i, j) at iteration iter introduces two vertices (i, j) in tabu list TL (Algorithm 1, line 12) and neither i nor j can be involved at iteration iter + 1 (Algorithm 2, line 9). // Comment: 不是作为一个整体禁止一堆, 而是分别禁止两个个体; (2023年2月21日)

## 4 Experimental Results and Comparisons














