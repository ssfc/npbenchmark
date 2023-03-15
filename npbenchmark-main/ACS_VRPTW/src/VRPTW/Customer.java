package VRPTW;

public class Customer
{
    int Number;//节点自身编号
    int R;//节点所属车辆路径编号
    int X, Y;//节点横纵坐标
    int Begin, End, Service;//节点被访问的最早时间，最晚时间以及服务时长
    int Demand;//节点的需求容量

    public Customer()
    {
        Number = 0;
        R = 0;
        Begin = 0;
        End = 0;
        Service = 0;
        X = 0;
        Y = 0;
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
        newCustomer.X = X;
        newCustomer.Y = Y;
        newCustomer.Demand = Demand;
        return newCustomer;
    }
}