package vrptw;

// Customer.java很明显是个结构体类, 就不要硬改private了
public class Customer
{
    // member variables;
    int node_index;//节点自身编号
    int route_number;//节点所属车辆路径编号
    int x; //节点横坐标
    // chatGPT: 变量横坐标通常命名为x或者X，具体取决于编程语言的命名规范和开发者的个人喜好。在Java中，一般使用小写字母x来表示变量横坐标值。
    int y; //节点纵坐标
    int ready_time; //时间窗开始时间;
    int due_time; // 时间窗结束时间;
    int service_time; // 服务时长
    int demand; //节点的需求容量

    // member functions;
    // constructor;
    public Customer()
    {
        node_index = 0;
        route_number = 0;
        ready_time = 0;
        due_time = 0;
        service_time = 0;
        x = 0;
        y = 0;
        demand = 0;
    }
}