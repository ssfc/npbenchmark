package vrptw;

// Parameter.java很明显是个结构体类, 就不要硬改private了
public class Parameter
{
    int max_iter;
    double Alpha;
    int Beta;
    int Sita;
    double w1;
    double w2;
    double[][] Graph;

    Parameter()
    {
        max_iter = 50;
        Alpha = 0.05;
        Beta = 1;
        Sita = 5;
        w1 = 0.9;
        w2 = 0.1;
    }

    Parameter(int interation, double alpha, int beta, int sita, double input_w1, double input_w2)
    {
        max_iter = interation;
        Alpha = alpha;
        Beta = beta;
        Sita = sita;
        w1 = input_w1;
        w2 = input_w2;
    }
}