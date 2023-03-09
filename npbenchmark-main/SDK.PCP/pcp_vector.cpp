//
// Created by take_ on 2022/12/13.
//
# include "pcp_vector.h"

using namespace boost;
using namespace std;

PCP_Vector::PCP_Vector(int input_num_vertex, int input_num_center, int input_radius,
                       vector<vector<int>> &input_coverages, int input_seed):
        num_vertex(input_num_vertex),
        num_center(input_num_center),
        radius(input_radius),
        center_coverages(input_num_vertex),
        center_cover_vertex(input_num_vertex, dynamic_bitset<>(input_num_vertex)),
        // vertex_reaching(input_num_vertex),
        num_reach_solution(input_num_vertex, 0),
        reach_one_solution(input_num_vertex, -1),
        solution(input_num_vertex, 0),
        // A1 LINE 3
        vertex_weights(input_num_vertex, 1),
        uncovered_vertices(input_num_vertex),
        num_uncovered(INT_MAX),
        best_num_uncovered(INT_MAX),
        prev_num_uncovered(INT_MAX),
        moved{0, 0},
        min_delta{INT_MAX},
        // A1 LINE 2:
        // tabu list TL <- NULL;
        // TL: tabu list;
        tabu_open{-1},
        tabu_close{-1},
        center_weights(input_num_vertex, 0),
        prev_center_weights(input_num_vertex, 0),
        equal_pair(2000, {0, 0}),
        equal_pair_count{0},
        sum_uncovered_weight{0},
        // A1 LINE 2:
        // iter <- 1;
        // iter: current iteration;
        iter{0}
{
    init_rand(input_seed); // initialize random generator;

    for(int i=0;i<input_coverages.size();i++) // i is center name
    {
        for(int j=0;j<input_coverages[i].size();j++) // j is vertex name;
        {
            int index = input_coverages[i][j];
            center_cover_vertex[i].set(index, true);
            center_coverages[i].push_back(index);
            // vertex_reaching[index].push_back(i);
        }
        // cerr << "center_cover_vertex[" << i << "] " << center_cover_vertex[i] << endl;
    }
    // cerr << endl;

    // Evaluation
    // cerr << "center_cover_vertex[0]: " << center_cover_vertex[0] << endl;
    // cerr << "center_cover_vertex[99]: " << center_cover_vertex[99] << endl;
    // cerr << "vertex_reach_center[0]: " << vertex_reach_center[0] << endl;

    // Evaluate A1 LINE 3
    // print_vector("weight", weight);
    uncovered_vertices.set(); // set uncovered all 1;

    // debug variables;
    start_time = clock();
}

PCP_Vector::~PCP_Vector()
= default;

void PCP_Vector::greedy_construct()
{
    // center_weights start with each center's coverage;
    for(int i=0;i<center_weights.size();i++)
    {
        center_weights[i] = int (center_coverages[i].size());
    }
    // print_vector("center_weights before", center_weights);
    int equal_delta_in_construct[2000] = {0}; //非禁忌相同delta值
    int equal_count_in_construct = 0;
    for(int i=0;i<num_center;i++) // do one iteration;
    {
        // cerr << "Construct iteration: " << iter << endl;

        unsigned long long max_overlap_size = 0;

        // cerr << "uncovered" << uncovered << endl;
        for(int j=0;j<num_vertex;j++) // consider only one set
        {
            dynamic_bitset<> this_intersection = center_cover_vertex[j] & uncovered_vertices;
            unsigned long long this_intersection_size = this_intersection.count();

            if(this_intersection_size > max_overlap_size)
            {
                max_overlap_size = this_intersection_size;
                equal_count_in_construct = 0;
                equal_delta_in_construct[equal_count_in_construct] = j; // j is index of center;
                equal_count_in_construct++;
            }
            else if(this_intersection_size == max_overlap_size)
            {
                equal_delta_in_construct[equal_count_in_construct] = j; // j is index of center;
                equal_count_in_construct++;
            }
        }

        // cerr << "equal count in construct: " << equal_count_in_construct << endl;
        unsigned int rand_select = generated_random() % equal_count_in_construct; // 相等tabu_delta随机选择
        int selected_center = equal_delta_in_construct[rand_select];
        solution_value.push_back(selected_center);
        // cerr << "random select: " << rand_select << endl;
        // cerr << "selected center: " << selected_center << endl;

        // consequences of opening selected center, this part refers to Algorithm 3;
        // Refer to A4 LINE 2:
        // for all v属于Vi do
        // V(i): the set of vertex that center i can serve; index is center, result is vertex;
        // Meaning: consequences of opening i
        for (int v : center_coverages[selected_center])
        {
            // cerr << v << " ";
            // Reference A4 LINE 3:
            // if |X 交 Cv| = 1 then
            // X: current center set;
            // v: vertex
            // Cv: 覆盖顶点v的中心集合;
            // |X 交 Cv|: number of centers covering v in X;
            if (num_reach_solution[v] == 1)
            {
                // print_index1("solution", solution);
                // print_index1("Cv", Cv);
                // int intersect_center = reach_one_solution[v];
                // cerr << "find intersect one: " << intersect_center << endl;

                // Refer to A4 LINE 4:
                // for l 属于 X交Cv:
                //     delta_l <- delta_l - wv,
                // l: X交Cv里面的中心; 现有解中覆盖V的中心;
                // delta_l: 既然l属于X, 那么把l删除后, covered的减量, uncovered的增量; (在里面越小越好);
                // Meaning: cancel penalty for deleting center l;
                // Comment: 由于|X 交 Cv| = 1, 所以这里面的循环只有一个数, 复杂度O(1).
                // Comment: 虽然中心l是当前中心集X中独一无二覆盖顶点v的, 但是由于swapped in的中心i也覆盖v, 所以它不再是不可或缺的了, 价值要减小. 这里减去的其实是LINE 14增加的量.
                // print_vector("center weights before", center_weights);
                center_weights[reach_one_solution[v]] -= vertex_weights[v];
                // print_vector("center weights after", center_weights);
            }
                // Refer to A4 LINE 5:
                // else if |X 交 Cv| = 0 then
                // X: current center set;
                // Cv: center set covering vertex v;
                // Meaning: 如果即将加入X的中心i所覆盖的顶点v无法被X包含的中心们覆盖;
            else if(num_reach_solution[v] == 0)
            {
                uncovered_vertices.reset(v);
                reach_one_solution[v] = selected_center;

                // Refer to A4 LINE 6:
                // for l 属于 Cv-{i}:
                //     delta_l <- delta_l - wv
                // l: Cv中除i以外的中心; 由于|X 交 Cv| = 0, 所以l不在X中;
                // Cv: 覆盖顶点v的中心集合;
                // delta_l: 既然l不属于X, 那么把l并入X后, covered的增量, uncovered的减量; (在外面越大越好);
                // Meaning: cancel reward for adding center l;
                // Comment: 虽然不在X的中心l能够覆盖顶点v而X中的其他中心都不行, 但是由于swapped in的中心i也覆盖v, 所以它不再是必须加入的了, 价值要减小.
                // print_index1("solution", solution);
                // print_index1("Cv", Cv);
                // for (int l : vertex_reaching[v])
                for (int l : center_coverages[v])
                {
                    // cerr << "l: " << l << endl;
                    center_weights[l] -= vertex_weights[v];
                }
                center_weights[selected_center] += vertex_weights[v]; // {j} does not need to minus;
                // cerr << endl;

                // Refer to A4 LINE 7:
                // end if
            }

            // v is now covered by the selected center;
            num_reach_solution[v]++;

            // Refer to A4 LINE 8:
            // end for
        }
        // cerr << endl;

        // Evaluate A4 LINE 6:
        // dynamic_bitset<> test_0_59 = center_cover_vertex[0] & center_cover_vertex[59];
        // cerr << "0 intersect 59: " << test_0_59.count() << endl;

        // Evaluate A4 LINE 4:
        // dynamic_bitset<> test_0_59_12 = center_cover_vertex[0] & (center_cover_vertex[59] | center_cover_vertex[12]);
        // cerr << "0 intersect (59 U 12): " << test_0_59_12.count() << endl;

        // Refer to A4 LINE 9:
        // X <- X U {i}
        // X: current center set;
        // i: center swapped in;
        // Meaning: Open selected center;
        solution.set(selected_center);
        // cerr << "Cover after union size (" << covered.count() << "): " << endl;
        // print_index1("Covered", covered);
        // print_index1("Uncovered", uncovered);
    }
    // print_vector("center weights after", center_weights);

    // print_index1("Center selected", solution);
    // cerr << "sum_uncovered_weight: " << sum_uncovered_weight << endl;

    uncovered_value.clear();
    for (size_t i = uncovered_vertices.find_first(); i != dynamic_bitset<>::npos; i = uncovered_vertices.find_next(i))
    {
        uncovered_value.push_back(int (i));
    }
    num_uncovered = int (uncovered_value.size());
    sum_uncovered_weight = num_uncovered;
}

// Algorithm 2: Find the best swap pair
// A2 LINE 1:
// function find__pair(X, TL, iter)
// X: current solution;
// TL: tabu list;
// iter: current iteration;
void PCP_Vector::find_pair()
{
    // A2 LINE 2:
    // The set of best swap moves M <- NULL;
    // M: the set of best swap moves;
    equal_pair_count = 0;
    // print_equal_pair();

    // A2 LINE 3:
    // The best objective value obj <- +INF;
    // Meaning: objective value should be optimized to zero, so start with infinity; (2023年2月19日)
    min_delta = INT_MAX;

    // A2 LINE 4:
    // v <- a randomly picked uncovered vertex in U(X);
    // print_index1("uncovered", uncovered);
    unsigned int random_uncovered_vertex = uncovered_value[generated_random() % uncovered_value.size()];
    // cerr << "random uncovered vertex: " << random_uncovered_vertex << endl;

    // A2 LINE 5:
    // for j属于C do
    //    delta_j_prev <- delta_j,
    // delta_j_prev: previous move;
    // delta_j: current move;
    // j: 中心序号;
    // C: 中心集;
    // Meaning: backup before trial moves;
    prev_center_weights = center_weights;

    // A2 LINE 6:
    // for all i属于Cv do /* Cv: candidates covering v */
    // i: Cv中的中心序号;
    // Comment: 在前面的算法和表达式中, i用来表示顶点序号, 但是这里表示swap_in的中心序号;
    // v: 随机未覆盖顶点;
    // Cv: 覆盖顶点v的中心集合;

    // Evaluate A1 LINE 12
    // cerr << "tabu tenure table: ";
    // for(int temp=0;temp<tabu_tenure_table.size();temp++)
    //    if(tabu_tenure_table[temp] > iter)
    //        cerr << temp << " ";
    // cerr << endl;
    // for (int ic : vertex_reaching[random_uncovered_vertex])
    for (int ic : center_coverages[random_uncovered_vertex])
    {
        if(ic == tabu_open || ic == tabu_close)
            continue;

        // cerr << "center: " << i << endl; // i is center name;
        // A2 LINE 7:
        // TryToOpenCenter(i) /* (Algorithm 3) */
        // Meaning 1: tries to open each candidate center which covers vertex k
        // Meaning 2: The sub-routine TryToOpenCenter(i) keeps each delta_j up-to-date to accelerate the calculation of the objective function f(X 直和 Swap(i, j));


        // A3 LINE 2:
        // for all v属于Vi do:
        // v: 顶点名称;
        // i: 中心序号; i在前面表示顶点序号, 这里却表示中心序号;
        // V(i): the set of vertex that center i can serve;
        // print_index1("Vi", center_cover_vertex[center]);
        for (int iv : center_coverages[ic])
        {
            // cerr << v << " ";
            // A3 LINE 3:
            // if |X 交 Cv| = 1 then
            // X: current center set;
            // v: vertex
            // Cv: 覆盖顶点v的中心集合;
            // |X 交 Cv|: number of centers covering v in X;
            if (num_reach_solution[iv] == 1)
            {
                // A3 L4 & L5
                // cerr << "find one: ";
                // print_index1("solution", solution);
                // print_index1("Cv", Cv);
                // int intersect_center = reach_one_solution[iv];
                // cerr << "find intersect one: " << intersect_center << endl;
                // for l 属于 X交Cv:
                //     delta_l <- delta_l - wv,
                // l: X交Cv里面的中心; 现有解中覆盖V的中心;
                // delta_l: 中心l的权重;
                // Meaning: cancel penalty for deleting center l;
                // Comment: 由于|X 交 Cv| = 1, 所以这里面的循环只有一个数, 复杂度O(1).
                // Comment: 虽然中心l是当前中心集X中独一无二覆盖顶点v的, 但是由于swapped in的中心i也覆盖v, 所以它不再是不可或缺的了, 价值要减小. 这里减去的其实是LINE 14增加的量.
                // print_vector("center weights before", center_weights);
                center_weights[reach_one_solution[iv]] -= vertex_weights[iv];
                // print_vector("center weights after", center_weights);

                // A3 LINE 6:
                // end if
            }
            // A3 LINE 7:
            // end for
        }
        // cerr << endl;
        // A3 LINE 8:
        // end function;

        // A2 LINE 8:
        // for all j 属于 X do /* evaluate closing center j */
        // j: 中心序号;
        // Comment: 打算swap_out的中心序号;
        // X: 当前解;
        // print_index1("X", solution);
        // cerr << "X: ";
        for (int j : solution_value)
        {
            // cerr << j << " ";
            // A2 LINE 9:
            // if {i, j}交TL = NULL then
            // i: center swap in;
            // j: center swap out;
            // TL: tabu list;
            // Meaning: not tabu move;
            if(j != tabu_open && j != tabu_close)
            {
                // A2 LINE 10:
                // if f(X直和Swap(i, j)) < obj then
                // X: current solution;
                // i: center swap in;
                // j: center swap out;
                // obj: num of vertices that has not been covered;
                // Meaning: this move is better than history best;
                // cerr << endl << "i " << i << " j " << j;
                int this_iter_delta = center_weights[j] - center_weights[ic];
                // cerr << " f(X+{i}-{j}) " << f_X_i_j << endl;
                if(this_iter_delta < min_delta)
                {
                    // A2 LINE 11:
                    // obj <- f(X直和Swap(i, j))
                    // obj: num of vertices that has not been covered;
                    // f(): objective function;
                    // X: current center set;
                    // i: center swap in;
                    // j: center swap out;
                    min_delta = this_iter_delta;
                    // cerr << "obj: " << obj << endl;

                    // LINE 12:
                    // M <- {Swap(i, j)}
                    // M: the set of best swap moves;
                    // i: center swap in;
                    // j: center swap out;
                    // Meaning: change best move to (i, j);
                    equal_pair_count = 0;
                    equal_pair[equal_pair_count].center_in = ic;
                    equal_pair[equal_pair_count].center_out = j;
                    equal_pair_count++;
                }
                    // LINE 13:
                    // else if f(X直和Swap(i, j)) == obj then
                    // f(): objective function; weight sum of uncovered vertices;
                    // X: current center set;
                    // i: center swap in;
                    // j: center swap out;
                else if(this_iter_delta == min_delta)
                {
                    // LINE 14:
                    // M <- M U (i, j)
                    // M: the set of best swap moves;
                    // i: center swap in;
                    // j: center swap out;
                    // Meaning: equal move list;
                    equal_pair[equal_pair_count] = Move{ic, j};
                    equal_pair_count++;

                    /*
                    if(equal_pair_count > max_equal_pair_count)
                    {
                        max_equal_pair_count = equal_pair_count;
                        cerr << "Max equal pair count: " << max_equal_pair_count << endl;
                    }
                     */

                    // A5 LINE 15:
                    // end if
                }

                // A2 LINE 16:
                // end if
            }

            // A2 LINE 17:
            // end for
        }
        // cerr << endl;

        // A2 LINE 18:
        // for j属于C do
        //    delta_j <- delta_j_previous,
        // Meaning: restore after trial moves;
        center_weights = prev_center_weights;

        // A2 LINE 19:
        // end for
    }

    // Evaluate A2 LINE 12 & LINE 14
    // print_equal_pair();


    // LINE 21:
    // end function
}


// Algorithm 4 Make a swap move
// Comment: 注意到make__move中只计算每个中心权重的变化, 而不计算作为整体的f(X)
// LINE 1:
// function make__move(i, j)
// i: center swapped in;
// j: center swapped out;
void PCP_Vector::make_move()
{
    sum_uncovered_weight += min_delta;

    // A4 LINE 2:
    // for all v属于Vi do
    // V(i): the set of vertex that center i can serve;
    // Meaning: consequences of opening i
    for (int v : center_coverages[moved.center_in])
    {
        // cerr << v << " ";
        // A4 LINE 3:
        // if |X 交 Cv| = 1 then /* (Algorithm 3) */
        // X: current center set;
        // Cv: center set covering vertex v;
        // Meaning: 如果即将加入X的中心i所覆盖的顶点v刚好也被另外一个X中的中心覆盖;
        if (num_reach_solution[v] == 1)
        {
            // Evaluate A4 LINE 3
            // print_index1("solution", solution);
            // print_index1("Cv", Cv);
            // int intersect_center = reach_one_solution[v];
            // cerr << "find intersect one: " << intersect_center << endl;
            // cerr << "reach_one_solution: " << reach_one_solution[v] << endl;

            // A4 LINE 4:
            // for l 属于 X交Cv:
            //     delta_l <- delta_l - wv,
            // l: X交Cv里面的中心; 现有解中覆盖V的中心;
            // delta_l: 既然l属于X, 那么把l删除后, covered的减量, uncovered的增量; (在里面越小越好);
            // Meaning: cancel penalty for deleting center l;
            // Comment: 由于|X 交 Cv| = 1, 所以这里面的循环只有一个数, 复杂度O(1).
            // Comment: 虽然中心l是当前中心集X中独一无二覆盖顶点v的, 但是由于swapped in的中心i也覆盖v, 所以它不再是不可或缺的了, 价值要减小. 这里减去的其实是LINE 14增加的量.
            // print_vector("center weights before", center_weights);
            center_weights[reach_one_solution[v]] -= vertex_weights[v];
            // print_vector("center weights after", center_weights);

        }
            // A4 LINE 5:
            // else if |X 交 Cv| = 0 then
            // X: current center set;
            // Cv: center set covering vertex v;
            // Meaning: 如果即将加入X的中心i所覆盖的顶点v无法被X包含的中心们覆盖;
        else if(num_reach_solution[v] == 0)
        {
            // print_index1("solution after opening i", solution);
            // print_index1("Cv after opening i", Cv);

            // A4 LINE 6:
            // for l 属于 Cv-{i}:
            //     delta_l <- delta_l - wv
            // l: Cv中除i以外的中心; 由于|X 交 Cv| = 0, 所以l不在X中;
            // Cv: 覆盖顶点v的中心集合;
            // delta_l: 既然l不属于X, 那么把l并入X后, covered的增量, uncovered的减量; (在外面越大越好);
            // Meaning: cancel reward for adding center l;
            // Comment: 虽然不在X的中心l能够覆盖顶点v而X中的其他中心都不行, 但是由于swapped in的中心i也覆盖v, 所以它不再是必须加入的了, 价值要减小.
            // for (int l : vertex_reaching[v])
            for (int l : center_coverages[v])
            {
                // cerr << l << endl;
                // print_vector("center weights before", center_weights);
                center_weights[l] -= vertex_weights[v];
                // print_vector("center weights after", center_weights);
            }
            // cerr << endl;
            center_weights[moved.center_in] += vertex_weights[v];

            reach_one_solution[v] = moved.center_in;
            uncovered_vertices.reset(v);
            // A4 LINE 7:
            // end if
        }

        num_reach_solution[v]++;

        // A4 LINE 8:
        // end for
    }
    // cerr << endl;

    // A4 LINE 9:
    // X <- X U {i} - {j}
    // X: current center set;
    // i: center swapped in;
    // j: center swapped out;
    // 加入的时候是先计算中心权重变化再加入, 删除的时候则是先删除再计算中心权重变化; (2023年3月2日)
    // print_index1("solution before A4 LINE 9", solution);
    solution.set(moved.center_in);
    solution.reset(moved.center_out);

    for (int i=0; i<num_center; i++)
    {
        if (solution_value[i] == moved.center_out)
        {
            solution_value[i] = moved.center_in;
            break;
        }
    }
    // print_index1("solution after swap", solution);

    // A4 LINE 10:
    // for all v 属于 Vj do
    // vj: vertex covered by center j;
    // Meaning: consequences of closing j
    for (int v : center_coverages[moved.center_out])
    {
        num_reach_solution[v]--;

        // cerr << v << " ";
        // A4 LINE 11:
        // if |X 交 Cv| = 0 then
        // X: current center set;
        // Cv: center set covering vertex v;
        // Meaning: 如果X中没有能够覆盖顶点v的中心;
        if(num_reach_solution[v] == 0)
        {
            // print_index1("solution after close j", solution);
            // print_index1("Cv after close j", Cv);

            uncovered_vertices.set(v);

            // LINE 12:
            // for l 属于 Cv - {j} do
            //    delta_l <- delta_l + wv;
            // l: Cv中除j以外的中心; 由于|X 交 Cv| = 0, 所以l不在X中;
            // Cv: 覆盖顶点v的中心集合;
            // delta_l: 既然l不属于X, 那么把l并入X后, covered的增量, uncovered的减量; (在外面越大越好);
            // Meaning: add reward for adding center l; 因为X中现在谁也不能覆盖顶点v, 所以能够覆盖顶点v的中心l价值要增加;
            // Comment: 此时的X已经把j删除了, 见LINE 9;
            // for (int l : vertex_reaching[v])
            for (int l : center_coverages[v])
            {
                // cerr << l << endl;
                // print_vector("center weights before", center_weights);
                // cerr << center_weights[l] << endl;
                center_weights[l] += vertex_weights[v];
                // print_vector("center weights after ", center_weights);
                // cerr << center_weights[l] << endl;
            }
            // cerr << endl;
            center_weights[moved.center_out] -= vertex_weights[v];
        }
            // A4 LINE 13:
            // else if |X 交 Cv| = 1 then
            // X: current center set;
            // Cv: center set covering vertex v;
            // Meaning: 如果已经被踢出X的中心j所覆盖的顶点v刚好也被另外一个X中的中心覆盖;
        else if (num_reach_solution[v] == 1)
        {
            // Evaluate A4 LINE 13
            // print_index1("solution", solution);
            // print_index1("Cv", Cv);
            int intersect_center = -1;

            // for(int l : vertex_reaching[v])
            for(int l : center_coverages[v])
            {
                if(solution[l])
                {
                    intersect_center = l;
                    break;
                }
            }

            reach_one_solution[v] = intersect_center;
            // cerr << "find intersect one: " << intersect_center << endl;

            // A4 LINE 14:
            // for l 属于 X 交 Cv do
            //    delta_l <- delta_l + wv;
            // l: X交Cv里面的中心; 现有解中覆盖v的中心;
            // delta_l: 既然l属于X, 那么把l删除后, covered的减量, uncovered的增量; (在里面越小越好);
            // Meaning: add penalty for deleting center l;
            // Comment: 由于|X 交 Cv| = 1, 所以这里面的循环只有一个数, 复杂度O(1).
            // Comment: 由于中心l是当前中心集X中独一无二覆盖顶点v的, 所以它的价值要增加.
            // print_vector("center weights before", center_weights);
            // cerr << center_weights[intersect_center] << endl;
            center_weights[intersect_center] += vertex_weights[v];
            // print_vector("center weights after", center_weights);
            // cerr << center_weights[intersect_center] << endl;

            // A4 LINE 15:
            // end if
        }

        // A4 LINE 16:
        // end for
    }

    // A4 LINE 17:
    // end function
    // print_index1("covered after swap", covered);
    // print_index1("uncovered after swap", uncovered);
}

// Algorithm 1 The main framework of the VWTS algorithm
// A1 LINE 1: /* (Section 3.1) */
// X <- init(G, p, rq);
// X: initial solution generated;
// G: graph;
// p: centers;
// rq: 最小化的最大服务半径;
// Meaning: generates an initial solution X by a greedy algorithm; (2023年2月10日)
void PCP_Vector::vertex_weight_tabu_search()
{
    // print_index1("center cover 0:", center_cover_vertex[0]);
    // print_index1("0 reach center:", vertex_reach_center[0]);
    // print_vector("center 0 cover: ", center_coverages[0]);
    // print_vector("vertex 0 reach: ", vertex_reaching[0]);

    // A1 LINE 1:
    // X <- init(G, p, rq);
    // X: initial solution generated;
    // G: graph;
    // p: centers;
    // rq: 最小化的最大服务半径;
    // Meaning: generates an initial solution X by a greedy algorithm; (2023年2月10日)
    greedy_construct();
    // sequence_construct();
    // print_index1("random construct solution", solution);

    // Evaluate num_reach_solution after greedy__construct;
    // print_vector("center[2]", center_coverages[2]);
    // print_vector("center[12]", center_coverages[12]);
    // print_vector("center[59]", center_coverages[59]);
    // print_vector("num_reach_solution", num_covered_by);

    // Evaluate reach_one_solution after greedy__construct
    // print_vector("reach_one_solution", reach_one_solution);

    // A1 LINE 2:
    // X* <- X
    // X*: history best solution;
    // X: initial solution generated (a set of centers);
    best_num_uncovered = num_uncovered;
    // cerr <<"best_num_uncovered: " << best_num_uncovered << endl;

    // X_prev <- X;
    // X_prev: solution of the previous iteration;
    // X: initial solution generated;
    prev_num_uncovered = num_uncovered;
    // cerr <<"prev_num_uncovered: " << prev_num_uncovered << endl;

    // Evaluate A2 LINE 11:
    // 测试f(X+{i}-{j})的计算是否准确
    // dynamic_bitset<> union_result = center_cover_vertex[2] | center_cover_vertex[66] | center_cover_vertex[31]| center_cover_vertex[59]| center_cover_vertex[77];
    // cerr << "uncovered number: " << num_vertex - union_result.count() << endl;

    // Evaluate covered and uncovered set in make__move;
    // dynamic_bitset<> union_result = center_cover_vertex[2] | center_cover_vertex[12] | center_cover_vertex[59];
    // dynamic_bitset<> union_uncovered = ~union_result;
    // print_index1("uncovered to evaluate make__move", union_uncovered);

    // A1 LINE 4:
    // while termination condition is not met do
    // Meaning: iteratively improves the incumbent solution by a tabu search procedure; (2023年2月10日)
    while(num_uncovered != 0)
    {
        // cerr << "iteration: " << iter << endl;

        // cerr << "f(X) before find pair: " << compute_sum_uncovered_weight() << endl;
        // cerr << "sum_uncovered_weight before find pair: " << sum_uncovered_weight << endl;

        // A1 LINE 5:
        // (i, j) <- FindPair(X_prev, TL, iter) /* (Algorithm 2) */
        // (i, j): pair swapped
        // X_prev: solution of the previous iteration;
        // TL: tabu list;
        // iter: current iteration;
        // Meaning: find_move; evaluates the neighborhood of the current solution and records the best neighborhood move while respecting their tabu states; (2023年2月10日)
        find_pair();

        // LINE 20:
        // return a randomly picked move in M
        // M: the set of best swap moves;
        if (equal_pair_count == 0)
        {
            tabu_open = -1;
            tabu_close = -1;
            continue;
        }
        else if(equal_pair_count == 1)
        {
            moved = equal_pair[0];
        }
        else
        {
            moved = equal_pair[generated_random() % equal_pair_count];
        }

        // cerr << "moved {" << moved.center_in << " " << moved.center_out << "}" << endl;
        // cerr << "center_weights[in: 65]: " << center_weights[65] << endl;
        // cerr << "center_weights[out: 59]: " << center_weights[59] << endl;
        // print_vector("center_weights", center_weights);

        // cerr << "f(X) before make move: " << compute_sum_uncovered_weight() << endl;
        // cerr << "sum_uncovered_weight before make move: " << sum_uncovered_weight << endl;

        // A1 LINE 6:
        // MakeMove(i, j) /* (Algorithm 4) */
        // (i,j): pair moved found in the previous;
        // Meaning: makes the best move; (2023年2月10日)
        make_move();

        uncovered_value.clear();
        for (size_t i = uncovered_vertices.find_first(); i != dynamic_bitset<>::npos; i = uncovered_vertices.find_next(i))
        {
            uncovered_value.push_back(int (i));
        }
        num_uncovered = uncovered_value.size();

        // cerr << "f(X) after make move: " << compute_sum_uncovered_weight() << endl;
        // cerr << "sum_uncovered_weight after make move: " << sum_uncovered_weight << endl;

        // A1 LINE 7:
        // if |U(X)| < |U(X*)| then
        // X: current solution;
        // |U(X)|: the set of clients uncovered by X;
        // X*: history best solution;
        // |U(X*)|: the set of clients uncovered by X*;
        // Meaning: If the current solution X improves the best solution found so far
        if(num_uncovered < best_num_uncovered)
        {
            // A1 LINE 8:
            // X* <- X
            // X*: history best solution;
            // X: current solution;
            // Meaning: then X* is updated with X;
            // cerr << "best_num_uncovered: " << best_num_uncovered << endl;
            best_num_uncovered = num_uncovered;
            // cerr << "best_num_uncovered: " << best_num_uncovered << endl;
        }
            // A1 LINE 9:
            // else if |U(X)| >= |U(X_prev)| then
            // X: current solution;
            // |U(X)|: the set of clients uncovered by X;
            // X_prev: solution of the previous iteration;
            // |U(X')|: the set of clients uncovered by X';
            // Meaning: the best move returned by function FindPair() cannot reduce the number of uncovered clients; (2023年2月17日)
        else if(num_uncovered >= prev_num_uncovered)
        {
            // cerr << "num_uncovered A1 LINE 9: " << num_uncovered << endl;
            // cerr << "prev_num_uncovered A1 LINE 9: " << prev_num_uncovered << endl;

            // A1 LINE 10 /* (Section 3.2) */
            // for v belongs to U(X) do
            //    wv <- wv + 1
            // wv: weight of vertex that uncovered by X;
            // Meaning 1: If stagnation (停滞) occurs, the weight of each uncovered client is adjusted; (2023年2月17日)
            // Meaning 2: when the tabu search is **trapped in local optimal** solution X, the VWTS algorithm increases the weight wi of each uncovered client i属于U(X) by one unit; (2023年2月18日)
            // Meaning 3: prevent vertices from being repeatedly uncovered and diversify the search in an adaptive manner; (2023年2月18日)
            // print_index1("uncovered", uncovered);
            // print_vector("vertex weights", vertex_weights);
            // print_index1("uncovered", uncovered);
            // print_vector("center weights", center_weights);
            
            for (size_t iv : uncovered_value)
            {
                vertex_weights[iv]++;
                // for (int ic : vertex_reaching[iv])
                for (int ic : center_coverages[iv])
                {
                    center_weights[ic]++;
                }
            }
            sum_uncovered_weight += num_uncovered;

            // print_vector("vertex weights", vertex_weights);
            // print_vector("center weights", center_weights);

            // A1 LINE 11:
            // end if /* more uncovered clients than last solution */
        }


        int flag = false;
        for(int i : vertex_weights)
        {
            if(i >= 65534)
            {
                flag = true;
                break;
            }
        }

        /*
        if(flag == true)
        {
            cerr << "Upper Overflow!!!" << endl;
            for(int i=0;i<vertex_weights.size();i++)
            {
                int half = vertex_weights[i] / 2;
                vertex_weights[i] = vertex_weights[i] - half;

                if(num_reach_solution[i]==1)
                {
                    center_weights[reach_one_solution[i]] = center_weights[reach_one_solution[i]] - half;
                }
                else if(num_reach_solution[i]==0)
                {
                    for(int j : center_coverages[i])
                    {
                        center_weights[j] = center_weights[j] - half;
                    }
                }
            }

            sum_uncovered_weight = 0;
            for(int i : uncovered_value)
            {
                sum_uncovered_weight = sum_uncovered_weight + vertex_weights[i];
            }
        }*/

        // A1 LINE 12:
        // TL <- (i, j) /* update tabu list (Section 3.4) */
        // TL: tabu list;
        // (i, j): pairs found;
        // Meaning: update tabu list; (2023年2月17日)
        tabu_open = moved.center_in;
        tabu_close = moved.center_out;

        ///* debug: tabu tenure;
        // cerr << "tabu tenure out: " << tabu_tenure_table[moved.center_out] << endl;
        // cerr << "tabu tenure in: " << tabu_tenure_table[moved.center_in] << endl;
        // print_tabu_tenure_table();
        //*/

        // A1 LINE 13:
        // X_prev <- X, iter <- iter + 1;
        // X_prev: solution of the previous iteration;
        // X: current solution;
        // cerr << "prev_num_uncovered: " << prev_num_uncovered << endl;
        prev_num_uncovered = num_uncovered;
        // cerr << "prev_num_uncovered: " << prev_num_uncovered << endl;
        iter++;

        if (iter % 50000 == 0)
        {
            // Evaluate whether sum__uncovered__weight and num__uncovered__vertices
            // long long temp_SUW = 0;
            // for(int i : uncovered_value)
            // {
            //    temp_SUW = temp_SUW + vertex_weights[i];
            // }
            //long long temp_num_uncovered = uncovered_value.size();

            cerr << "Radius: " << radius << " ";
            cerr << "iter: " << iter << " ";
            // cerr << "equal_pair_num: " << equal_pair_count << " ";
            // cerr << "best num : " << best_num_uncovered << " ";
            cerr << "SUW: " << sum_uncovered_weight << " ";
            // cerr << "temp SUW: " << temp_SUW << " ";
            // cerr << "min delta: " << min_delta << " ";
            cerr << "NUV : " << num_uncovered << " ";
            // cerr << "temp NUV : " << temp_num_uncovered << " ";
            // cerr << "moved " << moved.center_in << " " << moved.center_out;
            double elapsed_time = (clock() - start_time) / CLOCKS_PER_SEC;
            cerr << " time(s): " << elapsed_time
                 << " frequency:" << double(iter) / elapsed_time << endl;

        }

        // A1 LINE 14:
        // end while;
        // Meaning: when the specified termination condition is met, the algorithm terminates and returns the best solution X*; (2023年2月17日)
    }

    // Evaluate covered by after 30 make__move;
    // print_vector("center[90]", center_coverages[90]);
    // print_vector("center[3]", center_coverages[3]);
    // print_vector("center[37]", center_coverages[37]);
    // print_vector("num_covered_by", num_covered_by);
    // print_vector("reach one solution", reach_one_solution);
    // print_vector("num reach solution", num_reach_solution);

    // print final tabu_tenure_table;
    // print_tabu_tenure_table();

    double elapsed_time = (clock() - start_time) / CLOCKS_PER_SEC;
    if(num_uncovered == 0)
    {
        cerr << "success, iterations: " << iter << " elapsed_time(s): " << elapsed_time
             << " frequency:" << double (iter) / elapsed_time << " SUW " << sum_uncovered_weight << endl;
        // print_vector("vertex weights: ", vertex_weights);
    }
    else
    {
        cerr << "not found, iterations: " << iter << " elapsed_time(s): " << elapsed_time
             << " frequency:" << double (iter) / elapsed_time << endl;
    }
}

// output solution;
void PCP_Vector::get_solution(vector<NodeId>& output)
{
    int count = 0;
    for (int i : solution_value)
    {
        output[count] = i;
        count++;
    }
}

// debug function:
void PCP_Vector::print_index1(const string& name, const dynamic_bitset<>& dbs)
{
    cerr << name << ": ";
    for (size_t i = dbs.find_first(); i != dynamic_bitset<>::npos; i = dbs.find_next(i))
    {
        cerr << i << "(" << center_weights[i] << ") ";
    }

    cerr << endl;
}

// debug function: construct random solution;
void PCP_Vector::sequence_construct()
{
    // center_weights start with each center's coverage;
    for(int i=0;i<center_weights.size();i++)
    {
        center_weights[i] = int (center_coverages[i].size());
    }
    // print_vector("center_weights before", center_weights);

    for(int i=0;i<num_center;i++) // do one iteration;
    {
        int selected_center = i;
        solution_value.push_back(selected_center);
        // cerr << "random select: " << rand_select << endl;
        // cerr << "selected center: " << selected_center << endl;

        // consequences of opening selected center, this part refers to Algorithm 3;
        // Refer to A4 LINE 2:
        // for all v属于Vi do
        // V(i): the set of vertex that center i can serve; index is center, result is vertex;
        // Meaning: consequences of opening i
        for (int v : center_coverages[selected_center])
        {
            // cerr << v << " ";
            // Reference A4 LINE 3:
            // if |X 交 Cv| = 1 then
            // X: current center set;
            // v: vertex
            // Cv: 覆盖顶点v的中心集合;
            // |X 交 Cv|: number of centers covering v in X;
            if (num_reach_solution[v] == 1)
            {
                // print_index1("solution", solution);
                // print_index1("Cv", Cv);
                int intersect_center = reach_one_solution[v];
                // cerr << "find intersect one: " << intersect_center << endl;

                // Refer to A4 LINE 4:
                // for l 属于 X交Cv:
                //     delta_l <- delta_l - wv,
                // l: X交Cv里面的中心; 现有解中覆盖V的中心;
                // delta_l: 既然l属于X, 那么把l删除后, covered的减量, uncovered的增量; (在里面越小越好);
                // Meaning: cancel penalty for deleting center l;
                // Comment: 由于|X 交 Cv| = 1, 所以这里面的循环只有一个数, 复杂度O(1).
                // Comment: 虽然中心l是当前中心集X中独一无二覆盖顶点v的, 但是由于swapped in的中心i也覆盖v, 所以它不再是不可或缺的了, 价值要减小. 这里减去的其实是LINE 14增加的量.
                // print_vector("center weights before", center_weights);
                center_weights[intersect_center] -= vertex_weights[v];
                // print_vector("center weights after", center_weights);
            }
                // Refer to A4 LINE 5:
                // else if |X 交 Cv| = 0 then
                // X: current center set;
                // Cv: center set covering vertex v;
                // Meaning: 如果即将加入X的中心i所覆盖的顶点v无法被X包含的中心们覆盖;
            else if(num_reach_solution[v] == 0)
            {
                uncovered_vertices.reset(v);
                reach_one_solution[v] = selected_center;

                // Refer to A4 LINE 6:
                // for l 属于 Cv-{i}:
                //     delta_l <- delta_l - wv
                // l: Cv中除i以外的中心; 由于|X 交 Cv| = 0, 所以l不在X中;
                // Cv: 覆盖顶点v的中心集合;
                // delta_l: 既然l不属于X, 那么把l并入X后, covered的增量, uncovered的减量; (在外面越大越好);
                // Meaning: cancel reward for adding center l;
                // Comment: 虽然不在X的中心l能够覆盖顶点v而X中的其他中心都不行, 但是由于swapped in的中心i也覆盖v, 所以它不再是必须加入的了, 价值要减小.
                // print_index1("solution", solution);
                // print_index1("Cv", Cv);
                // for (int l : vertex_reaching[v])
                for (int l : center_coverages[v])
                {
                    // cerr << "l: " << l << endl;
                    center_weights[l] -= vertex_weights[v];
                }
                center_weights[selected_center] += vertex_weights[v]; // {j} does not need to minus;
                // cerr << endl;

                // Refer to A4 LINE 7:
                // end if
            }

            // v is now covered by the selected center;
            num_reach_solution[v]++;

            // Refer to A4 LINE 8:
            // end for
        }
        // cerr << endl;

        // Evaluate A4 LINE 6:
        // dynamic_bitset<> test_0_59 = center_cover_vertex[0] & center_cover_vertex[59];
        // cerr << "0 intersect 59: " << test_0_59.count() << endl;

        // Evaluate A4 LINE 4:
        // dynamic_bitset<> test_0_59_12 = center_cover_vertex[0] & (center_cover_vertex[59] | center_cover_vertex[12]);
        // cerr << "0 intersect (59 U 12): " << test_0_59_12.count() << endl;

        // Refer to A4 LINE 9:
        // X <- X U {i}
        // X: current center set;
        // i: center swapped in;
        // Meaning: Open selected center;
        solution.set(selected_center);
        // cerr << "Cover after union size (" << covered.count() << "): " << endl;
        // print_index1("Covered", covered);
        // print_index1("Uncovered", uncovered);
    }
    // print_vector("center weights after", center_weights);

    // print_index1("Center selected", solution);
    // cerr << "sum_uncovered_weight: " << sum_uncovered_weight << endl;

    uncovered_value.clear();
    for (size_t i = uncovered_vertices.find_first(); i != dynamic_bitset<>::npos; i = uncovered_vertices.find_next(i))
    {
        uncovered_value.push_back(int (i));
    }
    num_uncovered = int (uncovered_value.size());
    sum_uncovered_weight = num_uncovered;
}


// debug function;
long long int PCP_Vector::get_iteration() const
{
    return iter;
}

// debug function:
void PCP_Vector::print_vector(const string& name, vector<int> &vect)
{
    cerr << name << ": ";
    for(int i : vect)
    {
        cerr << i << " ";
    }
    cerr << endl;
}

// debug function:
void PCP_Vector::print_equal_pair()
{
    cerr << "Equal pair: ";
    for(int i=0;i<equal_pair_count;i++)
    {
        cerr << "{" << equal_pair[i].center_in << ", ";
        cerr << equal_pair[i].center_out << "}";
    }
    cerr << endl;
}

// debug func: this func is to test whether sum_uncovered_weight is correct or not;
unsigned PCP_Vector::compute_sum_uncovered_weight()
{
    unsigned int sum = 0;
    for (size_t u = uncovered_vertices.find_first(); u != dynamic_bitset<>::npos; u = uncovered_vertices.find_next(u))
    {
        sum += vertex_weights[u];
    }

    return sum;
}

// (1) debug on laptop by clion:
// .\SDK_PCP.exe 999999 1 <C:\wamp64\www\npbenchmark\npbenchmark-main\SDK.PCP\data\pmed01.n100p005.txt >sln.pmed01.n100p005.txt
// (2) debug on laptop by g++ in command line:
// cd C:\wamp64\www\npbenchmark\npbenchmark-main\SDK.PCP
// g++ -static-libgcc -static-libstdc++ -I C:\boost_1_81_0 Main.cpp PCenter.cpp pcp_vector.cpp -O3 && .\a.exe 999999 1 <C:\wamp64\www\npbenchmark\npbenchmark-main\SDK.PCP\data\pmed01.n100p005.txt >sln.txt
// (3) debug on ubuntu gdb:
// g++ Main.cpp PCenter.cpp pcp_vector.cpp -g && gdb a.out
// r 999999 1 <./data/pmed01.n100p005.txt >sln.txt
// r 999999 1 <./data/pcb3038p200r141.txt  >sln.txt