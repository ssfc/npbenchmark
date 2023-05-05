package ttp.Utils;


import java.util.ArrayList;



/**
 * This is a Java port of the MATLAB code from
 * "The travelling thief problem: the first step in the transition from 
 * theoretical problems to realistic problems" 
 * (Bonyadi et al., Congress on Evolutionary Computation 2013)
 *
 * @author wagner
 */
public class DriverOld {
    public static void main (String[] args) {
        
        // test based on page 1041 of the above-mentioned article
        double[][] distances    = new double[][]{{0,5,6,6},{5,0,5,6},{6,5,0,4},{6,6,4,0}};
        double[] weights        = new double[]{3,1,1,2,3};
        double[] values         = new double[]{100,40,40,20,30};
        int[][] av              = new int[][]{{0,0,1,0},{0,0,1,0},{0,0,1,0},{0,1,0,1},{0,1,0,0}};
        int[] tour              = new int[]{0,2,1,3,0};
        int[] z                 = new int[]{-1,2,-1,1,-1};
        double weightofKnapsack = 3;
        double vmax             = 1;
        double vmin             = 0.1;
        double rentRate         = 1;
        TTP x = new TTP(distances, weights, values, av, tour, z, weightofKnapsack, vmax, vmin, rentRate);
        x.print();
        
        /* 
         * Set of additional tests to validate certain aspects of the problem.
         */
        distances           = new double[][]{{0,10,10},{10,0,10},{10,10,0}};
        weights             = new double[]{60,50,40,30,20};
        values              = new double[]{600,500,400,300,200};
        av                  = new int[][]{{1,1,1},{1,1,1},{1,1,1},{1,1,1},{1,1,1}};  //(5x3)
        tour                = new int[]{0,1,2,0};
        z                   = new int[]{2,2,2,2,2};
        weightofKnapsack    = 111;
        vmax                = 1;
        vmin                = 0;
        rentRate            = 1;
        new TTP(distances, weights, values, av, tour, z, weightofKnapsack, vmax, vmin, rentRate).print();
        
        // as before, but increased rent
        rentRate            = 2;
        new TTP(distances, weights, values, av, tour, z, weightofKnapsack, vmax, vmin, rentRate).print();
        
        // as before, but do not pickup item number 4
        z                   = new int[]{2,2,2,2,-1};
        new TTP(distances, weights, values, av, tour, z, weightofKnapsack, vmax, vmin, rentRate).print();
        
        // as before, but changed availabilities (nothing should change)
        av                  = new int[][]{{0,0,1},{0,0,1},{0,0,1},{0,0,1},{0,0,1}};
        new TTP(distances, weights, values, av, tour, z, weightofKnapsack, vmax, vmin, rentRate).print();
        
        // as before, but try to pick up an unavailable item (outputs an error message and a default solution)
        z                   = new int[]{2,2,2,2,1};
        new TTP(distances, weights, values, av, tour, z, weightofKnapsack, vmax, vmin, rentRate).print();
        
        // as before, but do not pick up any items at all
        z                   = new int[]{-1,-1,-1,-1,-1};
        new TTP(distances, weights, values, av, tour, z, weightofKnapsack, vmax, vmin, rentRate).print();
        
        //pick the same item up later and later
        av                  = new int[][]{{1,1,1},{1,1,1},{1,1,1},{1,1,1},{1,1,1}};
        z                   = new int[]{0,-1,-1,-1,-1};
        new TTP(distances, weights, values, av, tour, z, weightofKnapsack, vmax, vmin, rentRate).print();
        z                   = new int[]{1,-1,-1,-1,-1};
        new TTP(distances, weights, values, av, tour, z, weightofKnapsack, vmax, vmin, rentRate).print();
        z                   = new int[]{2,-1,-1,-1,-1};
        new TTP(distances, weights, values, av, tour, z, weightofKnapsack, vmax, vmin, rentRate).print();
    }
    
}
class TTP {
    double fp   = Double.NEGATIVE_INFINITY;
    double ft   = Double.POSITIVE_INFINITY;
    double ob   = Double.NEGATIVE_INFINITY;
    double wend = Double.POSITIVE_INFINITY;
    
    private void reset() {
        fp = Double.NEGATIVE_INFINITY;
        ft = Double.POSITIVE_INFINITY;
        ob = Double.NEGATIVE_INFINITY;
        wend = Double.POSITIVE_INFINITY;
    }
    
    public void print() {
        System.out.println("fp="+fp+" ft="+ft+" ob="+ob+" wend="+wend);
    }
    
    /**
     * Translated code of the original "TTP1Objective.m".
     * 
     * Important note: in contrast to the MATLAB code, city numbers start from 0
     * and item numbers start from 0.
     * 
     * @param distances         a n by n matrix that shows the distances between the cities (there are n cities)
     * @param weights           the weight of each item (1 by m)
     * @param values            the profit of each item (1 by m)
     * @param av                a m by n matrix showing if the ith item is available in the jth city. 
     * @param tour              a 1 by n+1 array showing the tour (a complete tour)
     * @param z                 a 1 by m array, showing which item from which city (if z(j)==i, it means item j from city i)  
     * @param weightofKnapsack  maximum weight of the knapsack
     * @param vmax              maximum velocity
     * @param vmin              minimum velocity
     * @param rentRate          the rent rate of the knapsack
     * @return TTP object: 
     *          "fp" final profit gained form the picked items,
     *          "ft" the time takes to finish the tour (including changes of the speed),
     *          "ob" objective value,
     *          "wend" weight of the knapsack at the end of the tour
     */
    public TTP(double[][] distances, 
            double[] weights, 
            double[] values,
            int[][] av,
            int[] tour,
            int[] z,
            double weightofKnapsack,
            double vmax,
            double vmin,
            double rentRate
            ) {
        
        // correctness check: does the tour start and end in the same city
        if(tour[0]!=tour[tour.length-1]) {
            System.out.println("ERROR: The last city must the same as the first city");
        }

        
        double wc=0;
        ft=0;
        fp=0;
        
        for (int i=0; i<tour.length-1; i++) {
            
            // determine all the items that are picked up in the current city
            ArrayList selectedItem = new ArrayList();
            for (int j=0; j<z.length; j++) {
                if (z[j]==tour[i]) {
                    selectedItem.add(j);
                }
            }
            
            // correctness check: are all to-be-picked items available in the current city
            int availabilityCounter = 0;
            for (Object o:selectedItem) {
                int currentItem = ((Integer)o).intValue();
                if (av[currentItem][tour[i]]==1)
                    availabilityCounter++;
            }
            if(availabilityCounter!=selectedItem.size()) {
                System.out.println("ERROR: One or more items are not available at this city "+tour[i]);
                this.reset();
                return;
            }

            // do the actual TTP computations
            if(!selectedItem.isEmpty()) {
                for (Object o:selectedItem) {
                    int currentItem = ((Integer)o).intValue();
                    wc=wc+weights[currentItem];
                    fp=fp+values[currentItem];
                }
            }
            int h=i+1; //h: next tour city index
            ft=ft+
              (distances[tour[i]][tour[h]] / (1-wc*(vmax-vmin)/weightofKnapsack));
        }
        
        wend=weightofKnapsack-wc;
        ob=fp-ft*rentRate;
    }
}