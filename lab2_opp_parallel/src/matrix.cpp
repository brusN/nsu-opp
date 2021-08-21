#include "matrix.h"
#include <omp.h>

float *createFloatMatrix(const int &matrixSize) {
    return (float *) malloc(sizeof(float) * matrixSize * matrixSize);
}

float *setMatrixFloatValues(float *matrix, const int &matrixSize, const float &value) {
    #pragma omp parallel for
    for (int i = 0; i < matrixSize * matrixSize; ++i) {
        matrix[i] = value;
    }
    return matrix;
}

void readMatrixFloatValues(float *matrix, const int &matrixSize, FILE *stream) {
    if (fread(matrix, sizeof(float), matrixSize * matrixSize, stream) == 0) {
        std::cerr << "Error reading file!" << std::endl;
        exit(1);
    }
}

void printMatrix(float *matrix, const int &matrixSize, FILE *stream) {
    fwrite(matrix, sizeof(float),  matrixSize * matrixSize, stream);
}
