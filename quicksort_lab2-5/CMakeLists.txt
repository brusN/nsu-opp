cmake_minimum_required(VERSION 3.16)
project(quicksort_lab2_5)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenMP REQUIRED)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

set(SOURCE_BUILD src/Single/main.cpp src/File/file.cpp src/CSV/csv.cpp)
set(SOURCE_BUILD_PARALLEL src/Parallel/main_parallel.cpp src/File/file.cpp src/CSV/csv.cpp)

add_executable(quicksort_lab2_5 ${SOURCE_BUILD})
add_executable(quicksort_lab2_5_parallel ${SOURCE_BUILD_PARALLEL})
add_executable(array_generator src/Tester/generator.cpp src/File/file.cpp)
add_executable(array_checker src/Tester/checker.cpp src/File/file.cpp)
add_executable(lab_test src/main_test.cpp src/File/file.cpp src/CSV/csv.cpp)
