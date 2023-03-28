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
        AntColonySystem test = new AntColonySystem(parameter, read_in, seed);
        test.ACS_Strategy();

        // print result;
        test.print_result();
        test.check_answer();
    }
}


// laptop path:
// C:\wamp64\www\npbenchmark\npbenchmark-main\ACS_VRPTW\src\vrptw
// home server path:
// /home/ssfc/ACS_VRPTW/src/vrptw
// debugging and running command:
// home server:
// javac -d . vrptw/*.java && java vrptw.Main 1 ./data/solomon.c101.n101v25c200.txt

