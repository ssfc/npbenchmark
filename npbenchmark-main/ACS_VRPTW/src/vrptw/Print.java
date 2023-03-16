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
}