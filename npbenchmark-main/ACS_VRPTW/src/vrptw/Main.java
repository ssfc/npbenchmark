package vrptw;

public class Main
{
    public static void main (String[] args)
    {
        int seed = Integer.parseInt(args[0]);
        String instance_name = args[1];

        // read in files;
        ReadIn read_in = new ReadIn();
        read_in.read_file(instance_name);

        // set parameters;
        Parameter parameter = new Parameter();

        // compute result;
        System.out.println("waiting for a while ... ...");
        AntColonySystem ACS = new AntColonySystem(parameter, read_in, seed);
        ACS.ACS_Strategy();

        // print result;
        ACS.print_result();
        ACS.check_answer();
    }
}

/*
 * running command:
 * home server: javac -d . VRPTW/*.java && java VRPTW.Main
 *
 */