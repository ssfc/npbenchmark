package vrptw;

import static java.lang.Math.*;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.Scanner;

public class ReadIn
{
    int customerNr; // 客户数量
    int capacity; // 车辆容量
    double[][] graph; // 记录图
    Customer[] customers; // 存储客户数据

    //计算图上各节点间的距离
    private double compute_distance ( Customer c1, Customer c2 )
    {
        return sqrt ( ( c1.x - c2.x ) * ( c1.x - c2.x ) + ( c1.y - c2.y ) * ( c1.y - c2.y ) );
    }

    //读取数据
    public void read_file(String fileName)
    {
        try
        {
            Scanner in = new Scanner(new FileReader(fileName));

            in.next();
            customerNr = in.nextInt();
            capacity = in.nextInt();

            customers = new Customer[customerNr + 9];
            graph = new double[customerNr + 9][customerNr + 9];
            for(int i = 0; i < customerNr + 9; i++)
            {
                customers[i] = new Customer();
            }

            for ( int i = 0; i <= customerNr; ++i )
            {
                customers[i].node_number = in.nextInt();
                customers[i].x = in.nextInt();
                customers[i].y = in.nextInt();
                customers[i].demand = in.nextInt();
                customers[i].ready_time = in.nextInt();
                customers[i].due_time = in.nextInt();
                customers[i].service_time = in.nextInt();
                //System.out.println("customerNr X,Y = " + customers[i].X + "\t" + customers[i].Y);
            }

            in.close();
        }
        catch (FileNotFoundException e)
        {
            // 未找到文件
            System.out.println("File not found!");
            System.exit(-1);
        }

        for ( int i = 0; i <= customerNr; ++i )
        {
            for (int j = 0; j <= customerNr; ++j)
            {
                graph[i][j] = compute_distance(customers[i], customers[j]);
            }
        }
    }
}