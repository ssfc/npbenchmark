cmake_minimum_required(VERSION 3.22)
project(ConflictBasedSearch)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(ConflictBasedSearch
        HighLevelSolver.cpp
        HighLevelSolver.h
        LowLevelSolver.cpp
        LowLevelSolver.h
        main.cpp
        TreeNode.cpp
        TreeNode.h
        util.h)
