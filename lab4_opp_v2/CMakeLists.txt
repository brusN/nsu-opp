cmake_minimum_required(VERSION 3.19)
project(lab4_opp_v2)

set(CMAKE_CXX_STANDARD 11)

find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_COMPILER mpicc)
set(CMAKE_CXX_COMPILER mpicxx)

add_executable(lab4_opp_v2 src/main.cpp src/mpisource.cpp src/source.cpp)