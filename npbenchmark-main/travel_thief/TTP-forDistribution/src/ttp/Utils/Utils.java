/*
它包含一个叫做Utils的类，定义了一些实用的方法，用于计时、文件列表、排列和创建集群数组。
它还有一个主方法，调用了createClusterArray方法。
这段代码似乎与一种涉及旅行商和背包实例的优化问题有关。
 */
/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package ttp.Utils;

import java.io.File;
import java.io.FilenameFilter;
import java.io.IOException;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 *
 * @author wagner
 */
public class Utils {
    
    public static long startTime = 0;

    /*
    这个函数是一个静态的长整型方法，名为startTiming()。
    它的作用是获取当前的系统时间（以毫秒为单位），并将其赋值给一个静态的长整型变量startTime，然后返回startTime的值。
    这个函数可以用于开始计时某个操作或过程的执行时间。
     */
    public static long startTiming() {
        startTime = System.currentTimeMillis();
        return startTime;
    }

    /*
    它的作用是获取当前的系统时间（以毫秒为单位），并将其赋值给一个局部的长整型变量stopTime，然后计算stopTime和startTime之间的差值，
    即某个操作或过程的执行时间，赋值给一个局部的长整型变量timeUsed，然后返回timeUsed的值。
    这个函数可以用于停止计时某个操作或过程的执行时间。
     */
    public static long stopTiming() {
        long stopTime = System.currentTimeMillis(); 
        long timeUsed = stopTime-startTime;
//        long timeUsed = (stopTime-startTime)/1000;
//        System.out.println("[time used: "+timeUsed+"s]") ;
        return timeUsed;
    }

    /*
    它的作用是获取当前的系统时间（以毫秒为单位），并将其赋值给一个局部的长整型变量stopTime，然后计算stopTime和startTime之间的差值，
    即某个操作或过程的执行时间，赋值给一个局部的长整型变量timeUsed，然后在控制台打印出timeUsed的值（以毫秒为单位），
    并在前面加上"[time used: “，在后面加上"ms]”，最后返回timeUsed的值。
    这个函数可以用于停止计时并打印出某个操作或过程的执行时间。
     */
    public static long stopTimingPrint() {
        long stopTime = System.currentTimeMillis(); 
        long timeUsed = (stopTime-startTime);
        System.out.println("[time used: "+timeUsed+"ms]") ;
//        long timeUsed = (stopTime-startTime)/1000;
//        System.out.println("[time used: "+timeUsed+"s]") ;
        return timeUsed;
    }

    /*
    它的作用是根据一个字符串数组参数args，返回一个文件数组。
    它首先创建一个文件对象f，以args[0]作为路径，然后尝试打印出f的规范路径。
    然后它调用f的listFiles()方法，传入一个匿名的文件名过滤器对象，该对象的accept()方法判断一个文件名是否包含".ttp"和args[1]，
    如果是则返回true，否则返回false。
    这样就可以筛选出符合条件的文件，并存入一个文件数组fa。
    最后，如果开启了调试打印模式，就遍历fa并打印出每个文件的绝对路径，然后返回fa。
    这个函数可以用于获取某个目录下的特定类型和名称的文件列表。
     */
    public static File[] getFileList(final String[] args) {
        
        boolean debugPrint = false;
        
        File f = new File(args[0]);
        try {
            if (debugPrint) System.out.println(f.getCanonicalPath());
        } catch (IOException ex) {
        }
        
        File[] fa = f.listFiles(new FilenameFilter() {
            public boolean accept(File dir, String name) {
                boolean result = false;
                if (name.contains(".ttp") 
                        && name.contains(args[1])
                        ) result = true;
                return result;
            }});
        
        if (debugPrint)
            for (File temp:fa) {
                System.out.println(temp.getAbsolutePath());
            }
        
        return fa;
    }

    /*
    它的作用是根据一个整型参数N，返回一个长度为N的整型数组，该数组包含从0到N-1的所有整数，并且随机打乱顺序。
    它首先创建一个长度为N的整型数组a，并用一个循环将a[i]赋值为i。
    然后它用另一个循环，从0到N-1遍历i，每次随机生成一个0到i之间的整数r，然后交换a[r]和a[i]的值。这样就可以得到一个随机排列的数组a，并返回它。这个函数可以用于生成一个随机的排列。
     */
    public static int[] permutation(int N) {
        int[] a = new int[N];
        for (int i = 0; i < N; i++) 
            a[i] = i;
        // shuffle
        for (int i = 0; i < N; i++) {
            int r = (int) (Math.random() * (i+1));     // int between 0 and i
            int swap = a[r];
            a[r] = a[i];
            a[i] = swap;
        }
        return a;
    }
    
    /*
    它的作用是创建一个字符串数组，用于存储一些命令行脚本，然后打印出这些脚本。
    它首先定义了一些字符串数组，分别存储了旅行商问题的实例名称、背包问题的类型、背包问题的容量和实例编号。
    然后定义了一个脚本的位置和一个子文件夹的名称。
    接着定义了一个整型变量overallSanity，用于统计总共生成了多少个脚本。
    然后用一个嵌套的四重循环，遍历所有的旅行商实例、背包容量、背包类型和实例编号，每次拼接出一个脚本的字符串，并打印出来，
    同时增加sanity和overallSanity的值。每遍历完一个旅行商实例，就打印出sanity的值。
    最后打印出overallSanity的值，并用另一个循环打印出一些wget命令，用于下载旅行商问题的实例文件。
    这个函数可以用于生成一些用于运行优化问题的脚本。
     */
    public static void createClusterArray() {
        String[] tspNames = new String[]{"a280",
"berlin52",
"bier127",
"brd14051",
"ch130",
"ch150",
"d198",
"d493",
"d657",
"d1291",
"d1655",
"d2103",
"d15112",
"d18512",
"dsj1000",
"eil51",
"eil76",
"eil101",
"fl417",
"fl1400",
"fl1577",
"fl3795",
"fnl4461",
"gil262",
"kroA100",
"kroA150",
"kroA200",
"kroB100",
"kroB150",
"kroB200",
"kroC100",
"kroD100",
"kroE100",
"lin105",
"lin318",
"nrw1379",
"p654",
"pcb442",
"pcb1173",
"pcb3038",
"pla7397",
"pla33810",
"pla85900",
"pr76",
"pr107",
"pr124",
"pr136",
"pr144",
"pr152",
"pr226",
"pr264",
"pr299",
"pr439",
"pr1002",
"pr2392",
"rat99",
"rat195",
"rat575",
"rat783",
"rd100",
"rd400",
"rl1304",
"rl1323",
"rl1889",
"rl5915",
"rl5934",
"rl11849",
"st70",
"ts225",
"tsp225",
"u159",
"u574",
"u724",
"u1060",
"u1432",
"u1817",
"u2152",
"u2319",
"usa13509",
"vm1084",
"vm1748"};
        String[] knapsackTypes = new String[] {"bounded-strongly-corr","uncorr-similar-weights","uncorr"};
        int[] knapsackCapacities = new int[]{1,3,5,10};
        
        int instancesCount = 10;
        String[] instances = new String[instancesCount];
        for (int i=1; i<=instancesCount; i++) {
            
            String value = "";
            if (i<10) value += "0";
            value += i;
            
            instances[i-1] = value;
        }
        
        
        
        String scriptLocation = "/home/mwagner/scratch/ttp/driver.sh";
        
        String subfolder = "instances";
        
        
        
        
        int overallSanity = 0;
        
        for (String tsp:tspNames) {
            
            int sanity = 0;

            Matcher matcher = Pattern.compile("[0-9]+").matcher(tsp);
            matcher.find();
            
            int numberOfCities = Integer.parseInt(matcher.group());
//            String substring = tsp.substring(indexFirst);
            
//            System.out.println(tsp+" "+matcher.group());
            
//            matcher = Pattern.compile("\\s").matcher(tsp);
//            matcher.find();
//            int indexSecond = Integer.valueOf(matcher.group());
            
//            int numberOfCities = Integer.parseInt(substring.substring(0, indexSecond));
            
//            System.out.println(numberOfCities);
            
            
            for (int cap:knapsackCapacities) {
            
                for (String type:knapsackTypes) {
                    
                    for (String i:instances) {

                        String result = "";

                        result += scriptLocation + " " + subfolder + " " 
                                + tsp +"_n"+cap*(numberOfCities-1)+"_"
                                + type + "_" + i+".ttp "
                                ;
                        
                        result += "1 10000 600000"; // approach, max iterations without impr., time in ms.

                        System.out.println(result);
                        sanity++;
                        
                    }
                
                }
            }
            overallSanity += sanity;
            System.out.println("sanity="+sanity);
        }
        System.out.println("overallSanity="+overallSanity);
        
        
        for (String s:tspNames) {
            System.out.println("wget -c http://cs.adelaide.edu.au/~optlog/CEC2014COMP_Instances/"
                    +s+"-ttp.rar ");
        }
        
    }
    
    public static void main(String[] args) {
        createClusterArray();
    }
}
