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
        double elapsed_time= (end_time - begin_time)/(1e9); // 因为是纳秒, 所以除以1e9换算;
        System.out.println();
        System.out.println("elapsed time(s): "+ elapsed_time);
    }
}

/*
 * running command:
 * home server: javac -d . VRPTW/*.java && java VRPTW.Main
 *
 */