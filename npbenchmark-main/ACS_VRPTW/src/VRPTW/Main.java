package VRPTW;

public class Main {
    public static void main (String arg[]) {
        long begin_time = System.nanoTime();

        Parameter parameter = new Parameter();
        ReadIn readIn = new ReadIn();
        readIn.Read("c101.txt");

        System.out.println("waiting for a while ... ...");
        AntColonySystem ACS = new AntColonySystem(parameter, readIn);
        Solution bestSolution = ACS.ACS_Strategy();

        Print print = new Print(bestSolution, readIn);
        print.Output();
        print.CheckAns();

        long end_time = System.nanoTime();
        double usedTime= (end_time - begin_time)/(1e9);
        System.out.println();
        System.out.println("Run time(s): "+usedTime);
    }
}

/*
 * running command:
 * home server: javac -d . VRPTW/*.java && java VRPTW.Main
 *
 */