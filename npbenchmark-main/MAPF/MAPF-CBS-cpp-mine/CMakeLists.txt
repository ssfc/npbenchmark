cmake_minimum_required(VERSION 3.25)
project(a_star)

set(CMAKE_CXX_STANDARD 17)

add_executable(a_star main.cpp a_star.cpp a_star.h
        cbs.h
        cbs.cpp
        util.h)
