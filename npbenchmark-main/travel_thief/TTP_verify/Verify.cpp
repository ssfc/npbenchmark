//
// Created by Star on 2023/6/13.
//

#include "Verify.h"

//#define BUG

int n, m;
int C;
double maxv, minv, rent_r;

CITY cities[N];
ITEM items[M];
bool st[N], take_item[M];

vector<int> path;
vector<int> choose_items;
map<PII, double> dis_table;

int main(int argc, char* argv[])
{
//    freopen("../LOG/verify_err.log", "w", stderr);
    // usage: ./verify [ttpfile] [solvefile]
    ifstream ifs1(argv[1]);
    ifstream ifs2(argv[2]);
    read_data(ifs1);

//    for (int i = 1; i <= 10; i ++) {
//        fprintf(stderr, "%d, %d %d %d\n", i, items[i].val, items[i].weight, items[i].pos);
//    }

    read_solve(ifs2);

    pre_cal_dis();
    double ans = cal_val();
    printf("ans: %lf\n", ans);
    return 0;
}


void read_data(istream &fin)
{
    fin >> C;       // 背包容量
    fin >> minv;    // 最小速度
    fin >> maxv;    // 最大速度
    fin >> rent_r;  // 租金

    fin >> n;   // 城市数量
    for (int i = 1; i <= n; i++)
    {
        int id, a, b;
        fin >> id >> a >> b;
        cities[id] = {a, b};
    }

    fin >> m;   // 物品数量
    for (int i = 1; i <= m; i++)
    {
        int id, val, weight, pos;
        fin >> id >> val >> weight >> pos;
        items[id] = {id, val, weight, pos};
        cities[pos].item_list.push_back(id);
    }
}

void read_solve(istream &fin) {
    int id;
    for (int i = 1; i <= n; i ++) {
        fin >> id;
        path.push_back(id);
        if (!st[id]) {
            st[id] = true;
        }else {
            puts("TSP ERROR!");
            exit(1);
        }
    }


    int cur_w = 0;
    while (fin) {
        fin >> id;
        take_item[id] = true;
    }

}

double get_dis(CITY &a, CITY &b)
{
    return sqrt((double)(a.x - b.x) * (a.x - b.x) + (double)(a.y - b.y) * (a.y - b.y));
}

void pre_cal_dis()
{
    dis_table.clear();
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (dis_table[{i, j}])
                continue;
            dis_table[{i, j}] = get_dis(cities[i], cities[j]);
            dis_table[{j, i}] = get_dis(cities[i], cities[j]);
        }
    }
}

double cal_val()
{
    double cur_v = maxv;
    double cur_weight = 0;
    double cur_cost = 0, cur_val = 0;
    int cur_city_id = 1, next_city_id;
    for (int i = 0; i < path.size(); i++)
    {
        next_city_id = path[i];
        CITY &next_city = cities[next_city_id];
        CITY &cur_city = cities[cur_city_id];

        for (auto item_id : cur_city.item_list)
        {
            if (take_item[item_id] == true)
            {
                ITEM &t = items[item_id];
                cur_weight += t.weight;
                cur_val += t.val;
                if (cur_weight > C)
                {
                    puts("KP ERROR!");
                    exit(1);
                }
#ifdef BUG
                fprintf(stderr, "%d\tv: %d, w: %d\n", item_id, t.val, t.weight);
#endif
            }
        }
        double weight_rate = cur_weight / C;
        cur_v = maxv - ((maxv - minv) * weight_rate);
        double distance = get_dis(cur_city, next_city);
        cur_cost += distance / cur_v;

#ifdef BUG
        fprintf(stderr, "%d->%d[%.3lf]:\t\tcur_val: %lf, cur_weight: %lf, weight_rate: %lf, cur_v: %lf, cost_time: %lf, sum_time: %lf\n",
                cur_city_id, next_city_id, distance, cur_val, cur_weight, weight_rate, cur_v, distance / cur_v, cur_cost);
#endif
        cur_city_id = next_city_id;
    }
    return cur_val - cur_cost * rent_r;
}