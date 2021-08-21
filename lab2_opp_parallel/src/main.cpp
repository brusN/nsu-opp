#include <iostream>
#include <fstream>
#include <chrono>
#include "file.h"
#include "matrix.h"
#include "vector.h"
#include "mathsolver.h"

int main(int argc, char** argv) {
    const int matrixSize = (int)strtol(argv[1], nullptr, 10);
    if (matrixSize <= 0) {
        std::cerr << "Wrong matrix size!" << std::endl;
        exit(1);
    }
    FILE *matrixValuesFile = safeOpenFile(argv[2], (char *)"rb");
    FILE *vectorBValuesFile = safeOpenFile(argv[3], (char *)"rb");

    float *matrixA = createFloatMatrix(matrixSize);
    readMatrixFloatValues(matrixA, matrixSize, matrixValuesFile);
    safeCloseFile(matrixValuesFile);

    float *vectorB = createFloatVector(matrixSize);
    readVectorFloatValues(vectorB, matrixSize, vectorBValuesFile);
    safeCloseFile(vectorBValuesFile);

    float *vectorX = NULL;
    for (int curNumThreads = 8; curNumThreads <= 8; ++curNumThreads) {
        auto start = std::chrono::high_resolution_clock::now();
        vectorX = getSolveFloatEquationsSystem(matrixA, vectorB, matrixSize, curNumThreads);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> result = end - start;
        std::cout << "Threads: " << curNumThreads << " | Time: " << result.count() << std::endl;
        free(vectorX);
    }

    FILE* vectorXValuesFile = safeOpenFile(argv[4], (char *)"wb");
    printVector(vectorX, matrixSize, vectorXValuesFile);
    safeCloseFile(vectorXValuesFile);

    free(matrixA);
    free(vectorB);
    return 0;
}
