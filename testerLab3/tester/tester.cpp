#include <iostream>
#include "../src/file.h"
#include "../src/source.h"

int main(int argc, char **argv) {
    if (argc != 5) {
        std::cerr << "Wrong number of arguments!" << std::endl;
        exit(1);
    }

    std::string fileAPath = argv[1];
    std::string fileBPath = argv[2];
    int n_size = static_cast<int>(strtol(argv[3], nullptr, 10));
    int m_size = static_cast<int>(strtol(argv[4], nullptr, 10));

    FILE* fileA = safeOpenFile(fileAPath.c_str(), "rb");
    float *matrixA = initFloatMatrix(n_size, m_size);
    readFloatMatrixFromFile(fileA, matrixA, n_size, m_size);
    safeCloseFile(fileA);

    FILE* fileB = safeOpenFile(fileBPath.c_str(), "rb");
    float *matrixB = initFloatMatrix(n_size, m_size);
    readFloatMatrixFromFile(fileB, matrixB, n_size, m_size);
    safeCloseFile(fileB);

    float epslon = 1e-5;
    for (int i = 0; i < n_size; ++i) {
        for (int j = 0; j < m_size; ++j) {
            if (matrixA[i * m_size + j] - matrixB[i * m_size + j] > epslon) {
                std::cerr << "Mismatch detected! Exit called" << std::endl;
                exit(1);
            }
        }
    }

    std::cout << "Matrices matched!" << std::endl;
    free(matrixA);
    free(matrixB);
    return 0;
}