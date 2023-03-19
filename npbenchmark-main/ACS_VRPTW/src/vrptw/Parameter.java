package vrptw;

// Parameter.java很明显是个结构体类, 就不要硬改private了
public class Parameter
{
    int max_iter;
    double alpha;
    int beta;
    int theta;
    double w1;
    double w2;
    double[][] Graph;

    Parameter()
    {
        max_iter = 50;
        alpha = 0.05;
        beta = 1;
        theta = 5;
        w1 = 0.9;
        w2 = 0.1;
    }

    Parameter(int input_max_iter, double input_alpha, int input_beta, int input_theta, double input_w1, double input_w2)
    {
        max_iter = input_max_iter;
        alpha = input_alpha;
        beta = input_beta;
        theta = input_theta;
        w1 = input_w1;
        w2 = input_w2;
    }
}