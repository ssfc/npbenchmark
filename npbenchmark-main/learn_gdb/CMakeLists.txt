cmake_minimum_required(VERSION 3.24)
project(learn_gdb)

set(CMAKE_CXX_STANDARD 17)

add_executable(learn_gdb learn_gdb.cpp test_error.cpp debug_running.cpp)
