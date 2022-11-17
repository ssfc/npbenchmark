cmake_minimum_required(VERSION 3.23)
project(yuanfang)

set(CMAKE_CXX_STANDARD 14)

add_executable(yuanfang ts_vs_add.cpp ts_vs_add.h cmake-build-debug/ts_vs.cpp cmake-build-debug/ts_vs.h)
