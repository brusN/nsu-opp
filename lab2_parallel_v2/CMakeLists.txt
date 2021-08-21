cmake_minimum_required(VERSION 3.17)
project(lab2_parallel_v2)

find_package(OpenMP REQUIRED)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

set(CMAKE_CXX_STANDARD 14)

add_executable(lab2_parallel_v2 src/main.cpp src/mathsolver.cpp src/matrix.cpp src/vector.cpp src/file.cpp)
add_executable(test_parallel src/test_main.cpp src/mathsolver.cpp src/matrix.cpp src/vector.cpp src/file.cpp)
add_executable(omp_test src/test_parallel.cpp)