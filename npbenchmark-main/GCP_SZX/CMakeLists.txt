cmake_minimum_required(VERSION 3.24)
project(GCP_SZX)

set(CMAKE_CXX_STANDARD 17)

add_executable(GCP_SZX main.cpp solver.h solver.cpp GraphColoring.h GraphColoring.cpp)
