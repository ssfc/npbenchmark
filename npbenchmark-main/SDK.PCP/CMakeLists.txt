cmake_minimum_required(VERSION 3.23)
project(SDK_PCP)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

include_directories(.)

add_executable(SDK_PCP
        Main.cpp
        PCenter.cpp
        PCenter.h pcp_greedy.h pcp_greedy.cpp pcp_local_search.h pcp_local_search.cpp pcp_vector.h pcp_vector.cpp)
