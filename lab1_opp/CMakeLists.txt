cmake_minimum_required(VERSION 3.17)
project(lab1_opp)

find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_COMPILER mpicc)
set(CMAKE_CXX_COMPILER mpicxx)

set(SOURCE_MPI_LAB src/main.cpp src/source.cpp src/file.cpp src/mathsource.cpp)
set(SOURCE_TEST src/test.cpp src/source.cpp)

add_executable(lab1_opp ${SOURCE_MPI_LAB})
add_executable(test_lab ${SOURCE_TEST})