//
// Created by take_ on 2023/5/29.
//
// extern "C" void hello_world(); // 声明c文件中的函数原型
extern "C" void compute_tsp(char* parameter_file_name); // 声明c文件中的函数原型

int main()
{
    // hello_world();
    char input_parameter_file_name[] = "pr2392.par";
    compute_tsp(input_parameter_file_name);
    return 0;
}