cmake_minimum_required(VERSION 3.24)
project(tabucol_GCP)

set(CMAKE_CXX_STANDARD 17)

add_executable(tabucol_GCP main.cpp GraphColoring.h GraphColoring.cpp tabucol.h tabucol.cpp)
