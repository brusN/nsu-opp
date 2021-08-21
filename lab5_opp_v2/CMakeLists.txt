cmake_minimum_required(VERSION 3.19)
project(lab5_opp_v2)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(lab5_opp_v2 src/main.cpp src/source.h)
