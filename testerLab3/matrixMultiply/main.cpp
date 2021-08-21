#include <iostream>
#include <string>
#include "source.h"
#include "file.h"

#include "mathsource.h"

int main(int argc, char **argv) {
    if (argc != 8) {
        std::cerr << "Wrong number of arguments!" << std::endl;
        exit(1);
    }

    std::string fileAPath = argv[1];
    int n_sizeA = static_cast<int>(strtol(argv[2], nullptr, 10));
    int m_sizeA = static_cast<int>(strtol(argv[3], nullptr, 10));
    std::string fileBPath = argv[4];
    int n_sizeB = static_cast<int>(strtol(argv[5], nullptr, 10));
    int m_sizeB = static_cast<int>(strtol(argv[6], nullptr, 10));
    std::string outFileName = argv[7];

    FILE* fileA = safeOpenFile(fileAPath.c_str(), "rb");
    float *matrixA = safeReadFloatMatrixFromFile(fileAPath, matrixA, n_sizeA, m_sizeA);
    safeCloseFile(fileA);

    FILE* fileB = safeOpenFile(fileBPath.c_str(), "rb");
    float *matrixB = safeReadFloatMatrixFromFile(fileBPath, matrixB, n_sizeB, m_sizeB);
    safeCloseFile(fileB);

    float *matrixC = multiplyMatrices(matrixA, matrixB, n_sizeA, m_sizeA, n_sizeB, m_sizeB);

    printMatrix(matrixC, n_sizeA, m_sizeB);

    FILE *outputFile = safeOpenFile(outFileName.c_str(), "wb");
    fwrite(matrixC, sizeof(float), n_sizeA * m_sizeB, outputFile);
    safeCloseFile(outputFile);

    free(matrixA);
    free(matrixB);
    free(matrixC);
    std::cout << "Single core multiply has finished!" << std::endl;
    return 0;
}