#include "mathsource.h"

void printMatrix(float *matrix, const int &nSize, const int &mSize) {
    for (int i = 0; i < nSize; ++i) {
        for (int j = 0; j < mSize - 1; ++j) {
            std::cout << matrix[i * mSize + j] << " ";
        }
        std::cout << matrix[i * mSize + mSize - 1] << std::endl;
    }
}

float *floatMatrixMultiply(const float *matrixA, const int &n_sizeA, const int &m_sizeA, const float *matrixB, const int &n_sizeB, const int &m_sizeB) {
    float *matrixC = (float *) safeMalloc(sizeof (float), n_sizeA * m_sizeB);
    for (int i = 0; i < n_sizeA; ++i) {
        for (int j = 0; j < m_sizeB; ++j)
        {
            matrixC[i * m_sizeB + j] = 0.0f;
            for (int k = 0; k < n_sizeB; ++k) {
                matrixC[i * m_sizeB + j] += matrixA[i * n_sizeB + k] * matrixB[k * m_sizeB + j];
            }
        }
    }
    return matrixC;
}

float *multiplyMatrices(float *matrix1, float *matrix2, const size_t &n1, const size_t &m1, const size_t &n2, const size_t &m2) {
    float *result = new float[n1 * m2];
    for (size_t i = 0; i < n1; ++i) {
        for (size_t j = 0; j < m2; ++j) {
            for (size_t k = 0; k < m1; ++k) {
                result[i * m2 + j] += matrix1[i * m1 + k] * matrix2[k * m2 + j];
            }
        }
    }
    return result;
}