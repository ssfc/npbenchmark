/*
这个文件是一个Java类，定义了一个TTPSolution对象，代表了旅行商偷窃问题（TTP）的一个解决方案。
这个类有一些字段，比如tspTour，packingPlan，fp，ft，ob等，用来存储解决方案的信息，比如城市的旅行路线，要打包的物品，最终的利润，花费的时间，目标值等。
这个类还有一些方法，比如reset，print，printFull，getObjective，answer和writeResult，用来对解决方案对象进行各种操作，比如重置字段，打印信息，计算目标值，格式化答案输出，和将结果写入文件。
 */

package ttp;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

/**
 *
 * @author wagner
 */
public class TTPSolution {
    
    public int[] tspTour;
    public int[] packingPlan;
    
/* "fp" final profit gained form the picked items,
   "ft" the time takes to finish the tour (including changes of the speed),
   "ob" objective value,
   "wend" weight of the knapsack at the end of the tour   
*/
    public double fp   = Double.NEGATIVE_INFINITY;
    public double ft   = Double.POSITIVE_INFINITY;
    public long ftraw   = Long.MAX_VALUE;
    public double ob   = Double.NEGATIVE_INFINITY;
    public double wend = Double.POSITIVE_INFINITY;
    public double wendUsed = Double.POSITIVE_INFINITY;
    public long computationTime = Long.MAX_VALUE;

    /*
    函数public void reset()是TTPSolution类的一个方法，用来将解决方案对象的所有字段重置为初始值。
    例如，它将fp设置为Double.NEGATIVE_INFINITY，
    ft设置为Double.POSITIVE_INFINITY，
    ob设置为Double.NEGATIVE_INFINITY等。
    这个方法在解决方案对象需要被重用或修改为不同的问题实例时很有用。
     */
    public void reset() {
        fp = Double.NEGATIVE_INFINITY;
        ft = Double.POSITIVE_INFINITY;
        ftraw = Long.MAX_VALUE;
        ob = Double.NEGATIVE_INFINITY;
        wend = Double.POSITIVE_INFINITY;
        wendUsed = Double.POSITIVE_INFINITY;
        computationTime = Long.MAX_VALUE;
    }

    /*
    函数public void print()是TTPSolution类的一个方法，用来将解决方案对象的信息打印到标准输出。
    它以格式化的方式打印fp，ft和ob这三个字段，然后打印tspTour和packingPlan这两个数组。
    这个方法在解决方案对象需要被显示或检查以便调试时很有用。
     */
    public void print() {
        System.out.print(wend+" "+wendUsed+" "+fp+" "+ftraw+" "+ft+" "+ob +" "+computationTime);
    }

    /*
    函数public void println()是TTPSolution类的一个方法，用来将解决方案对象的信息打印到标准输出，并在末尾加上一个换行符。
    它以格式化的方式打印fp，ft和ob这三个字段，然后打印tspTour和packingPlan这两个数组。
    然后它打印一个换行符(“\n”)。这个方法在解决方案对象需要被显示或检查以便调试并与其他输出分隔时很有用。
     */
    public void println() {
        this.print();
        System.out.println();
    }

    /*
    函数public void printFull()是TTPSolution类的一个方法，用来将解决方案对象的信息打印到标准输出，并为每个字段和数组加上标签。
    它以格式化的方式打印wend，wendUsed，fp，ftraw，ft，ob和computationTime这七个字段，并附上它们的名称。
    然后它打印tspTour和packingPlan这两个数组，并附上它们的名称。
    这个方法在解决方案对象需要被显示或检查以便调试并通过标签识别时很有用。
     */
    public void printFull() {
        this.println();
        System.out.println("tspTour "+Arrays.toString(tspTour));
        System.out.println("packingPlan "+Arrays.toString(packingPlan));
    }

    /*
    函数public double getObjective()是TTPSolution类的一个方法，用来返回解决方案对象的目标值。
    目标值是衡量解决方案对于旅行商偷窃问题的优劣的一个指标。它存储在解决方案对象的ob字段中。
    这个方法在解决方案对象需要被评估或与其他解决方案比较时很有用。
     */
    public double getObjective() {
        return this.ob;
    }

    /*
    函数public TTPSolution(int[] tspTour, int[] packingPlan)是TTPSolution类的一个构造器(构造函数)，
    用来根据给定的tspTour和packingPlan数组创建一个新的解决方案对象。
    tspTour数组是旅行商偷窃问题中要访问的城市的一个排列。
    packingPlan数组是一个二进制向量，表示要在背包中打包哪些物品。
    这个构造器在需要从现有的数组创建一个新的解决方案对象时很有用。
     */
    public TTPSolution(int[] tspTour, int[] packingPlan) {
        this.tspTour = tspTour;
        this.packingPlan = packingPlan;
    }
    
    
    
    /* answer() and writeResult(String file) are necessary for the participation in the competition.
     * 
     * Text from http://cs.adelaide.edu.au/~optlog/CEC2014Comp/ follows:
     * - Output: the file with the result for a single iteration should follow the following naming convention: 
     *           <ttpfile>.<algorithmname>.<systemtime>, e.g.,
     *           a280_n279_bounded-strongly-corr_02.ttp.thisIsMyAlgorithm.12345678912
     * - The file needs to contain the following information as comma-separated values in square brackets: 
     *           the permutation of the cities in the first line (note: the first city is "1", do not print the "1" at the end), and 
     *           the list of packed items in the second line (the numbering of the items starts with "1"). 
     *           For example:
     *                  [1,5,4,2,3]
     *                  [20,113]
     *           which means that only the items with the numbers 20 and 113 are picked, and the sequence of visited cities is <1,5,4,2,3,1>. 
     *           Note that this format can easily be achieved in Java with the function Arrays.toString(...). 
     */
    /*
    函数private String answer()是TTPSolution类的一个方法，用来返回解决方案对象的一个特定格式的字符串表示。
    这个格式包括两行：第一行是tspTour数组中城市的排列（每个城市的编号加1），
    第二行是packingPlan数组中打包的物品的列表（每个物品的编号加1并按升序排序）。
    这个方法在解决方案对象需要被输出为一个竞赛或一个标准评估时很有用。
     */
    private String answer() {
    	int[] tourOut = new int[tspTour.length - 1];
    	for (int i = 0; i < tspTour.length - 1; i++){
    		tourOut[i] = tspTour[i] + 1;
    	}
    	
    	int itemsPerCity = packingPlan.length / (tspTour.length  -2);
    	
    	List<Integer> packingPlanList = new ArrayList<Integer>();
    	int packingPlanIndex = 0;
    	for (int i = 1; i < tspTour.length - 1; i++){
    		int city = tspTour[i];
    		for (int j = 0; j < itemsPerCity; j++){
    			if (packingPlan[packingPlanIndex] == 1){
    				int itemIndex = j * (tspTour.length  -2) + city - 1;
    				int itemIndexFrom1 = itemIndex + 1;
    				packingPlanList.add(itemIndexFrom1 );
    			}
    			packingPlanIndex++;
    		}	    	
    	}   	
    	Collections.sort(packingPlanList);
    	
    	int[] packingOut = new int[packingPlanList.size()];
    	for (int i = 0; i < packingPlanList.size(); i++){
    		packingOut[i] = packingPlanList.get(i);
    	}
        return Arrays.toString(tourOut) + "\n" + Arrays.toString(packingOut) +  "\n";
    }
    
    public void writeResult(String title){
            BufferedWriter writer = null;
            try {
                    writer = new BufferedWriter(new FileWriter(title, false));
                    writer.write(this.answer());
                    writer.flush();
                    writer.close();
            } catch (IOException e) {
                    e.printStackTrace();

            }
    }

}
