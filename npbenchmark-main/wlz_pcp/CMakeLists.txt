cmake_minimum_required(VERSION 3.24)
project(wlz_pcp)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
set(Boost_LIBRARY_DIRS C:/boost_1_81_0/libs)
set(Boost_INCLUDE_DIR C:/boost_1_81_0)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

find_package(Boost REQUIRED)
include_directories(C:/boost_1_81_0)

add_executable(wlz_pcp main.cpp vwts.h vwts.cpp PCenter.h PCenter.cpp)
