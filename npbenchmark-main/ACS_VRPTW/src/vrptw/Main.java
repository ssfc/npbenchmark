package vrptw;

public class Main
{
    public static void main (String[] args)
    {
        int seed = Integer.parseInt(args[0]);
        String instance_name = args[1];

        long begin_time = System.nanoTime();

        // read in files;
        ReadIn read_in = new ReadIn();
        read_in.Read(instance_name);

        // set parameters;
        Parameter parameter = new Parameter();

        // compute result;
        System.out.println("waiting for a while ... ...");
        AntColonySystem ACS = new AntColonySystem(parameter, read_in, seed);
        Solution bestSolution = ACS.ACS_Strategy();

        // print result;
        ACS.print_result();
        ACS.check_answer();

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