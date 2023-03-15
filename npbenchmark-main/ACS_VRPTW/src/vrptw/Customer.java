package vrptw;

public class Customer
{
    int Number;//节点自身编号
    int R;//节点所属车辆路径编号
    int x; //节点横坐标
    // chatGPT: 变量横坐标通常命名为x或者X，具体取决于编程语言的命名规范和开发者的个人喜好。在Java中，一般使用小写字母x来表示变量横坐标值。
    int y; //节点纵坐标
    int ready_time; //时间窗开始时间;
    int due_time; // 时间窗结束时间;
    int Service; // 服务时长
    int Demand; //节点的需求容量

    public Customer()
    {
        Number = 0;
        R = 0;
        ready_time = 0;
        due_time = 0;
        Service = 0;
        x = 0;
        y = 0;
        Demand = 0;
    }

    public Customer copy()
    {
        Customer newCustomer = new Customer();
        newCustomer.Number = Number;
        newCustomer.R = R;
        newCustomer.ready_time = ready_time;
        newCustomer.due_time = due_time;
        newCustomer.Service = Service;
        newCustomer.x = x;
        newCustomer.y = y;
        newCustomer.Demand = Demand;
        return newCustomer;
    }
}