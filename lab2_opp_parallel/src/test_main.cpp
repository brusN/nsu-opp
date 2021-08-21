#include <iostream>
#include "matrix.h"
#include "vector.h"
#include "mathsolver.h"
#include "file.h"

int main(int argc, char **argv) {
    std::ifstream matrixFile("matrixA.txt");
    std::ifstream vectorBFile("vectorB.txt");
    if (!matrixFile.is_open() || !vectorBFile.is_open()) {
        std::cerr << "Input files hasn't opened!" << std::endl;
        exit(1);
    }

    const int matrixSize = static_cast<int>(strtol(argv[1], nullptr, 10));
    float *matrixA = createFloatMatrix(matrixSize);
    for (int i = 0; i < matrixSize; ++i) {
        for (int j = 0; j < matrixSize; ++j) {
            matrixFile >> matrixA[i * matrixSize + j];
        }
    }
    matrixFile.close();

    float *vectorB = createFloatVector(matrixSize);
    for (int i = 0; i < matrixSize; ++i) {
        vectorBFile >> vectorB[i];
    }
    vectorBFile.close();

    float *vectorX = getSolveFloatEquationsSystem(matrixA, vectorB, matrixSize, <#initializer#>);
    std::ofstream outFile("vectorX.txt");
    for (int i = 0; i < matrixSize; ++i) {
        outFile << vectorX[i] << std::endl;
    }
    outFile.close();

    free(vectorB);
    free(vectorX);
    free(matrixA);
}