#include <iostream>
#include <chrono>
#include "sort.h"
#include "../File/file.h"

/*
 * argv[1] - размер массива
 * argv[2] - путь к бинарному файлу с неотсортированным массивом
 * argv[3] - путь к бинарному файлу с выводом отсортированного массива [DISABLED]
 */

int main(int argc, char **argv) {
    const int arraySize = strtol(argv[1], nullptr, 10);

    // Считывание большого массива типа int
    FILE *inputFile = safeFileOpen(argv[2], "rb");
    int *array = (int*)malloc(sizeof(int) * arraySize);
    safeFileRead(array, sizeof(int), arraySize, inputFile);
    fclose(inputFile);

    // Замер времени работы алгоритма
    auto startTime = std::chrono::high_resolution_clock::now();
    quicksort<int>(array, 0, arraySize - 1);
    auto endTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> resultTime = endTime - startTime;
    std::cout << "Result time of quicksort: " << resultTime.count() << std::endl;

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
