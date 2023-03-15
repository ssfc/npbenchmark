package vrptw;

public class Customer
{
    int Number;//节点自身编号
    int R;//节点所属车辆路径编号
    int x; //节点横坐标
    int y; //节点纵坐标
    int Begin; //时间窗开始时间;
    int End; // 时间窗结束时间;
    int Service; // 服务时长
    int Demand; //节点的需求容量

    public Customer()
    {
        Number = 0;
        R = 0;
        Begin = 0;
        End = 0;
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
        newCustomer.Begin = Begin;
        newCustomer.End = End;
        newCustomer.Service = Service;
        newCustomer.x = x;
        newCustomer.y = y;
        newCustomer.Demand = Demand;
        return newCustomer;
    }
}