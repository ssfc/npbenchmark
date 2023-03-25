package vrptw;

// Customer.java很明显是个结构体类, 就不要硬改private了
public class Node
{
    // member variables;
    int x; //节点横坐标
    // chatGPT: 变量横坐标通常命名为x或者X，具体取决于编程语言的命名规范和开发者的个人喜好。在Java中，一般使用小写字母x来表示变量横坐标值。
    int y; //节点纵坐标
    int window_begin; //时间窗开始时间;
    int window_end; // 时间窗结束时间;
    int min_stay_time; // 服务时长
    int demand; //节点的需求容量

    // member functions;
    // constructor;
    public Node()
    {
        window_begin = 0;
        window_end = 0;
        min_stay_time = 0;
        x = 0;
        y = 0;
        demand = 0;
    }
}