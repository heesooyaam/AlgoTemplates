cmake_minimum_required(VERSION 3.28)
project(AlgoTemplates)

set(CMAKE_CXX_STANDARD 20)

include_directories(Graphs)

add_executable(AlgoTemplates
        Graphs/Flows/Dinic.cpp)
