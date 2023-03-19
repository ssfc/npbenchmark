package vrptw;

import java.util.ArrayList;

/**
 * <p>Title: SolutionType</p>
 * <p>Description: </p>
 * @author zll_hust
 * @date 2020Äê2ÔÂ2ÈÕ
 */
public class Solution
{
    double total_cost;
    ArrayList <Route> routes = new ArrayList<>();

    public void compute_total_cost()
    {
        total_cost = 0;
        for (Route route : routes)
        {
            total_cost += route.distance;
        }
    }
}