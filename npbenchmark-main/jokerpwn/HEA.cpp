//
// Created by take_ on 2022/12/31.
//
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using   namespace std;
void string2arc(const string &a,vector<int> &b,const string &delim){
    string::size_type pos1=a.find(delim),pos2=a.find(delim,pos1+1);
    string str1=a.substr(pos1,pos2-pos1),str2=a.substr(pos2);
    const char *num_str1=str1.c_str(),*num_str2=str2.c_str();
    b[0]=atoi(num_str1);
    b[1]=atoi(num_str2);

}
void string2vex(const string &a,vector<int> &b,const string &delim){
    string::size_type pos0=a.find(delim),pos1=a.find(delim,pos0+1)+1,pos2=a.find(delim,pos1+1);
    string str1=a.substr(pos1,pos2-pos1);
    string str2=a.substr(pos2);

    const char *num_str1=str1.c_str(),*num_str2=str2.c_str();
    b.push_back(atoi(num_str1));
    b.push_back(atoi(num_str2));
    cout<<atoi(num_str1)<<" "<<atoi(num_str2)<<endl;

}
const  long long MAX_ITER=1000000;
const  long long tabu_iter=100;
class HEA{
private:
    int **graph;
    int **Sol;//解升维，p个种群，N个解
    int ***Population;//每个种群不同颜色数对应的点集合
    int ***pos;//种群中各顶点对应位置
    int *Best_Sol;//p个种群中最优的解
    int record[3];//选两个冲突最小的种群作为父母
    int *best_f;//各种群历史最小冲突数
    int *f;//各种群当前冲突数
    int ***f_table;//各种群冲突维护表
    int K;//颜色数
    int ***Adjacent_Color_Table;//邻接颜色表
    int N,e;
    long long iter2;
    long long   **TabuTenure;//禁忌表
    int p;//种群大小
    double sum_f=0;//记录所有种群总冲突数
    long long iter;//迭代步数

public:

    inline void get_init_info (int color_num,int population)
    {
        K=color_num;
        p=population;
    }

    //初始化各数据结构
    void Initialization(){
        //ifstream infile("DSJC250.1.col.txt",ios::in);
        ifstream infile("DSJC250.9.col.txt",ios::in);

        string delim=" ";
        string str;
        if(!infile.good())
        {
            cout<<"dataset file open error"<<endl;
            return;
        }
        vector<int> num;
        Population=new int **[p+1];
        pos=new int**[p+1];
        f=new int[p+1];
        f_table=new int **[p+1];
        best_f=new int[p+1];
        for(int i=0;i<p+1;i++)
        {
            f[i]=0;
            best_f[i]=0;
            Population[i]=new int *[K];
            pos[i]=new int *[K];

            for(int k=0;k<K;k++){
                Population[i][k] = new int[100];
                pos[i][k]=new int[500];
                for(int j=0;j<100;j++)
                    Population[i][k][j]=0;
                for(int j=0;j<500;j++)
                    pos[i][k][j]=0;
            }

            f_table[i]=new int*[2];
            f_table[i][0]=new int[1000];
            f_table[i][1]=new int[1000];

            for(int j=0;j<1000;j++)
            {
                f_table[i][0][j]=0;
                f_table[i][1][j]=0;
            }
        }



        while(!infile.fail()){
            getline(infile,str);

            if(str.find("p")==0){

                string2vex(str,num,delim);
                N=num[0];
                e=num[1];

                //内存分配
                Adjacent_Color_Table=new int **[p+1];
                for(int i=0;i<p+1;i++){
                    Adjacent_Color_Table[i]=new int*[N];
                    for(int j=0;j<N;j++){
                        Adjacent_Color_Table[i][j]=new int[K];
                        for(int k=0;k<K;k++)
                            Adjacent_Color_Table[i][j][k]=0;
                    }
                }

                graph=new int*[N];
                TabuTenure=new long long*[N];

                //数值初始化
                for(int i=0;i<N;i++){

                    TabuTenure[i]=new long long[K];
                    graph[i]=new int[N];

                    for(int j=0;j<K;j++) {
                        TabuTenure[i][j] = 0;
                    }

                    for(int j=0;j<N;j++)
                        graph[i][j]=0;

                }
                //种群初始化
                Sol=new int *[p+1];
                Sol[0]=new int[N];
                for(int i=0;i<N;i++)
                    Sol[0][i]=0;

                for(int i=1;i<=p;i++)
                {
                    Sol[i]=new int[N];
                    for(int j=0;j<N;j++)
                    {
                        Sol[i][j]=rand()%K;//每个种群初始解
                        int k=Sol[i][j];
                        //种群初始化
                        int count=++Population[i][k][0];
                        Population[i][k][count]=j;
                        pos[i][k][j]=count;
                    }
                }


            }
            else if(str.find("e")==0){
                string2arc(str,num,delim);
                int first=num[0]-1,next=num[1]-1;//对应数组下标

                //邻接颜色表统计+图数据+冲突表初始化
                graph[first][0]++;
                graph[next][0]++;
                graph[first][graph[first][0]]=next;
                graph[next][graph[next][0]]=first;

                for(int i=1;i<=p;i++){
                    int first_color=Sol[i][first],next_color=Sol[i][next];
                    Adjacent_Color_Table[i][first][next_color]++;
                    Adjacent_Color_Table[i][next][first_color]++;

                    if(Sol[i][first]==Sol[i][next])//冲突数记录
                    {
                        f[i]++;
                        if(!f_table[i][1][first])
                        {
                            f_table[i][0][0]++;
                            int count=f_table[i][0][0];
                            f_table[i][0][count]=first;
                            f_table[i][1][first]=count;
                        }
                        if(!f_table[i][1][next]){
                            f_table[i][0][0]++;
                            int count=f_table[i][0][0];
                            f_table[i][0][count]=next;
                            f_table[i][1][next]=count;
                        }
                    }
                }
            }

        }
        for(int i=1;i<=p;i++)
            best_f[i]=f[i];
        infile.close();
    }


    void FindMove(int pop_num,int &u,int &vi,int &vj,int &delt) {
        delt = 10000;
        int delt1 = 10000;
        int count = 0, tabu_count = 0;
        int equ_delt[2000][2]={0};//非禁忌相同delt值
        int equ_tabudelt[2000][2]={0};//禁忌相同delt值
        int **cur_color_table = Adjacent_Color_Table[pop_num];


        for(int m=1;m<=f_table[pop_num][0][0];m++){
            int i=f_table[pop_num][0][m];
            int old_color=Sol[pop_num][i];
            for (int k = 0; k < K; k++) {//遍历颜色搜索冲突min

                if (k ==old_color) continue;

                int tmp_delt = cur_color_table[i][k] - cur_color_table[i][old_color];//计算DELT

                if (tmp_delt > delt)
                    continue;

                //非禁忌移动
                if (TabuTenure[i][k] < iter) {
                    if (tmp_delt < delt) {
                        count = 0;
                        delt = tmp_delt;
                    }
                    count++;
                    equ_delt[count - 1][0] = i;
                    equ_delt[count - 1][1] = k;

                }
                    //禁忌移动
                else {
                    if (tmp_delt <= delt1) {
                        if (tmp_delt < delt1) {
                            delt1 = tmp_delt;
                            tabu_count = 0;
                        }
                        tabu_count++;
                        equ_tabudelt[tabu_count - 1][0] = i;
                        equ_tabudelt[tabu_count - 1][1] = k;
                    }

                }
            }//结束当前节点的颜色判断
        }//结束统计比较

        //最终判断
        int tmp = 0;
        if (f[pop_num]+ delt1 < best_f[pop_num]) {
            delt = delt1;
            tmp = rand() % tabu_count;//相等delt随机选择
            u = equ_tabudelt[tmp][0];
            vj = equ_tabudelt[tmp][1];

        } else {
            tmp = rand() % count;//相等delt随机选择
            u = equ_delt[tmp][0];
            vj = equ_delt[tmp][1];

        }
        vi = Sol[pop_num][u];
    }

    void MakeMove(int pop_num,int &u,int &vi,int &vj, int &delt){
        Sol[pop_num][u]=vj;
        int node_pos=pos[pop_num][vi][u],count=Population[pop_num][vi][0],final=Population[pop_num][vi][count];
        //原颜色集合中删除
        Population[pop_num][vi][node_pos]=final;
        pos[pop_num][vi][u]=0;
        pos[pop_num][vi][final]=node_pos;
        //数量变化
        Population[pop_num][vi][0]--;

        //新颜色集合中增加
        Population[pop_num][vj][0]++;
        int new_pos=Population[pop_num][vj][0];
        pos[pop_num][vj][u]=new_pos;
        Population[pop_num][vj][new_pos]=u;

        f[pop_num]=f[pop_num]+delt;
        if(best_f[pop_num]>f[pop_num])
            best_f[pop_num]=f[pop_num];

        TabuTenure[u][vi]=iter+f[pop_num]+rand()%10;

        int **pop_f_table=f_table[pop_num];
        int **cur_adj_table=Adjacent_Color_Table[pop_num];
        //邻接颜色表+冲突表更新
        for(int m=1;m<=graph[u][0];m++){
            int i=graph[u][m];
            int tmp=Sol[pop_num][i];

            cur_adj_table[i][vi]--;
            cur_adj_table[i][vj]++;

            if(vi==tmp&&!cur_adj_table[i][tmp]){//若修改颜色后冲突消除,在冲突表中删除
                int pos=pop_f_table[1][i],count=pop_f_table[0][0],final=pop_f_table[0][count];
                pop_f_table[0][pos]=pop_f_table[0][count];//最后一个往前丢
                pop_f_table[1][final]=pos;//修改最后一个节点位置
                pop_f_table[1][i]=0;//位置清空
                pop_f_table[0][0]--;//数量减少
            }
            else if(vj==tmp&&!pop_f_table[1][i]){//修改颜色后产生新冲突
                pop_f_table[0][0]++;
                int pos=pop_f_table[0][0];
                pop_f_table[0][pos]=i;//末尾插入
                pop_f_table[1][i]=pos;//记录位置
                //cout<<i<<endl;
            }
        }

        if(pop_f_table[1][u]&&!cur_adj_table[u][Sol[pop_num][u]]) {//变色后若冲突消除
            int pos = pop_f_table[1][u], count = pop_f_table[0][0], final = pop_f_table[0][count];
            pop_f_table[0][pos] = pop_f_table[0][count];
            pop_f_table[1][final] = pos;//修改最后一个节点位置
            pop_f_table[1][u] = 0;
            pop_f_table[0][0]--;
        }

    }
    void tabu_search(int pop_num){
        int u,vi,vj;
        int delt;
        int tmp;
        iter=0;
        while(iter<tabu_iter&&f[pop_num]>0){
            iter++;
            FindMove(pop_num,u,vi,vj,delt);
            MakeMove(pop_num,u,vi,vj,delt);
            //cout<<iter<<" "<<delt<<" "<<f[pop_num]<<endl;
        }
    }

    void init_population(){
        //数据初始化
        Initialization();
        //用TS优化种群初始值
        for(int i=1;i<=p;i++)
        {
            tabu_search(i);//禁忌搜索算法对种群预处理
            for(int i=0;i<N;i++)
                for(int k=0;k<K;k++){
                    TabuTenure[i][k]=0;
                }
        }

        int min_f=10000,max_f=-10000,second_f=-10000;
        for(int i=1;i<=p;i++){
            if(f[i]<min_f)
            {
                min_f=f[i];
                record[0]=i;//最优冲突种群
            }

            if(f[i]>max_f)
            {
                max_f=f[i];
                record[1]=i;//最差冲突种群
            }
            else if(f[i]>second_f)
            {
                second_f=f[i];
                record[2]=i;//次最差
            }
            //sum_f+=f[i];
        }
        Best_Sol=Sol[record[0]];
    }

    void Crossover_Operator(int S1,int S2){
        //要改要改
        int tmp[501]={0};//记录被删除的顶点
        int node;

        for(int l=0;l<K;l++){
            int choice=0,not_choice=0;
            if(l%2) {
                choice=S1;
                not_choice=S2;
            }
            else  {
                choice=S2;
                not_choice=S1;
            }
            //选择继承父/母
            int **V=Population[choice];
            int Vi=-1;
            int max_size=0;

            //选择最大基数的颜色数,可优化
            for(int i=0;i<K;i++)
            {
                if(V[i][0]>=max_size){
                    Vi=i;
                    max_size=V[i][0];
                }
            }

            //遗传操作
            Population[0][l][0]=V[Vi][0];//数目复制
            for(int i=1;i<=V[Vi][0];i++){
                Population[0][l][i]=V[Vi][i];
                node=V[Vi][i];
                pos[0][l][node]=i;
                //解决方案更新
                Sol[0][node]=l;
            }

            //删除操作
            int num=tmp[0];
            for(int i=1;i<=V[Vi][0];i++)
            {
                int n=V[Vi][i];//Vi:颜色，n:顶点
                tmp[i+num]=n;
                int **the_other=Population[not_choice];

                int k=Sol[not_choice][n];
                int node_pos=pos[not_choice][k][n],count=the_other[k][0],final=the_other[k][count];
                //在另一父母中删除
                the_other[k][node_pos]=final;
                pos[not_choice][k][final]=node_pos;
                the_other[k][0]--;
            }
            tmp[0]+=V[Vi][0];
            //清空此颜色集
            V[Vi][0]=0;
        }//结束交叉操作

        //剩余节点随机分配给子代
        for(int k=0;k<K;k++){
            int l=rand()%K;
            if(Population[S1][k][0])
            {
                int num=Population[0][l][0];
                Population[0][l][0]+=Population[S1][k][0];
                for(int i=1;i<=Population[S1][k][0];i++)
                {
                    Population[0][l][i+num]=Population[S1][k][i];
                    node=Population[S1][k][i];
                    pos[0][l][node]=i+num;
                    Sol[0][node]=l;
                }
            }
        }

        int size1,size2;
        int k1,k2;
        //删除节点随机分配新颜色
        for(int i=1;i<=tmp[0];i++)
        {
            k1=rand()%K,k2=rand()%K;
            node=tmp[i];

            //更新种群信息
            size1=++Population[S1][k1][0],size2=++Population[S2][k2][0];
            pos[S1][k1][node]=size1;
            pos[S2][k2][node]=size2;
            Population[S1][k1][size1]=node;
            Population[S2][k2][size2]=node;

            //更新冲突相关信息
            if(k1!=Sol[S1][node])
                update_info(S1,node,k1);
            if(k2!=Sol[S2][node])
                update_info(S2,node,k2);
        }

        //最优节点变化
        if(S1==record[0]||S2==record[0]){
            int min_f=10000;
            for(int i=1;i<=p;i++)
            {
                if(f[i]<min_f)
                {
                    min_f=f[i];
                    record[0]=i;
                }
            }
        }

        //更新后代的邻接颜色表和冲突信息
        int **child_adj_table=Adjacent_Color_Table[0];
        int **child_f_table=f_table[0];
        for(int i=0;i<N;i++) {
            int k=Sol[0][i];
            //邻接表影响更新
            for (int m = 1; m <= graph[i][0]; m++) {
                int adj_node = graph[i][m];
                child_adj_table[adj_node][k]++;
                if (k== Sol[0][adj_node])
                {
                    f[0]++;
                    if(!child_f_table[1][i])//冲突表中不存在，则添加此节点
                    {
                        child_f_table[0][0]++;
                        child_f_table[0][child_f_table[0][0]]=i;
                        child_f_table[1][i]=child_f_table[0][0];
                    }
                }
            }
        }

        f[0]/=2;
        best_f[0]=f[0];
    }

    //根据POPULATION信息更新邻接颜色表和冲突数
    void update_info(int pop_num,int node,int k){
        int **pop_f_table=f_table[pop_num];

        int old_color=Sol[pop_num][node];
        //解和冲突数更新
        Sol[pop_num][node]=k;
        f[pop_num]+=Adjacent_Color_Table[pop_num][node][k]-Adjacent_Color_Table[pop_num][node][old_color];
        if(f[pop_num]<0)
            cin>>old_color;
        //邻接节点更新颜色表
        for(int j=1;j<=graph[node][0];j++)
        {
            int adj_node=graph[node][j];
            Adjacent_Color_Table[pop_num][adj_node][old_color]--;
            Adjacent_Color_Table[pop_num][adj_node][k]++;
        }
    }


    //按冲突数计算概率，选择父母
//    void choose_parents(int &S1,int &S2){
//        double fitness[p+1];
//        for(int i=0;i<=p;i++)
//            fitness[i]=(double)f[i]/sum_f;
//        //S1=rand()%p;
//
//

    //更新后代
    void update_population(){
        //由于只更新了父母、孩子，故只考虑两解与原始解大小
//        int worst_sol[4]={S1,S2,record[1],record[2]};//采用冒泡排序对四个数进行比较
//        int tmp;
//        int count=3;
//
//        for(int i=0;i<3;i++){
//            for(int j=0;j<3-i;j++)
//            {
//                if(f[worst_sol[j]]<f[worst_sol[j+1]]){
//                    tmp=worst_sol[j];
//                    worst_sol[j]=worst_sol[j+1];
//                    worst_sol[j+1]=tmp;
//                }
//            }
//        }
        //最差解和最优解更新
        int max_f=0;
        for(int i=1;i<=p;i++){
            if(f[i]>=max_f)
            {
                max_f=f[i];
                record[1]=i;
            }
        }

        int worst_sol=record[1];

        //子代数据复制，替代最差解所在位置
        for(int k=0;k<K;k++){
            //数目复制
            Population[worst_sol][k][0]=Population[0][k][0];
            for(int j=1;j<=Population[0][k][0];j++){
                int  node=Population[0][k][j];
                Population[worst_sol][k][j]=node;
                pos[worst_sol][k][node]=j;
                Sol[worst_sol][node]=Sol[0][node];
            }
        }

        for(int i=0;i<N;i++){
            for(int k=0;k<K;k++)
                Adjacent_Color_Table[worst_sol][i][k]=Adjacent_Color_Table[0][i][k];
        }

//        f_table[worst_sol][0][0]=f_table[0][0][0];
//        for(int i=1;i<=f_table[0][0][0];i++) {
//            int node = f_table[0][0][0];
//            f_table[worst_sol][0][i] = node;
//            f_table[worst_sol][1][node] = i;
//        }

        f[worst_sol]=f[0];
//        best_f[worst_sol]=f[0];

        //若子代是最优解，最优解位置被最差解替代
        if(f[0]<f[record[0]]){
            record[0]=worst_sol;
            Best_Sol=Sol[worst_sol];
        }

        //子代tmp节点信息清空
        for(int i=0;i<N;i++){
            for(int k=0;k<K;k++){
                Adjacent_Color_Table[0][i][k]=0;
            }
            Sol[0][i]=-1;
        }
        f[0]=0;
        best_f[0]=0;

        //子代冲突表清空
        int **child_f_table=f_table[0];
        int length=child_f_table[0][0];
        child_f_table[0][0]=0;
        for(int i=1;i<=length;i++){
            int node=child_f_table[0][i];
            child_f_table[1][node]=0;
            child_f_table[0][i]=0;
        }
        for(int i=0;i<N;i++)
            for(int k=0;k<K;k++)
                TabuTenure[i][k]=0;
    }

    void run(){
        ofstream ofile("test.txt");
        if(!ofile.good())
        {
            cout<<"file open error"<<endl;
            return;
        }
        srand(clock());
        ofile<<"随机种子"<<clock()<<endl;
        init_population();

        //开始进化
        iter2=0;
        double  start_time,end_time,elapsed_time;
        start_time=clock();

        while(iter2<MAX_ITER&&f[record[0]]>0){
            int S1=rand()%p+1,S2=rand()%p+1;//随机选择两个父母
            while(S1==S2){
                S1=rand()%p+1;
            }
            Crossover_Operator(S1,S2);//交叉得最大集合

            tabu_search(0);//子代通过禁忌搜索改良

            //最差解的替换和信息更新
            update_population();
            iter2++;
            // cout<<iter2<<" "<<f[record[0]]<<endl;
        }

        end_time=clock();
        elapsed_time=(end_time - start_time)/CLOCKS_PER_SEC;

        ofile<<"颜色数为"<<K<<"的图着色解如下："<<endl;
        for(int i=0;i<N;i++)
            ofile<<i+1<<" "<<Best_Sol[i]<<endl;
        ofile<<"时间"<<elapsed_time<<endl;
        ofile<<"迭代次数"<<iter2<<endl;
        cout<<"检查结果如下"<<endl;
        for(int i=0;i<N;i++){
            cout<<i<<" ";
            for(int j=1;j<=graph[i][0];j++){
                int node=graph[i][j];
                if(Best_Sol[i]==Best_Sol[node])
                    cout<<"error"<<" ";
                else
                    cout<<node<<" ";
            }
            cout<<endl;
        }

        ofile.close();

    }

};


int main() {
    int K,p;
    cout<<"输入颜色数：";
    cin>>K;
    cout<<"输入种群大小：";
    cin>>p;
    HEA test;
    test.get_init_info(K,p);
    test.run();
    return 0;
}

