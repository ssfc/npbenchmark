package VRPTW;

public class Parameter {
    public int IterMax;//ËÑË÷µü´ú´ÎÊý
    public double Alpha, w1, w2;
    public int Beta, Sita;
    public double[][] Graph;//¼ÇÂ¼Í¼

    Parameter() {
        this.IterMax = 500;
        this.Alpha = 0.05;
        this.Beta = 1;
        this.Sita = 5;
        this.w1 = 0.9;
        this.w2 = 0.1;
    }

    Parameter(int interation, double alpha, int beta, int sita, double w1, double w2) {
        this.IterMax = interation;
        this.Alpha = alpha;
        this.Beta = beta;
        this.Sita = sita;
        this.w1 = w1;
        this.w2 = w2;
    }
}