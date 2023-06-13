#!/bin/bash
# get all filename in specified path

instance_path="../instances/data_ex/"   # 样例目录
solve_path="../res/res_TTP/"                # 解文件目录

files=$(ls $instance_path)  # 样例名称

for filename in $files
do
#    echo ./verify $instance_path$filename $solve_path$filename.txt   # 打印要执行的命令
    ./verify $instance_path$filename $solve_path$filename.txt
done

# Usage： 更改 “instance_path” 和 "solve_path"，在对应目录下运行脚本： sh multi_verify.sh

