package ttp;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author wagner
 */
public class TTPInstance {

    public static void main(String[] args) {
        TTPInstance test = new TTPInstance(new File("instances/a280_n1395_bounded-strongly-corr_1.ttp"));
    }
    
    public String problemName;
    public String knapsackDataType;
    public int numberOfNodes;
    public int numberOfItems;
    public long capacityOfKnapsack;
    public double minSpeed;
    public double maxSpeed;
    public double rentingRatio;
    public String edgeWeightType;
    public double[][] nodes;
    public int[][] items;
    
    public File file;
    
    /* Constructor 
     * reads the instance from the file
     */
    public TTPInstance(File file) {
        boolean debugPrint = !true;
        if (debugPrint) System.out.println(file.getAbsolutePath());
        BufferedReader br = null;
        try {
            br = new BufferedReader(new FileReader(file));
            this.file = file;
            String line;
            while ((line = br.readLine()) != null) {
                // process the line

                if (line.startsWith("PROBLEM NAME")) {
                    line = line.substring(line.indexOf(":")+1);
                    line = line.replaceAll("\\s+","");
                    this.problemName = line;
                }
                if (line.startsWith("KNAPSACK DATA TYPE")) {
                    line = line.substring(line.indexOf(":")+1);
                    line = line.replaceAll("\\s+","");
                    this.knapsackDataType = line;
                }
                if (line.startsWith("DIMENSION")) {
//                if (line.startsWith("NUMBER OF NODES")) {
                    line = line.substring(line.indexOf(":")+1);
                    line = line.replaceAll("\\s+","");
                    this.numberOfNodes=Integer.parseInt(line);
                }
                if (line.startsWith("NUMBER OF ITEMS")) {
                    line = line.substring(line.indexOf(":")+1);
                    line = line.replaceAll("\\s+","");
                    this.numberOfItems=Integer.parseInt(line);
                }
                if (line.startsWith("CAPACITY OF KNAPSACK")) {
                    line = line.substring(line.indexOf(":")+1);
                    line = line.replaceAll("\\s+","");
                    this.capacityOfKnapsack=Long.parseLong(line);
                }
                if (line.startsWith("MIN SPEED")) {
                    line = line.substring(line.indexOf(":")+1);
                    line = line.replaceAll("\\s+","");
                    this.minSpeed=Double.parseDouble(line);
                }
                if (line.startsWith("MAX SPEED")) {
                    line = line.substring(line.indexOf(":")+1);
                    line = line.replaceAll("\\s+","");
                    this.maxSpeed=Double.parseDouble(line);
                }
                if (line.startsWith("RENTING RATIO")) {
                    line = line.substring(line.indexOf(":")+1);
                    line = line.replaceAll("\\s+","");
                    this.rentingRatio=Double.parseDouble(line);
                }
                if (line.startsWith("EDGE_WEIGHT_TYPE")) {
                    line = line.substring(line.indexOf(":")+1);
                    line = line.replaceAll("\\s+","");
                    this.edgeWeightType = line;
                }
                if (line.startsWith("NODE_COORD_SECTION")) {
                    this.nodes = new double[this.numberOfNodes][3];
                    for (int i=0; i<this.numberOfNodes; i++) {
                        line = br.readLine();
                        String[] splittedLine = line.split("\\s+");
                        for (int j=0; j<splittedLine.length; j++) {
                            double temp = Double.parseDouble(splittedLine[j]);
//                            int temp = Integer.parseInt(splittedLine[j]);
                            // adjust city number by 1
                            if (j==0) temp =  temp-1;
                            this.nodes[i][j] = temp;
                        }
                    }
                }
                if (line.startsWith("ITEMS SECTION")) {
                    this.items = new int[this.numberOfItems][4];
                    for (int i=0; i<this.numberOfItems; i++) {
                        line = br.readLine();
                        String[] splittedLine = line.split("\\s+");
                        for (int j=0; j<splittedLine.length; j++) {
                            int temp = Integer.parseInt(splittedLine[j]);
                            // adjust city number by 1
                            if (j==0) temp =  temp-1;  // item numbers start here with 0 --> in TTP files with 1
                            if (j==3) temp =  temp-1;  // city numbers start here with 0 --> in TTP files with 1
                            this.items[i][j] = temp;
                        }
                    }
                }
            }
            br.close();
        } catch (IOException ex) {
        }
//        if (debugPrint || true) printInstance(false);
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
     * @param z                 a 1 by m array, showing which item from which city (if z(i)==j, it means item i from city j)  -->>>> the other way around: 
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
    public void evaluate(TTPSolution solution) {
        
        
        
        boolean debugPrint = !true;
        
        int[] tour = solution.tspTour;
        int[] z = solution.packingPlan;
        long weightofKnapsack = this.capacityOfKnapsack;
        double rentRate = this.rentingRatio;
        double vmin = this.minSpeed;
        double vmax = this.maxSpeed;
        solution.ftraw = 0;
//            double[][] distances, 
//            double[] weights, 
//            double[] values,
//            int[][] av,
//            int[] tour,
//            int[] z,
//            double weightofKnapsack,
//            double vmax,
//            double vmin,
//            double rentRate
//            ) {
        
        // correctness check: does the tour start and end in the same city
        if(tour[0]!=tour[tour.length-1]) {
            System.out.println("ERROR: The last city must be the same as the first city");
            solution.reset();
            return;
        }
        
        double wc=0;
        solution.ft=0;
        solution.fp=0;
        
        /* the following is used for a different interpretation of "packingPlan"
         * 
         */
        int itemsPerCity = solution.packingPlan.length / (solution.tspTour.length-2);
        if (debugPrint) System.out.println("itemsPerCity="+itemsPerCity+" solution.tspTour.length="+solution.tspTour.length);
       
//        for (int i=0; i<tour.length; i++) {
        for (int i=0; i<tour.length-1; i++) {
            
//            // determine all the items that are picked up in the current city
//            ArrayList selectedItem = new ArrayList();
//            for (int j=0; j<z.length; j++) {
//                if (z[j]==tour[i]) {
//                    selectedItem.add(j);
//                }
//            }
//            
//            // correctness check: are all to-be-picked items available in the current city
//            int availabilityCounter = 0;
//            for (Object o:selectedItem) {
//                int currentItem = ((Integer)o).intValue();
//                if (av(currentItem,tour[i])==1)
////                if (av[currentItem][tour[i]]==1)
//                    availabilityCounter++;
//            }
//            if(availabilityCounter!=selectedItem.size()) {
//                System.out.println("ERROR: One or more items are not available at this city "+tour[i]);
//                solution.reset();
//                return;
//            }
//
//            // do the actual TTP computations
//            if(!selectedItem.isEmpty()) {
//                if (debugPrint) System.out.print("packing: ");
//                for (Object o:selectedItem) {
//                    int currentItem = ((Integer)o).intValue();
//                    if (debugPrint) System.out.print(currentItem+"(p="+this.items[currentItem][1]+") ");
//                    wc=wc+this.items[currentItem][2];
////                    wc=wc+weights[currentItem];
//                    solution.fp=solution.fp+this.items[currentItem][1];
////                    solution.fp=solution.fp+values[currentItem];
//                }
//                if (debugPrint) System.out.println();
//            }
            
            // important: nothing to be picked at the first city!
            if (debugPrint) System.out.print("\ni="+i+" checking packing: ");
            
            int currentCityTEMP = tour[i]; // what's the current city? --> but the items start at city 2 in the TTP file, so I have to take another 1 off!
            
            int currentCity = currentCityTEMP-1;
            
            if (i>0) if (debugPrint) System.out.print("city "+currentCityTEMP+" cityIndexForItem[][] "+currentCity+" (this.numberOfNodes="+this.numberOfNodes+"): ");
            
            if (i>0) 
                for (int itemNumber=0; itemNumber<itemsPerCity; itemNumber++) {
                int indexOfPackingPlan = (i-1)*itemsPerCity+itemNumber;
                if (debugPrint) System.out.print("indexOfPackingPlan="+indexOfPackingPlan+" ");
                
                // what is the next item's index in items-array?
                int itemIndex = currentCity+itemNumber*(this.numberOfNodes-1);//* (this.numberOfNodes-1); 
                if (debugPrint) System.out.print("itemIndex="+itemIndex+" ");
                
                if (z[indexOfPackingPlan]==1) {
                    // pack item
//                    int itemIndex = currentCity+itemNumber*(this.numberOfNodes-1);//* (this.numberOfNodes-1); 
//                    int itemIndex = (i-1)+itemNumber* (this.numberOfNodes-1); // GECCO incorrect
                    
//                    if (debugPrint) System.out.print("itemIndex="+itemIndex+" ");
                    
                    int currentWC = this.items[itemIndex][2];
                    wc=wc+currentWC;
                    
                    int currentFP=this.items[itemIndex][1];
                    solution.fp=solution.fp+currentFP;
                    
                    if (debugPrint) System.out.print("[fp="+currentFP+",wc="+currentWC+"] ");
                }
            }
            if (debugPrint) System.out.println();
            
            int h= (i+1)%(tour.length-1); //h: next tour city index
            if (debugPrint) System.out.println("  i="+i+" h="+h + " tour[i]="+tour[i]+" tour[h]="+tour[h]);
            
            long distance = (long)Math.ceil(distances(tour[i],tour[h]));
            
            
            
            // compute the raw distance
            solution.ftraw += distance;
            
            // compute the adjusted (effective) distance
            solution.ft=solution.ft+
              (distance / (vmax-wc*(vmax-vmin)/weightofKnapsack)); // "1-" replaced by "vmax-" on 11/09/2016 based on comment by Christopher Crouch
//              (distances[tour[i]][tour[h]] / (1-wc*(vmax-vmin)/weightofKnapsack));
            
            if (debugPrint) System.out.println("i="+i+" tour[i]="+tour[i]+" tour[h]="+tour[h]+" distance="+distance+" fp="+solution.fp + " ft=" + solution.ft);
        }
        
        solution.wendUsed = wc;
        solution.wend=weightofKnapsack-wc;
        solution.ob=solution.fp-solution.ft*rentRate;
        
        
        
    }
    
    
    
//    public int av(int currentItem, int city) {
///*        int result = 1;
//        for (int j=city; j<this.numberOfItems; ) {
//            if (this.items[currentItem]==this.items[city]) {
//                return 1;
//            }
//            j+=this.numberOfNodes-1;
//        }
//        return 0;
//        */
//        
//        /* Assume that items are always available in a city.
//         * It is up to the algorithm to ensure that no unpackable items are in the plan.
//         */
//        return 1;
//    }
    
    // used to simulate the distance matrix
    public double distances(int i, int j) {
        double result = 0;
        result = Math.sqrt(
                            (this.nodes[i][1]-this.nodes[j][1]) *
                            (this.nodes[i][1]-this.nodes[j][1]) + 
                            (this.nodes[i][2]-this.nodes[j][2]) *
                            (this.nodes[i][2]-this.nodes[j][2]) 
                        );
        
        if (!true) System.out.println(" distance="+this.nodes[i][1]+ " "
                +this.nodes[j][1]+" "+this.nodes[i][2]+" "+this.nodes[j][2]+"->"+result);
        
        return result;
    }
    
    public void printInstance() {
        printInstance(true);
    }
    
    /* prints the details
     * shortSummary: 
     *   true   prints a short version in one line
     *   false  prints the node and item data as well
     */
    public void printInstance(boolean shortSummary) {
        if (shortSummary) {
            System.out.print("TTP Instance: ");
        } else {
            System.out.println("---- TTP Instance START ----");
        }
        
        System.out.println(this.problemName+
                " "+this.knapsackDataType+
                " "+this.numberOfNodes+
                " "+this.numberOfItems+
                " "+this.capacityOfKnapsack+
                " "+this.minSpeed+
                " "+this.maxSpeed+
                " "+this.rentingRatio);
        
        if (shortSummary) {
        } else {
            for (double[] i:this.nodes) {
                System.out.println(Arrays.toString(i));
            }
            for (int[] i:this.items) {
                System.out.println(Arrays.toString(i));
            }
            System.out.println("---- TTP Instance END ----");
        }
    }
}
