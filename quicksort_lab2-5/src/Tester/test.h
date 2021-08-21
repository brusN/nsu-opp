#ifndef QUICKSORT_LAB2_5_TEST_H
#define QUICKSORT_LAB2_5_TEST_H

#include <iostream>
#include <omp.h>
#include <cstdlib>
#include <ctime>
#include "../Single/sort.h"

template <typename T> T *generateRandomArray(const int &arraySize, const int &range, const int &shift) {
    T *newArray = new T[arraySize];

    for (int i = 0; i < arraySize; ++i) {
        newArray[i] = rand() % range + shift;
    }
    return newArray;
}

template <typename T> void printArray(T* array, const int &arraySize) {
    for (int i = 0; i < arraySize - 1; ++i) {
        std::cout << array[i] << " ";
    }
    std::cout << array[arraySize - 1] << std::endl;
}

#endif //QUICKSORT_LAB2_5_TEST_H
