cmake_minimum_required(VERSION 3.5)
project(TS)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

add_executable(TS
        include/Graph.h
        include/Includes.h
        include/TabuSearch.h
        include/Solution.h
        main.cpp
        src/Graph.cpp
        src/TabuSearch.cpp
        src/Solution.cpp)
