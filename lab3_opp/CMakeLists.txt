cmake_minimum_required(VERSION 3.19)
project(lab3_opp)

find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_COMPILER mpicc)
set(CMAKE_CXX_COMPILER mpicxx)

set(SOURCE_MPI_LAB3 src/main.cpp src/file.cpp src/mathsource.cpp src/source.cpp src/mpisource.cpp)

add_executable(test_lab src/test.cpp src/mathsource.cpp src/source.cpp src/file.cpp src/mpisource.cpp)
add_executable(lab3_opp ${SOURCE_MPI_LAB3})