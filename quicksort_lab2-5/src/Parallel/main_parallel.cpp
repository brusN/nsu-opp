#include <iostream>
#include <chrono>
#include <string>
#include <cstring>
#include "sort_parallel.h"
#include "../File/file.h"
#include "../CSV/csv.h"

/*
 * argv[1] - размер массива
 * argv[2] - путь к бинарному файлу с неотсортированным массивом
 * argv[3] - количество используемых потоков
 * argv[4] - путь к бинарному файлу с выводом отсортированного массива [DISABLED]
 */

int main(int argc, char **argv) {
    const int arraySize = strtol(argv[1], nullptr, 10);
    const int maxNumThreads = 8;
    const int numThreads = strtol(argv[3], nullptr, 10);

    // Считывание большого массива
    FILE *inputFile = safeFileOpen(argv[2], "rb");
    int *array = (int*)malloc(sizeof(int) * arraySize);
    safeFileRead(array, sizeof(int), arraySize, inputFile);
    fclose(inputFile);

    CSVFile stats("statsParallelProgCores.csv", ';');
    int *copyArray = NULL;

    // Для отчета
    for (int curNumThreads = 1; curNumThreads <= maxNumThreads; ++curNumThreads) {
        copyArray = (int*)malloc(sizeof(int) * arraySize);
        copyArray = static_cast<int *>(std::memcpy(copyArray, array, sizeof(int) * arraySize));
        // Замер времени работы алгоритма
        auto startTime = std::chrono::high_resolution_clock::now();
        #pragma omp parallel shared(array) num_threads(curNumThreads)
        {
        #pragma omp single nowait
            {
                quicksort_parallel<int>(copyArray, 0, arraySize - 1);
            }
        }
        auto endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> resultTime = endTime - startTime;
        std::cout << "Cores: " << curNumThreads << " | Result time of quicksort: " << resultTime.count() << std::endl;
        stats.addNote(std::to_string(curNumThreads), std::to_string(resultTime.count()));
        free(copyArray);
    }

    /*
    // Вывод отсортированного массива
    FILE *outFile = safeFileOpen(argv[3], "wb");
    fwrite(array, sizeof(int), arraySize, outFile);
    fclose(outFile);
     */

    //printArray<int>(array, arraySize);
    free(array);
    return 0;
}
