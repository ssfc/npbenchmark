package vrptw;

import static java.lang.Math.*;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.Scanner;

public class ReadIn
{
    int num_nodes; // 客户数量+1, 因为算上了仓库; 101
    int max_num_agents; // 最大可用车辆; 25
    int capacity; // 车辆容量; 200

    // Dimension: num__nodes == 客户数量+1
    // nodes[i]: 算例中第i行的数据;
    Node[] nodes;

    // Dimension: num__nodes * num__nodes == (客户数量+1) * (客户数量+1)
    // distance[i][j]: 点i和点j的距离;
    double[][] distance;

    //计算图上各节点间的距离
    private double compute_distance(Node c1, Node c2)
    {
        return ((int) 10 * sqrt((c1.x - c2.x) * (c1.x - c2.x) + (c1.y - c2.y) * (c1.y - c2.y)) ) ;
    }

    //读取数据
    public void read_file(String fileName)
    {
        try
        {
            Scanner in = new Scanner(new FileReader(fileName));

            num_nodes = in.nextInt();
            max_num_agents = in.nextInt();
            capacity = in.nextInt();

            nodes = new Node[num_nodes];
            distance = new double[num_nodes][num_nodes];
            for(int i = 0; i < num_nodes; i++)
            {
                nodes[i] = new Node();
            }

            for ( int i = 0; i < num_nodes; i++ ) // 之所以+1, 是因为编号从0开始, 仓库也算作一行;
            {
                nodes[i].x = in.nextInt();
                nodes[i].y = in.nextInt();
                nodes[i].demand = in.nextInt();
                nodes[i].service_time = in.nextInt() * 10;
                nodes[i].ready_time = in.nextInt() * 10;
                nodes[i].due_time = in.nextInt() * 10;
                //System.out.println("customerNr X,Y = " + customers[i].X + "\t" + customers[i].Y);
            }

            in.close();
        }
        catch(FileNotFoundException e)
        {
            // 未找到文件
            System.out.println("File not found!");
            System.exit(-1);
        }

        for(int i = 0; i < num_nodes; i++) // 之所以+1, 是因为编号从0开始, 仓库也算作一行;
        {
            for(int j = 0; j < num_nodes; j++) // 之所以+1, 是因为编号从0开始, 仓库也算作一行;
            {
                distance[i][j] = compute_distance(nodes[i], nodes[j]);
            }
        }
    }
}