cmake_minimum_required(VERSION 3.23)
project(SDK_GCP)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

include_directories(.)

add_executable(SDK_GCP
        GraphColoring.cpp
        GraphColoring.h
        Main.cpp)
