package vrptw;

public class Parameter
{
    int IterMax;
    double Alpha, w1, w2;
    int Beta, Sita;
    double[][] Graph;

    Parameter()
    {
        IterMax = 50;
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