cmake_minimum_required(VERSION 3.23)
project(SDK_PCP)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

include_directories(.)

add_executable(SDK_PCP
        Main.cpp
        PCenter.cpp
        PCenter.h)
