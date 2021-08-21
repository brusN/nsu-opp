#ifndef LAB2_OPP_MATRIX_H
#define LAB2_OPP_MATRIX_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>

float* createFloatMatrix(const int &matrixSize);
float* setMatrixFloatValues(float *matrix, const int &matrixSize, const float &value);
void readMatrixFloatValues(float *matrix, const int &matrixSize, FILE *stream);
void printMatrix(float *matrix, const int &matrixSize, FILE *stream);

#endif //LAB2_OPP_MATRIX_H
