#include <iostream>
#include <climits>
#include <ctime>
#include <string>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <ostream>

using namespace std;

const int MaxIter=100000;
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

class tabu_search{
public:
    int *Sol;//节点对应颜色值
    int f;//冲突值
    //  int *BestSol;//最优解
    int best_f;//历史最优冲突值
    long long **TabuTenure;//禁忌表
    int **Adjacent_Color_Table;//邻接颜色表
    int N,e;//点数，边数
    int K;//颜色数
    int neibor_bucket[9][10000][2]={0};//存放各邻域动作的桶
    int ****Pos;//辅助桶排序数据结构
    int **graph;//图--记录邻接边,方便更新邻接颜色表
    long long iter;//步数记录
    int tabu_best_move[2];//记录当前禁忌表中BEST_MOVE
    int non_tabu_best_move[2];//记录当前非禁忌表中BEST_MOVE

    void put_color_num(int color_num){
        K=color_num;
    }
//初始化各数据结构
    void Initialization(){
        ifstream infile("C:\\wamp64\\www\\npbenchmark\\npbenchmark-main\\SDK.GCP\\joker\\DSJC125.1.col.txt",ios::in); // 44 colors;
        // ifstream infile("C:\\wamp64\\www\\npbenchmark\\npbenchmark-main\\SDK.GCP\\joker\\DSJC250.9.col.txt",ios::in); // 72 colors;
        // ifstream infile("C:\\wamp64\\www\\npbenchmark\\npbenchmark-main\\SDK.GCP\\joker\\DSJC500.5.col.txt",ios::in); // 72 colors;
        string delim=" ";
        string str;
        int tmp_delt;//桶初始化
        if(!infile.good())
        {
            cout<<"file open error"<<endl;
            return;
        }
        vector<int> num;

        f=0;

        while(!infile.fail()){
            getline(infile,str);

            if(str.find("p")==0){

                string2vex(str,num,delim);
                N=num[0];
                e=num[1];

                //内存分配
                Sol=new int[N];
                Adjacent_Color_Table=new int *[N];
                graph=new int*[N];
                TabuTenure=new long long*[N];
                Pos=new int ***[N];


                //数值初始化
                for(int i=0;i<N;i++){
                    Adjacent_Color_Table[i]=new int[K];
                    TabuTenure[i]=new long long[K];
                    graph[i]=new int[N];
                    Pos[i]=new int **[K];

                    for(int k=0;k<K;k++) {
                        Adjacent_Color_Table[i][k] = 0;
                        TabuTenure[i][k] = 0;
                        Pos[i][k]=new int *[K+1];
                        for(int j=0;j<K+1;j++)
                        {
                            Pos[i][k][j]=new int[2];
                            Pos[i][k][j][0]=0;
                            Pos[i][k][j][1]=0;
                        }
                    }

                    graph[i][0]=0;
                    for(int j=1;j<N;j++)
                        graph[i][j]=-1;

                }

                for(int i=0;i<N;i++){
                    Sol[i]=rand()%K;//初始解
                }

            }
            else if(str.find("e")==0){
                string2arc(str,num,delim);
                int first=num[0]-1,next=num[1]-1;//对应数组下标

                //邻接颜色表统计+图数据初始化
                graph[first][0]++;
                graph[next][0]++;
                graph[first][graph[first][0]]=next;
                graph[next][graph[next][0]]=first;

                Adjacent_Color_Table[first][Sol[next]]++;
                Adjacent_Color_Table[next][Sol[first]]++;

                if(Sol[first]==Sol[next])//存在冲突，加入邻域动作
                {
                    f++;
                    for(int k=0;k<K;k++){
                        if(k!=Sol[first]){
                            tmp_delt=Adjacent_Color_Table[first][k]-Adjacent_Color_Table[first][Sol[first]];
                            put_buc(tmp_delt,k,first);
                            Pos[first][Sol[first]][K][1]=true;//新增冲突标志位
                        }
                        if(k!=Sol[next]){
                            tmp_delt=Adjacent_Color_Table[next][k]-Adjacent_Color_Table[next][Sol[next]];
                            put_buc(tmp_delt,k,next);
                            Pos[next][Sol[next]][K][1]=true;//新增冲突标志位
                        }
                    }
                }
            }

        }
        best_f=f;
        infile.close();

    }

    void FindMove(int &u,int &vi,int &vj,int &delt){
        delt=INT_MAX;
        int delt1=INT_MAX;

        int i,pos,new_color,j,count,final_node,final_color;
        int tabu_count;
        bool tabu=false;

        for(j=0;j<9;j++){
            tabu_count=0;
            bool in=true;
            count=neibor_bucket[j][0][0];
            while(in&&count){

                pos=rand()%count+1;//随机选择动作
                i=neibor_bucket[j][pos][0];
                new_color=neibor_bucket[j][pos][1];

                if(Pos[i][Sol[i]][new_color][1])
                {
                    if(TabuTenure[i][new_color]<iter){
                        delt=Adjacent_Color_Table[i][new_color]-Adjacent_Color_Table[i][Sol[i]];
                        in=false;
                    }
                    else{
                        delt1=Adjacent_Color_Table[i][new_color]-Adjacent_Color_Table[i][Sol[i]];
                        tabu_count++;
                        if(f+delt1<best_f)
                        {
                            in=false;
                            tabu=true;
                        }
                    }
                }
                else//此动作不存在，则进行删除
                {
                    final_node=neibor_bucket[j][count][0];
                    final_color=neibor_bucket[j][count][1];
                    neibor_bucket[j][pos][0]=final_node;
                    neibor_bucket[j][pos][1]=final_color;
                    count=--neibor_bucket[j][0][0];
                    Pos[final_node][Sol[final_node]][final_color][1]=pos;
                }

                if(!count||tabu_count==count)
                    in=false;
            }

            if(count&&tabu_count!=count)
                break;
        }


        //最终判断
        if(tabu)
        {
            delt=delt1;
            u=i;
            vj=new_color;
        }
        else{
            u=i;
            vj=new_color;
        }
        int tag;
        if(delt==10000)
            cin>>tag;

        vi=Sol[u];
    }
    void MakeMove(int &u,int &vi,int &vj, int &delt){
        Sol[u]=vj;
        f=f+delt;
        if(best_f>f)
            best_f=f;
        //更新禁忌表
        TabuTenure[u][vi]=iter+f+rand()%10;

        int tmp,tmp_delt;
        int i;
        int pos,final,count;
        bool tag;
        //邻接颜色表和桶更新
        for(int m=1;m<=graph[u][0];m++) {
            i = graph[u][m];
            tmp = Sol[i];
            Adjacent_Color_Table[i][vi]--;
            Adjacent_Color_Table[i][vj]++;
            tag=Pos[i][tmp][K][1];

            if (!Adjacent_Color_Table[i][tmp]&&tag) {//冲突消除，标志
                for(int k=0;k<K;k++){
                    Pos[i][tmp][k][1]=0;
                }

                Pos[i][tmp][K][1]=false;
            }
            else if (vj == tmp && !tag) {//新增加冲突，邻域动作更新
                for (int k = 0; k < K; k++) {
                    if (k == tmp) continue;
                    tmp_delt = Adjacent_Color_Table[i][k] - Adjacent_Color_Table[i][tmp];
                    put_buc(tmp_delt,k,i);
                }
                Pos[i][tmp][K][1]=true;
            }
            else if (tag) {//仍为冲突节点，考虑DELT变化的动作加入桶
                if (vi == Sol[i]) {//该节点在老颜色中
                    tmp_delt= Adjacent_Color_Table[i][vj] - Adjacent_Color_Table[i][vi] + 2;//移到新颜色delt+2
                    put_buc(tmp_delt,vj,i);

                    for (int k = 0; k < K; k++) {//移到其他颜色DELT+1
                        if (k == vi || k == vj) continue;

                        tmp_delt=Adjacent_Color_Table[i][k] - Adjacent_Color_Table[i][vi] + 1;
                        Pos[i][vi][k][1]=0;
                        put_buc(tmp_delt,k,i);
                    }
                }
                else if (vj == Sol[i]) {//该节点在新颜色中
                    tmp_delt= Adjacent_Color_Table[i][vi] - Adjacent_Color_Table[i][vj] - 2;//移到老颜色DELT-2
                    Pos[i][vj][vi][1]=0;
                    put_buc(tmp_delt,vi,i);
                    for (int k = 0; k < K; k++) {
                        if (k == vi || k == vj) continue;

                        tmp_delt= Adjacent_Color_Table[i][k] - Adjacent_Color_Table[i][vj] - 1;//移到其他颜色DELT-1
                        Pos[i][vj][k][1]=0;
                        put_buc(tmp_delt,k,i);
                    }
                }
                else {
                    //其他颜色移到老颜色中
                    tmp_delt= Adjacent_Color_Table[i][vi] - Adjacent_Color_Table[i][Sol[i]] - 1;
                    Pos[i][Sol[i]][vi][1]=0;
                    put_buc(tmp_delt,vi,i);
                    //其他颜色移到新颜色中
                    tmp_delt= Adjacent_Color_Table[i][vj] - Adjacent_Color_Table[i][Sol[i]] + 1;
                    Pos[i][Sol[i]][vj][1]=0;
                    put_buc(tmp_delt,vj,i);
                }
            }
        }

        //若节点本身不再有冲突，邻域动作应删除
        if(!Adjacent_Color_Table[u][vj]&&Pos[u][vi][vj][1])
            for(int k=0;k<k;k++)
                Pos[u][vj][k][1]=0;

    }
    inline void put_buc(int tmp_delt,int new_color,int node) {
        int buc_num,count;
        int old_color=Sol[node];

        if (tmp_delt <= -4)
            buc_num = 0;
        else if (tmp_delt >= 4)
            buc_num = 8;
        else {
            switch (tmp_delt) {
                case -3:
                    buc_num = 1;
                    break;
                case -2:
                    buc_num = 2;
                    break;
                case -1:
                    buc_num = 3;
                    break;
                case 0:
                    buc_num = 4;
                    break;
                case 1:
                    buc_num = 5;
                    break;
                case 2:
                    buc_num = 6;
                    break;
                case 3:
                    buc_num = 7;
                    break;
            }
        }

        int i,color;
        //插到第一个非空位置
        if(neibor_bucket[buc_num][0][0]>=N*K){
            while(true){
                count=rand()%neibor_bucket[buc_num][0][0]+1;//随机选择动作
                i=neibor_bucket[buc_num][count][0];
                color=neibor_bucket[buc_num][count][1];

                if(!Pos[i][Sol[i]][color][1])//空位插入
                    break;
            }
        }
        else{//插到末尾
            count=++neibor_bucket[buc_num][0][0];
        }

        neibor_bucket[buc_num][count][0] = node;
        neibor_bucket[buc_num][count][1] = new_color;
        Pos[node][old_color][new_color][0] = buc_num;
        Pos[node][old_color][new_color][1]=count;

    }
    void run(){
        iter=0;
        ofstream ofile("C:\\wamp64\\www\\npbenchmark\\npbenchmark-main\\SDK.GCP\\joker\\test.txt ", ios::out);
        if(!ofile.good())
        {
            cout<<"file open error"<<endl;
            return;
        }
        srand(clock());
        ofile<<"Random seed "<<clock()<<endl;
        Initialization();

        double  start_time,end_time,elapsed_time;
        start_time=clock();

        int u,vi,vj,delt;
        while(f>0){//冲突仍存在
            iter++;
            FindMove(u,vi,vj,delt);
            MakeMove(u,vi,vj,delt);
            if(iter%100000==0)
                cout<<best_f<<" "<<delt<<" "<<iter<<" "<<f<<endl;
        }
        end_time=clock();
        elapsed_time=(double(end_time - start_time))/CLOCKS_PER_SEC;

        cout<<elapsed_time<<endl;

        ofile<<"颜色数"<<K<<"解具体情况："<<endl;
        for(int i=0;i<N;i++)
            ofile<<i+1<<" "<<Sol[i]<<endl;
        ofile<<"时间"<<elapsed_time<<endl;
        ofile<<"迭代次数"<<iter<<endl;



        ofile.close();
    }

};
int main(){
    tabu_search init;
    int color_num;
    cout<<"Input num of color:";
    cin>>color_num;
    init.put_color_num(color_num);

    init.run();
}