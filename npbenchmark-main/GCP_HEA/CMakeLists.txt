cmake_minimum_required(VERSION 3.24)
project(GCP_HEA)

set(CMAKE_CXX_STANDARD 17)

add_executable(GCP_HEA main.cpp GraphColoring.h GraphColoring.cpp hea.h hea.cpp)
