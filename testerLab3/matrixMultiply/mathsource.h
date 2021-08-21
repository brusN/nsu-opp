#ifndef LAB3_OPP_V2_MATHSOURCE_H
#define LAB3_OPP_V2_MATHSOURCE_H

#include <iostream>
#include "source.h"

void printMatrix(float *matrix, const int &nSize, const int &mSize);
float *floatMatrixMultiply(const float *matrixA, const int &n_sizeA, const int &m_sizeA, const float *matrixB, const int &n_sizeB, const int &m_sizeB);
float *multiplyMatrices(float *matrix1, float *matrix2, const size_t &n1, const size_t &m1, const size_t &n2, const size_t &m2);
#endif //LAB3_OPP_V2_MATHSOURCE_H
