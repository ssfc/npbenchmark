cmake_minimum_required(VERSION 3.24)
project(tabucol_vector)

set(CMAKE_CXX_STANDARD 17)

add_executable(tabucol_vector main.cpp GraphColoring.cpp GraphColoring.h tabucol.cpp tabucol.h overflow.cpp)
