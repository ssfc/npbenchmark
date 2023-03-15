package vrptw;

public class Parameter
{
    public int IterMax;
    public double Alpha, w1, w2;
    public int Beta, Sita;
    public double[][] Graph;

    Parameter()
    {
        IterMax = 100;
        Alpha = 0.05;
        Beta = 1;
        Sita = 5;
        w1 = 0.9;
        w2 = 0.1;
    }

    Parameter(int interation, double alpha, int beta, int sita, double input_w1, double input_w2)
    {
        IterMax = interation;
        Alpha = alpha;
        Beta = beta;
        Sita = sita;
        w1 = input_w1;
        w2 = input_w2;
    }
}