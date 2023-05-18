// ttp.Optimisation包含了迭代启发式算法随机局部搜索(RLS)和(1+1)进化算法(EA)。通过参数“mode”，用户可以在RLS(mode==1)和EA(mode==2)之间切换。switch-case语句可以轻松扩展以纳入其他启发式算法。
/*
该包实现了两种迭代启发式算法：随机局部搜索(RLS)和(1+1)进化算法(EA)。
这些算法可以用来解决旅行商问题(TSP)和背包问题(KP)的组合，即旅行窃贼问题(TTP)。
用户可以通过参数“mode”来选择使用RLS(mode==1)或者EA(mode==2)。
代码文件还包含了一些辅助方法，如使用linkern程序生成TSP的最优解，或者使用stivale2结构初始化物理内存管理器。
 */
package ttp.Optimisation;


import java.io.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import ttp.TTPInstance;
import ttp.TTPSolution;
import ttp.Utils.DeepCopy;

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author wagner
 */
public class Optimisation {
    
    /*
    函数public static TTPSolution hillClimber(TTPInstance instance, int[] tour, int mode, int durationWithoutImprovement, int maxRuntime)
    的作用是使用随机局部搜索(RLS)或者(1+1)进化算法(EA)来优化旅行窃贼问题(TTP)的解决方案。
    函数的参数如下：
    - instance: 一个TTPInstance对象，表示TTP的实例，包含了城市、物品、距离、重量、价值等信息。
    - tour: 一个整数数组，表示TSP的一个解，即城市的访问顺序。
    - mode: 一个整数，表示使用RLS(mode==1)或者EA(mode==2)来优化背包问题(KP)的解，即物品的装载方案。
    - durationWithoutImprovement: 一个整数，表示在没有改进的情况下，算法继续运行的次数上限。
    - maxRuntime: 一个整数，表示算法运行的时间上限（毫秒）。

    函数的返回值是一个TTPSolution对象，表示TTP的一个解决方案，包含了tour和packingPlan两个属性，分别表示城市的访问顺序和物品的装载方案。
    函数还计算了该解决方案的目标值(ob)和最终背包剩余容量(wend)，并记录了算法运行的时间(computationTime)。

    随机局部搜索(RLS)和(1+1)进化算法(EA)都是一种启发式搜索方法，它们适用于优化问题，使用局部搜索来寻找最优解¹。
    它们都是迭代算法，从一个任意的解开始，然后通过对解进行微小的改变来寻找更好的解。
    如果改变后的解更好，就接受它并继续迭代；否则就拒绝它并尝试其他改变。
    这两种算法的区别在于改变解的方式：RLS每次只改变解中的一位（即装载或不装载一个物品），
    而EA每次以一定概率改变解中的每一位（即以1/n的概率装载或不装载每个物品，其中n是物品数量）²。
    这两种算法都属于贪心方法，即总是朝着目标函数增加（或减少）的方向移动，并且不回溯³。
    这两种算法对于凸优化问题（目标函数是凸函数）能够找到全局最优解，而对于其他类型的问题通常只能找到局部最优解⁴。
     */
    public static TTPSolution hillClimber(TTPInstance instance, int[] tour, 
            int mode, 
            int durationWithoutImprovement, int maxRuntime) {
        
        ttp.Utils.Utils.startTiming();
        
        TTPSolution s = null;
        boolean debugPrint = !true;
        
        int[] packingPlan = new int[instance.numberOfItems];
        
                
        boolean improvement = true;
        double bestObjective = Double.NEGATIVE_INFINITY;
        
        long startingTimeForRuntimeLimit = System.currentTimeMillis()-200;
        
        int i = 0;
        int counter = 0;
        while(counter<durationWithoutImprovement) {
            
            if (i%10==0 /*do the time check just every 10 iterations, as it is time consuming*/
                    && (System.currentTimeMillis()-startingTimeForRuntimeLimit)>=maxRuntime)
                break;
            
            
            if (debugPrint) {
                System.out.println(" i="+i+"("+counter+") bestObjective="+bestObjective); 
            }
            int[] newPackingPlan = (int[])DeepCopy.copy(packingPlan);
            
            boolean flippedToZero = false;
            
            switch (mode) {
                case 1: 
                    // flip one bit
                    int position = (int)(Math.random()*newPackingPlan.length);
//                    newPackingPlan[position] = Math.abs(newPackingPlan[position]-1);
                    if (newPackingPlan[position] == 1) {
                                newPackingPlan[position] = 0;
                                // investigation: was at least one item flipped to zero during an improvement?
//                                flippedToZero = true;
                    } else {
                        newPackingPlan[position] = 1;
                    }
                    break;
                case 2:
                    // flip with probability 1/n
                    for (int j=0; j<packingPlan.length; j++) {
                        if (Math.random()<1d/packingPlan.length)
                            if (newPackingPlan[j] == 1) {
                                newPackingPlan[j] = 0;
                                // investigation: was at least one item flipped to zero during an improvement?
//                                flippedToZero = true;
                            } else {
                                newPackingPlan[j] = 1;
                            }
                    }
                    break;
            }
            
            
            
//            ttp.Utils.Utils.startTiming();
            TTPSolution newSolution = new TTPSolution(tour, newPackingPlan);
            instance.evaluate(newSolution);
//            System.out.println(ttp.Utils.Utils.stopTiming());
            
                        
            /* replacement condition:
             *   objective value has to be at least as good AND
             *   the knapsack cannot be overloaded
             */
            if (newSolution.ob >= bestObjective && newSolution.wend >=0 ) {
                
                // for the stopping criterion: check if there was an actual improvement 
                if (newSolution.ob > bestObjective && newSolution.wend >=0) {
                    improvement = true;
                    counter = 0;
                }
                
                packingPlan = newPackingPlan;
                s = newSolution;
                bestObjective = newSolution.ob;
                
            } else {
                improvement = false;
                counter ++;
            }
            
            i++;
            
        }
        
        long duration = ttp.Utils.Utils.stopTiming();
        s.computationTime = duration;
        return s;
    }

    /*
    函数public static int[] linkernTour(TTPInstance instance)的作用是使用linkern程序来生成TSP的最优解。
    函数的参数如下：
    instance: 一个TTPInstance对象，表示TTP的实例，包含了城市、物品、距离、重量、价值等信息。
    函数的返回值是一个整数数组，表示TSP的一个解，即城市的访问顺序。
    函数先根据TTP实例的文件名找到对应的TSP文件，然后调用linkern程序来求解TSP，并将结果保存在一个文件中。
    最后，函数从文件中读取结果，并返回一个整数数组。
     */
    public static int[] linkernTour(TTPInstance instance) {
        int[] result = new int[instance.numberOfNodes+1];
        
        boolean debugPrint = !true;
        
        String temp = instance.file.getAbsolutePath();
        int index = temp.indexOf("_");
        String tspfilename = temp;//.substring(0,index)+".tsp";
        if (index==-1) index = tspfilename.indexOf(".");
        String tspresultfilename = temp.substring(0,index)+".linkern.tour";
        
        if (debugPrint) System.out.println("LINKERN: "+tspfilename);
    
        File tspresultfile = new File(tspresultfilename);
        
        
        try {
            if (!tspresultfile.exists()) {
                List<String> command = new ArrayList<String>();
                command.add("./linkern");
                command.add("-o");
                command.add(tspresultfilename);
                command.add(tspfilename);
//                printListOfStrings(command);

                ProcessBuilder builder = new ProcessBuilder(command);
                builder.redirectErrorStream(true);
                final Process process = builder.start();
                InputStream is = process.getInputStream();
                InputStreamReader isr = new InputStreamReader(is);
                BufferedReader br = new BufferedReader(isr);
                String line;
                while ((line = br.readLine()) != null) {
                    if (debugPrint) System.out.println("<LINKERN> "+line);
                }
                if (debugPrint) System.out.println("Program terminated?");    
                int rc = process.waitFor();
                if (debugPrint) System.out.println("Program terminated!");
            }

            List<String> command = new ArrayList<String>();
            command.add("cat");
            command.add(tspresultfilename);
//            printListOfStrings(command);
            ProcessBuilder builder = new ProcessBuilder(command);
            builder.redirectErrorStream(true);
            final Process process = builder.start();
            InputStream is = process.getInputStream();
            InputStreamReader isr = new InputStreamReader(is);
            BufferedReader br = new BufferedReader(isr);
            // discard the first line
            String line = br.readLine();                    
            for (int i=0; i<result.length; i++) {
                line = br.readLine();
                if (debugPrint) System.out.println("<TOUR> "+line);
                index = line.indexOf(" ");
                int number = Integer.parseInt(line.substring(0,index));
                result[i] = number;
                if (debugPrint) System.out.println(Arrays.toString(result));
            }
            if (debugPrint) System.out.println("Program terminated?");    
            int rc = process.waitFor();
            if (debugPrint) System.out.println("Program terminated!");
            
            } catch (Exception ex) {
            }
        return result;
    }

    /*
    函数public static void doAllLinkernTours()的作用是对所有的TSP文件使用linkern程序来生成最优解，并将结果保存在文件中。
    函数先获取instances/tsplibCEIL目录下的所有TSP文件，然后对每个TSP文件，根据文件名生成一个结果文件名，如果结果文件不存在，就调用linkern程序来求解TSP，并将结果保存在结果文件中。
    linkern程序是Concorde软件包的一部分，它实现了一种称为Chained Lin-Kernighan heuristic的启发式算法，用于求解TSP²。
    该算法使用可变的k边交换来改进一个初始路径³。该程序不包含在当前网页内容中，需要单独获取和安装。
     */
    public static void doAllLinkernTours() {
        
        boolean debugPrint = false;
        
        File f = new File("instances/tsplibCEIL");
//        File f = new File("instances/");
        try {
            if (debugPrint) System.out.println(f.getCanonicalPath());
        } catch (IOException ex) {
        }
        
        File[] fa = f.listFiles(new FilenameFilter() {
            public boolean accept(File dir, String name) {
                boolean result = false;
//                if (name.contains(".ttp") 
                if (name.contains(".tsp") 
                        ) result = true;
                return result;
            }});
        
        if (debugPrint)
            for (File temp:fa) {
                System.out.println(temp.getAbsolutePath());
            }
        
        // create a nonsense instance just to be able to run linkernTour/1 on it
//        TTPInstance instance = new TTPInstance(new File("."));        
//        int[] tour = new int[0];
//        tour = Optimisation.linkernTour(instance);
        
        
        
//        int[] result = new int[instance.numberOfNodes+1];
//        
//        boolean debugPrint = !true;
//        
//        String temp = instance.file.getAbsolutePath();
//        int index = temp.indexOf("_");
        for(File tsp:fa) {
            String tspfilename = tsp.getAbsolutePath();
            int index = tspfilename.indexOf("_");
            if (index==-1) index = tspfilename.indexOf(".");
            String tspresultfilename = tspfilename.substring(0, index) +".linkern.tour";
//            int index = tspfilename.indexOf(".tsp");
//            String tspresultfilename = tspfilename.substring(0, index) +".linkern.tour";
//            String tspresultfilename = tspfilename+".linkern.tour";

            if (debugPrint) System.out.println("LINKERN: "+tspfilename);

            File tspresultfile = new File(tspresultfilename);

            try {
                if (! tspresultfile.exists()) {
                    List<String> command = new ArrayList<String>();
                    command.add("./linkern");
                    command.add("-o");
                    command.add(tspresultfilename);
                    command.add(tspfilename);
//                    printListOfStrings(command);

                    ProcessBuilder builder = new ProcessBuilder(command);
                    builder.redirectErrorStream(true);
                    
                    ttp.Utils.Utils.startTiming();
                    
                    final Process process = builder.start();
                    InputStream is = process.getInputStream();
                    InputStreamReader isr = new InputStreamReader(is);
                    BufferedReader br = new BufferedReader(isr);
                    String line;
                    while ((line = br.readLine()) != null) {
                        if (debugPrint) System.out.println("<LINKERN> "+line);
                    }
                    if (debugPrint) System.out.println("Program terminated?");    
                    int rc = process.waitFor();
                    
                    long duration = ttp.Utils.Utils.stopTiming();
                    
                    System.out.println( new File(tspresultfilename).getName() +" "+duration);
                    
                    if (debugPrint) System.out.println("Program terminated!");
                }
                
                
                
                
                } catch (Exception ex) {
                }
        }
        
    }
    
    public static void printListOfStrings(List<String> list) {
        String result = "";
        for (String s:list)
            result+=s+" ";
        System.out.println(result);
    }
}
