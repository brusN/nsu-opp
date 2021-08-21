#ifndef LAB3_OPP_MATHSOURCE_H
#define LAB3_OPP_MATHSOURCE_H

float *floatMatrixTranspose(float *matrix, int &nSize, int &mSize);
void printMatrix(float *matrix, const int &nSize, const int &mSize);
float *floatMatrixMultiply(const float *matrixA, const int &n_sizeA, const int &m_sizeA, const float *matrixB, const int &n_sizeB, const int &m_sizeB);

#endif //LAB3_OPP_MATHSOURCE_H
