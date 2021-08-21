#ifndef LAB1_OPP_SOURCE_H
#define LAB1_OPP_SOURCE_H

#include <iostream>
#include <cmath>

namespace lab2_consts {
    const float tao = -1e-2;
    const float epslon = 1e-2;
}

float *initFloatMatrix(const int &sizeA, const int &sizeB);
float *initFloatVector(const int &size);
float *readFloatMatrixFromFile(FILE *stream, float *matrix, const int &sizeA, const int &sizeB);
float *readFloatVectorFromFile(FILE *stream, float *vector, const int &size);
int getCountRowsForProcess(const int &countProc, const int &rank, const int &sizeA);
int getShiftIndexForArray(const int &rank, const int *countPartsForEachProc);
float getFloatVectorNormal(const float *vector, const int &size);

#endif //LAB1_OPP_SOURCE_H
