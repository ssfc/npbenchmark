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
