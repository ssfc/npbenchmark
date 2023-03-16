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
    double totalCost;
    ArrayList <Route> routes = new ArrayList<>();

    public void compute_tatalCost()
    {
        totalCost = 0;
        for (Route route : routes)
        {
            totalCost += route.distance;
        }
    }
}