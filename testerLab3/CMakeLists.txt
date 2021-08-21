cmake_minimum_required(VERSION 3.19)
project(lab3_opp)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_MATRIX_GENERATOR matrix_generator/generator.cpp src/file.cpp src/source.cpp)
set(SOURCE_TESTER tester/tester.cpp src/file.cpp src/source.cpp)

set(SOURCE_MATRIX_MULTIPLY matrixMultiply/main.cpp matrixMultiply/mathsource.cpp matrixMultiply/source.cpp matrixMultiply/file.cpp)

add_executable(tester_lab3_opp ${SOURCE_TESTER})
add_executable(generator_lab3_opp ${SOURCE_MATRIX_GENERATOR})
add_executable(multiply_single_lab3_opp ${SOURCE_MATRIX_MULTIPLY})