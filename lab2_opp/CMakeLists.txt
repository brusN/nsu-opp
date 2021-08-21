cmake_minimum_required(VERSION 3.17)
project(lab2_opp)

set(CMAKE_CXX_STANDARD 14)
include_directories(src)

add_executable(lab2_opp src/main.cpp src/matrix.cpp src/vector.cpp src/mathsolver.cpp src/file.cpp)
add_executable(test_main src/test_main.cpp src/matrix.cpp src/vector.cpp src/mathsolver.cpp src/file.cpp)