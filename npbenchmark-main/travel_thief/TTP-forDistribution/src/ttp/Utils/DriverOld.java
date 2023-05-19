/*
这是一个Java文件，包含两个类：DriverOld和TTP。

DriverOld类有一个main方法，用于测试TTP类的功能。

TTP类是一个用于求解旅行商窃贼问题（Travelling Thief Problem）的类，它接收一些参数，如城市之间的距离矩阵，物品的重量和价值，物品在城市中的可用性，旅行商的行程，窃贼的背包容量，最大和最小速度，背包租金等。
TTP类有一个构造方法，用于根据这些参数计算最终的利润、时间、目标值和背包重量，并有一个print方法，用于打印这些结果。
TTP类还有一些私有方法，用于重置结果和检查参数的正确性。
 */

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
/*
public class DriverOld是一个Java类，它的作用是用于测试TTP类的功能。
它有一个main方法，用于创建一些测试数据，如城市之间的距离矩阵，物品的重量和价值，物品在城市中的可用性，旅行商的行程，窃贼的背包容量，最大和最小速度，背包租金等。
然后它用这些数据创建一个TTP对象，并调用它的print方法，打印出最终的利润、时间、目标值和背包重量。
它还有一些额外的测试，用于验证TTP类的一些方面，如不同的租金率，不同的物品选择，不同的物品可用性等。
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

/*
根据当前的网页内容，class TTP是一个Java类，它的作用是用于求解旅行商窃贼问题（Travelling Thief Problem）。
这个类有一些属性，如fp，ft，ob，wend，用于存储最终的利润、时间、目标值和背包重量。
它有一个构造方法，用于根据一些参数，如城市之间的距离矩阵，物品的重量和价值，物品在城市中的可用性，旅行商的行程，窃贼的背包容量，最大和最小速度，背包租金等，计算这些属性的值。
它还有一个print方法，用于打印这些属性的值。它还有一些私有方法，用于重置属性的值和检查参数的正确性。
 */
class TTP {
    double fp   = Double.NEGATIVE_INFINITY;
    double ft   = Double.POSITIVE_INFINITY;
    double ob   = Double.NEGATIVE_INFINITY;
    double wend = Double.POSITIVE_INFINITY;

    /*
    它的作用是重置TTP类的属性，即将fp，ft，ob，wend的值分别设为负无穷，正无穷，负无穷，正无穷。
     */
    private void reset() {
        fp = Double.NEGATIVE_INFINITY;
        ft = Double.POSITIVE_INFINITY;
        ob = Double.NEGATIVE_INFINITY;
        wend = Double.POSITIVE_INFINITY;
    }

    /*
    打印TTP类的属性，即将fp，ft，ob，wend的值输出到控制台。
     */
    public void print() {
        System.out.println("fp="+fp+" ft="+ft+" ob="+ob+" wend="+wend);
    }

    /*
    它是TTP类的构造方法。它的作用是根据一些参数，创建一个TTP对象，并计算它的属性。它有十个参数，分别是：
    distances: 一个二维数组，表示城市之间的距离矩阵
    weights: 一个一维数组，表示物品的重量
    values: 一个一维数组，表示物品的价值
    av: 一个二维数组，表示物品在城市中的可用性 (Comment: available的简称?)
    tour: 一个一维数组，表示旅行商的行程
    z: 一个一维数组，表示窃贼从哪个城市偷取哪个物品
    weightofKnapsack: 一个数值，表示窃贼的背包容量
    vmax: 一个数值，表示最大速度
    vmin: 一个数值，表示最小速度
    rentRate: 一个数值，表示背包租金
    它没有返回值，但是会给TTP对象赋值fp，ft，ob，wend四个属性。它有一个修饰符，叫做public，它表示这个方法是公开的，可以被其他类访问。
     */
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