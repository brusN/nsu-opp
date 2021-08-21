cmake_minimum_required(VERSION 3.17)
project(lab2_opp_parallel)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

add_executable(lab2_opp_parallel_v0 src/main.cpp src/vector.cpp src/matrix.cpp src/file.cpp src/mathsolver0.cpp)
add_executable(lab2_opp_parallel_v1 src/main.cpp src/vector.cpp src/matrix.cpp src/file.cpp src/mathsolver.cpp)
add_executable(lab2_opp_parallel_v2 src/main.cpp src/vector.cpp src/matrix.cpp src/file.cpp src/mathsolver2.cpp)

add_definitions(-O3)

add_executable(test_parallel src/test_main.cpp src/vector.cpp src/matrix.cpp src/file.cpp src/mathsolver.cpp)
add_executable(test_para src/test_par.cpp src/vector.cpp src/matrix.cpp src/file.cpp src/mathsolver.cpp)