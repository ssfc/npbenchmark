cmake_minimum_required(VERSION 3.25)
project(c_and_cpp C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

add_executable(c_and_cpp main.cpp hello_world.c)
