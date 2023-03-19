package vrptw;

import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.Random;

/**
 * <p>Title: ACS</p>
 * <p>Description: </p>
 * @author zll_hust
 * @date 2020年2月2日
 */
public class AntColonySystem
{
    // member variables;
    int num_nodes; // 客户数量+1, 因为算上了仓库; 101
    int num_agents; // agent数量
    int max_num_agents; // 最大可用车辆
    int capacity; // 车辆容量

    // Dimension: num__nodes == 客户数量+1
    // Meaning: 存储客户数据, 就是算例中一行一行的东西;
    Node[] nodes;

    // Dimension: num__nodes * num__nodes == (客户数量+1) * (客户数量+1)
    // Meaning: 记录图
    double[][] graph;

    // Dimension: num__nodes == 客户数量 + 1
    // Meaning: 记录每一位agent k未服务过的客户
    ArrayList<Integer>[] untreated;
    int iter;
    int max_iter; // 最大迭代次数

    // Dimension: num__nodes == 客户数量 + 1
    // Meaning: agents
    Solution[] solutions;
    Solution best_solution;

    // Dimension: num__nodes == 客户数量 + 1
    // Meaning: agent k 出发位置、当前位置、下一位置
    int[] agent_position;

    // Dimension: num__nodes * num__nodes == (客户数量+1) * (客户数量+1)
    // Meaning: 信息素
    double[][] pheromone;

    // Dimension: num__nodes * num__nodes == (客户数量+1) * (客户数量+1)
    // Meaning: 启发值
    double[][] heuristic;

    // Dimension: num__nodes * num__nodes == (客户数量+1) * (客户数量+1)
    // Meaning: infoPhe = pheromone ^ beta * herustic ^ sita
    double[][] infoPhe;
    double pheromone_0; // 信息素初始值
    double w1; // 计算概率的参数
    double w2; // 计算概率的参数
    double alpha; // 计算infoPhe的参数，
    double beta; // 计算infoPhe的参数，
    double sita; // 计算infoPhe的参数，
    Random generated_random;

    // member functions;
    // constructor;
    public AntColonySystem(Parameter parameter, ReadIn readIn, int seed)
    {
        num_nodes = readIn.num_nodes;
        num_agents = readIn.num_nodes - 1;
        max_num_agents = readIn.max_num_agents;
        capacity = readIn.capacity;
        graph = readIn.graph;
        nodes = readIn.nodes;
        iter = 0;
        max_iter = parameter.max_iter;
        solutions = new Solution[num_nodes]; // 设置初始agents数量和城市数一样多, 后面会慢慢减小;
        untreated = new ArrayList[num_nodes]; // 数组数量等于agents数
        for (int i = 0; i < num_nodes; i++)
        {
            untreated[i] = new ArrayList<>();
        }
        agent_position = new int[num_nodes];
        pheromone = new double[num_nodes][num_nodes];
        heuristic = new double[num_nodes][num_nodes];
        infoPhe = new double[num_nodes][num_nodes];
        alpha = parameter.alpha;
        beta = parameter.beta;
        sita = parameter.Sita;
        w1 = parameter.w1;
        w2 = parameter.w2;
        generated_random = new Random(seed);
    }

    // 初始化总体参数
    public void init()
    {
        // 计算信息素初始值
        double total_distance = 0;
        double num = 0;
        for (int i = 0; i < num_nodes; i++)
        {
            for (int j = 0; j < num_nodes; j++)
            {
                if (i != j)
                {
                    total_distance += graph[i][j];
                    num ++;
                }
            }
        }

        pheromone_0 = num / (total_distance * num_nodes);

        // 初始化信息素、启发值
        for (int i = 0; i < num_nodes; i++)
        {
            for (int j = 0; j < num_nodes; j++)
            {
                if (i != j)
                {
                    pheromone[i][j] = pheromone_0;
                    pheromone[j][i] = pheromone_0;
                    heuristic[i][j] = 1 / graph[i][j];
                    heuristic[j][i] = 1 / graph[i][j];
                }
            }
        }
    }

    // 初始化agent参数
    public void reset()
    {
        // 初始化每位agent未服务的客户
        for (int i = 0; i < num_agents; i++)
        {
            untreated[i].clear();
            for ( int j = 1; j < num_nodes; j++) // 之所以从1开始算, 是因为0是仓库, 1开始才是客户;
            {
                untreated[i].add(j);
            }
        }
        // 初始化起始服务客户
        for (int i = 0; i < num_agents; i++)
        {
            solutions[i] = new Solution();
            agent_position[i] = 0;
        }

    }

    // 构造完整解
    public void construct_solution()
    {
        // 为每一位agent分别构造解
        for (int i = 0; i < num_agents; i++)
        {
            // 路径开始
            Route route = new Route();
            route.customers.add(0);

            while(untreated[i].size() != 0)
            {
                int next = select_next(i, route);

                // 如果下一个选择不合法或客户已配送完毕
                if (next == 0)
                {
                    route.customers.add(0);
                    route.time += graph[agent_position[i]][0];
                    route.distance += graph[agent_position[i]][0];
                    solutions[i].routes.add(route);
                    solutions[i].total_cost += route.distance;
                    route = new Route();
                    route.customers.add(0);
                    agent_position[i] = 0;
                }
                else
                {
                    route.customers.add(next);
                    route.load += nodes[next].demand;
                    route.time = Math.max(route.time + graph[agent_position[i]][next], nodes[next].ready_time) + nodes[next].service_time;
                    route.distance += graph[agent_position[i]][next];
                    agent_position[i] = next;
                    for (int j = 0; j < untreated[i].size(); j++)
                    {
                        if (untreated[i].get(j) == next)
                        {
                            untreated[i].remove(j);
                        }
                    }
                }
            }
            // 最后一条路径返回配送中心
            route.customers.add(0);
            route.time = Math.max(graph[agent_position[i]][0], nodes[0].ready_time) + nodes[0].service_time;
            route.distance += graph[agent_position[i]][0];
            solutions[i].routes.add(route);
            solutions[i].total_cost += route.distance;
        }
    }

    public int select_next(int k, Route route)
    {
        // 若全部处理完，返回配送中心
        if (untreated[k].size() == 0)
        {
            return 0;
        }

        // 计算概率
        double sumPhe = 0;
        double sumTime = 0;
        double[] infoPhe = new double[num_agents];
        double[] infoTime = new double[num_agents];
        for (int i = 0; i < untreated[k].size(); i++)
        {
            infoPhe[i] =Math.pow(pheromone[agent_position[k]][untreated[k].get(i)], beta)
                    * Math.pow(heuristic[agent_position[k]][untreated[k].get(i)], sita);
            infoTime[i] = 1 / (Math.abs(route.time - nodes[untreated[k].get(i)].ready_time) +
                    Math.abs(route.time - nodes[untreated[k].get(i)].due_time));
            sumPhe += infoPhe[i];
            sumTime += infoTime[i];
        }

        double rate = generated_random.nextDouble();
        int next = 0;
        double sum_prob = 0;

        // 生成0-1随机数，累加概率，若大于当前累加部分，返回当前城市编号
        for (int i = 0; i < untreated[k].size(); i++)
        {
            sum_prob += infoPhe[i] * w1 / sumPhe + infoTime[i] * w2 / sumTime;
            if (rate < sum_prob)
            {
                next = untreated[k].get(i);
                // 检验合法性
                double time = route.time + graph[agent_position[k]][next];
                double load = route.load + nodes[next].demand;
                if (time > nodes[next].due_time || load > capacity)
                    continue;
                else
                    break;
            }
        }
        // 检验合法性
        double time = route.time + graph[agent_position[k]][next];
        double load = route.load + nodes[next].demand;
        if (time > nodes[next].due_time || load > capacity)
        {
            next = 0;
        }

        return next;
    }

    // 更新信息素
    public void update_pheromone()
    {
        Solution now_best = new Solution();
        now_best.total_cost = Integer.MAX_VALUE;
        double delta = 0;

        // 查找最优解
        for (int i = 0; i < num_agents; i++)
        {
            if (solutions[i].total_cost < now_best.total_cost)
                now_best = solutions[i];
        }

        // 更新最优解 若当前最优代替历史最优，增加信息素时获得增益
        if (now_best.total_cost < best_solution.total_cost)
        {
            delta = (best_solution.total_cost - now_best.total_cost) / best_solution.total_cost;
            best_solution = now_best;
        }

        //更新信息素含量
        // 信息素挥发
        for (int i = 0; i < num_nodes-1; i ++) // 这里难道不应该是node__nodes吗? 为什么少一个?
        {
            for (int j = 0; j < num_nodes-1; j++)
            {
                pheromone[i][j] *= (1 - alpha);
            }
        }
        // 信息素增加
        for (int i = 0; i < now_best.routes.size(); i ++)
        {
            for (int j = 1; j < now_best.routes.get(i).customers.size(); j++)
            {
                pheromone[now_best.routes.get(i).customers.get(j - 1)][now_best.routes.get(i).customers.get(j)]
                        += (1 / (double)now_best.total_cost) * (1 + delta);
                // 对称处理
                pheromone[now_best.routes.get(i).customers.get(j)][now_best.routes.get(i).customers.get(j - 1)]
                        = pheromone[now_best.routes.get(i).customers.get(j - 1)][now_best.routes.get(i).customers.get(j)];
            }
        }
    }

    public void ACS_Strategy()
    {
        DecimalFormat df = new DecimalFormat("#.##");
        long begin_time = System.nanoTime();
        best_solution = new Solution();
        best_solution.total_cost = Integer.MAX_VALUE;
        init();
        while (iter < max_iter)
        {
            reset();//初始化agent信息
            construct_solution();//对于所有的agent构造一个完整的tour
            update_pheromone();//更新信息素
            if(iter % 5 == 0)
            {
                double elapsed_time= (System.nanoTime() - begin_time)/(1e9); // 因为是纳秒, 所以除以1e9换算;
                System.out.println("iter: " + iter + "\tnum agents: " + best_solution.routes.size()
                        + "\tbest solution cost: " + df.format(best_solution.total_cost)
                        + "\telapsed time(s): " + df.format(elapsed_time)
                        + "\tfrequency: " + df.format((double) iter / elapsed_time));
            }

            iter++;
        }

        double elapsed_time= (System.nanoTime() - begin_time)/(1e9); // 因为是纳秒, 所以除以1e9换算;
        System.out.println();
        System.out.println("success, iterations: " + iter + "\tnum agents: " + best_solution.routes.size()
                         + "\telapsed time(s): " + df.format(elapsed_time)
                         + "\tfrequency: " + df.format((double) iter / elapsed_time));
    }

    // debug function
    public void print_result()
    {
        System.out.println("************************************************************");
        System.out.println("The Minimum Total Distance = " + best_solution .total_cost);
        System.out.println("Concrete Schedule of Each Route as Following : ");

        int id = 0;
        for (int i = 1; i < best_solution.routes.size(); i++)
        {
            if (best_solution.routes.get(i).customers.size() > 2)
            {
                id++;
                System.out.print("No." + id + " : ");

                for (int j = 0; j < best_solution.routes.get(i).customers.size() - 1; ++j)
                {
                    System.out.print(best_solution.routes.get(i).customers.get(j) + " -> ");
                }
                System.out.println(best_solution.routes.get(i).customers.get(best_solution.routes.get(i).customers.size() - 1));
            }
        }
        System.out.println("************************************************************");
    }

    // debug function
    public void check_answer()
    {
        boolean check_time = true;
        boolean check_cost = true;
        boolean check_capacity = true;
        boolean check_num_agents = true;
        // 检验距离计算是否正确
        double total_cost = 0;
        for (Route best_route : best_solution.routes)
        {
            for (int j = 1; j < best_route.customers.size(); ++j)
            {
                total_cost += graph[best_route.customers.get(j - 1)][best_route.customers.get(j)];
            }
        }
        // 防止精度损失
        if (Math.abs(total_cost - best_solution .total_cost) > 1)
        {
            check_cost = false;
        }

        for (Route best_route : best_solution.routes)
        {
            int time = 0;
            for (int j = 1; j < best_route.customers.size(); ++j)
            {
                time += graph[best_route.customers.get(j - 1)][best_route.customers.get(j)];
                if (time > nodes[best_route.customers.get(j)].due_time)
                {
                    check_time = false;
                }
                time = Math.max(time, nodes[best_route.customers.get(j)].ready_time)
                        + nodes[best_route.customers.get(j)].service_time;
            }
        }

        for (Route best_route : best_solution.routes)
        {
            int load = 0;
            for (int j = 1; j < best_route.customers.size() - 1; ++j)
            {
                load += nodes[best_route.customers.get(j)].demand;
            }
            if (load > capacity)
            {
                check_capacity = false;
            }
        }

        if(best_solution.routes.size() > max_num_agents)
        {
            check_num_agents = false;
        }

        System.out.println("Check total cost = " + total_cost + "\t" + check_cost);
        System.out.println("Check time windows = " + check_time);
        System.out.println("Check time capacity = " + check_capacity);
        System.out.println("Check num agents = " + check_num_agents);
    }
}