1. 如何确定一系列从同一个仓库出发的车辆的行驶路径, 在每辆车均不超载且每个客户都在给定时间窗内被访问的前提下, 最小化所有车辆的行驶时间之和. (2023年3月11日)

## 问题概述
1. 给定一个有向完全图G, 图中包含一个仓库节点c和若干客户节点v. 每个客户节点都对应一个配送请求, 需要一个在指定的时间窗内(Ti1, Ti2)将给定重量wi的物品一次性送达, 交接过程花费固定的时间ti3. 此外, 每辆车的最大载重量相同Wmax且均需从仓库出发最后回到仓库, 同时任意两点间的最短行驶时间已知 tij(超时视为到达节点后原地等待). 请确定使用几辆车(P)参与配送, 并给出每辆车依次访问的有序节点列表, 使得所有车辆的总行驶时间(tv1+tv2+tv3)最短.

## 命令行参数
1. 和以前一样; 

## 输入的算例文件格式
4(节点数 N) 3(最大可用车辆数 K) 10(各车辆的载重量上限 Q)  
节点0坐标(1, 5) 物品重量0 最短停留时间0 时间窗(0, 8)  
节点1坐标(2, 1) 物品重量6 最短停留时间3 时间窗(4, 5)  
节点2坐标(3, 7) 物品重量2 最短停留时间4 时间窗(5, 6)  
节点3坐标(8, 8) 物品重量9 最短停留时间5 时间窗(6, 7)  

## 输出的解文件格式
V 行整数表示 V 辆车 (V ≤ K) 的行驶路径
3: 车辆 0 从仓库 0 出发后依次经过节点 3, 最后返回;
1 2: 车辆 1 从仓库 0 出发后依次经过节点 1 和 2, 最后返回; 


## Other work
1. 
2. adzen(c++): https://github.com/adzen/VRPTW
3. 
4. 
5. zhou hang (周航): https://github.com/zll-hust
6. 他(华科管院的)的主页有好多VRPTW的仓库; 
7. 禁忌算法VRPTW (java): https://github.com/zll-hust/Tabu_VRPTW
8. 蚁群算法ACS_VRPTW (java): https://github.com/zll-hust/ACS_VRPTW
9. CSDN: https://blog.csdn.net/zll_hust/article/details/104167809
10. 遗传算法VRPTW (c++): https://github.com/zll-hust/GA_VRPTW
11. 标号法VRPTW (c++): https://github.com/zll-hust/LabelSetingAlgorithmForSPPTW
12. ALNS VRPTW (java): https://github.com/zll-hust/ALNS_VRPTW
13. 列生成+pulse algorithm求解VRPTW (java): https://github.com/zll-hust/CG-VRPTW 
14. 2E-VRPTWSPD (java): https://github.com/zll-hust/2E-VRPTWSPD
15. 
16. 

C101: 
SU' data first 3 lines;
101 25 200
(x)40 (y)50 (demand)0  (last)0 (ready)0 (due)1236
(x)45 (y)68 (demand)10 (last)90 (ready)912 (due)967

py-ga's data first 3 lines;
C101
VEHICLE
NUMBER     CAPACITY
  25         200
CUSTOMER
CUST NO.  XCOORD.   YCOORD.    DEMAND   READY TIME  DUE DATE   SERVICE   TIME

    0   (x)40  (y)50  (demand)0     (ready)0      (due)1236    (last)0   
    1   (x)45  (y)68  (demand)10    (ready)912    (due)967     (last)90   
    2      45         70         30        825        870         90   




