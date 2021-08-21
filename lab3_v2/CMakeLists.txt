cmake_minimum_required(VERSION 3.19)
project(lab3_opp_v2)

find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_COMPILER mpicc)
set(CMAKE_CXX_COMPILER mpicxx)

set(SOURCE_MPI_LAB3 src/main.cpp src/file.cpp src/mpisource.cpp src/source.cpp src/mathsource.cpp)
add_executable(lab3_opp_v2 ${SOURCE_MPI_LAB3})

