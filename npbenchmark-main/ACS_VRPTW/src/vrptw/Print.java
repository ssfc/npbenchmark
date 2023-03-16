package vrptw;
/*
 * <p>Title: Print</p>
 * <p>Description: </p>
 * @author zll_hust
 * @date 2020年2月2日
 */

import java.util.ArrayList;

public class Print
{
    private double bestCost;
    public int capacity;
    private ArrayList <Route> bestRoutes=new ArrayList<>();
    private double[][] Graph;
    public Customer[] customers;

    Print(Solution bestSolution, ReadIn readIn)
    {
        bestCost = bestSolution.totalCost;
        bestRoutes = bestSolution.routes;
        Graph = readIn.Graph;
        customers = readIn.customers;
        capacity = readIn.capacity;
    }

    //结果输出
    public void check_answer()
    {
        boolean check_time = true;
        boolean check_cost = true;
        boolean check_capacity = true;
        // 检验距离计算是否正确
        double totalCost = 0;
        for (Route bestRoute : bestRoutes)
        {
            for (int j = 1; j < bestRoute.customers.size(); ++j)
            {
                totalCost += Graph[bestRoute.customers.get(j - 1)][bestRoute.customers.get(j)];
            }
        }
        // 防止精度损失
        if (Math.abs(totalCost - bestCost) > 1)
        {
            check_cost = false;
        }

        for (Route bestRoute : bestRoutes)
        {
            int time = 0;
            for (int j = 1; j < bestRoute.customers.size(); ++j)
            {
                time += Graph[bestRoute.customers.get(j - 1)][bestRoute.customers.get(j)];
                if (time > customers[bestRoute.customers.get(j)].due_time)
                    check_time = false;
                time = Math.max(time, customers[bestRoute.customers.get(j)].ready_time)
                        + customers[bestRoute.customers.get(j)].service_time;
            }
        }

        for (Route bestRoute : bestRoutes)
        {
            int load = 0;
            for (int j = 1; j < bestRoute.customers.size() - 1; ++j)
            {
                load += customers[bestRoute.customers.get(j)].Demand;
            }
            if (load > capacity)
                check_capacity = false;
        }

        System.out.println("Check total cost = " + totalCost + "\t" + check_cost);
        System.out.println("Check time windows = " + check_time);
        System.out.println("Check time capacity = " + check_capacity);
    }
}