#include "mathsource.h"

void printMatrix(float *matrix, const int &nSize, const int &mSize) {
    for (int i = 0; i < nSize; ++i) {
        for (int j = 0; j < mSize - 1; ++j) {
            std::cout << matrix[i * mSize + j] << " ";
        }
        std::cout << matrix[i * mSize + mSize - 1] << std::endl << std::endl;
    }
}

float *floatMatrixMultiply(const float *matrixA, const int &n_sizeA, const int &m_sizeA, const float *matrixB, const int &n_sizeB, const int &m_sizeB) {
    float *matrixC = (float *) safeMalloc(sizeof (float), n_sizeA * m_sizeB);
    for (int i = 0; i < n_sizeA; ++i) {
        for (int j = 0; j < m_sizeB; ++j) {
            matrixC[i * m_sizeB + j] = 0.0f;
            for (int k = 0; k < n_sizeB; ++k) {
                matrixC[i * m_sizeB + j] += matrixA[i * n_sizeB + k] * matrixB[k * m_sizeB + j];
            }
        }
    }
    return matrixC;
}