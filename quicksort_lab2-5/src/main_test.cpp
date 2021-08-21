#include <iostream>
#include <chrono>
#include <fstream>
#include <cstring>
#include <string>
#include <omp.h>
#include "Single/sort.h"
#include "File/file.h"
#include "Tester/test.h"
#include "CSV/csv.h"

#include "Single/sort.h"
#include "Parallel/sort_parallel.h"

namespace lab_consts {
    const string singleCoreFilePath = "singleProg.csv";
    const string multiCoreFilePath = "parallelProg.csv";
    const char splitter = ';';
}

int main(int argc, char **argv) {
    CSVFile singleCoreProgFile(lab_consts::singleCoreFilePath, lab_consts::splitter);
    CSVFile multiCoreProgFile(lab_consts::multiCoreFilePath, lab_consts::splitter);
    const int shift = 1e4;
    int startArraySize = 3e5;
    const int finalSize = 7e5;
    const int numThreads = static_cast<int>(strtol(argv[1], nullptr, 10));

    int *array1 = nullptr;
    int *array2 = nullptr;

    for (int arraySize = startArraySize; arraySize < finalSize; arraySize += shift) {
        std::cout << "Array size: " << arraySize << std::endl;

        // Создаем две копии массива
        array1 = generateRandomArray<int>(arraySize, INT32_MAX, 0);
        array2 = (int*)malloc(sizeof(int) * arraySize);
        array2 = static_cast<int *>(std::memcpy(array2, array1, sizeof(int) * arraySize));

        // Замеряем время для однопоточной программы
        auto start = std::chrono::high_resolution_clock::now();
        quicksort<int>(array1, 0, arraySize - 1);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> resultTime = end - start;
        std::cout << "Single core program time: " << resultTime.count() << std::endl;
        singleCoreProgFile.addNote(std::to_string(arraySize), std::to_string(resultTime.count()));

        // Замеряем время для многопоточной программы
        start = std::chrono::high_resolution_clock::now();
        #pragma omp parallel shared(array2) num_threads(numThreads)
        {
            #pragma omp single nowait
            quicksort_parallel<int>(array2, 0, arraySize - 1);
        }
        end = std::chrono::high_resolution_clock::now();
        resultTime = end - start;
        std::cout << "Multi core program time: " << resultTime.count() << std::endl;
        multiCoreProgFile.addNote(std::to_string(arraySize), std::to_string(resultTime.count()));

        std::cout << std::endl;

        free(array1);
        free(array2);
    }

    return 0;
}