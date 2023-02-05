cmake_minimum_required(VERSION 3.24)
project(GCP_HEA_dynamic)

set(CMAKE_CXX_STANDARD 17)

add_executable(GCP_HEA_dynamic main.cpp hea.h hea.cpp GraphColoring.h GraphColoring.cpp)
