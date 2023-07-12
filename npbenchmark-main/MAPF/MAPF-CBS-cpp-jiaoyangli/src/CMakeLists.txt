cmake_minimum_required(VERSION 3.25)
project(src)

set(CMAKE_CXX_STANDARD 14)

set(Boost_LIBRARY_DIRS C:/boost_1_81_0/libs)
set(Boost_INCLUDE_DIR C:/boost_1_81_0)

find_package(Boost REQUIRED)
include_directories(C:/boost_1_81_0)

add_executable(src
        agents_loader.cpp
        common.cpp
        compute_heuristic.cpp
        driver.cpp
        HLHeuristic.cpp
        ICBSNode.cpp
        ICBSSearch.cpp
        LLNode.cpp
        map_loader.cpp
        MDD.cpp
        RectangleReasoning.cpp
        SingleAgentICBS.cpp)
