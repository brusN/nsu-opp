#ifndef LAB3_OPP_SOURCE_H
#define LAB3_OPP_SOURCE_H

#define ROOT_RANK_NUM 0

#include <iostream>
#include <cmath>

float *initFloatMatrix(const int &sizeA, const int &sizeB);
float *initFloatVector(const int &size);
float *readFloatMatrixFromFile(FILE *stream, float *matrix, const int &sizeA, const int &sizeB);
float *readFloatVectorFromFile(FILE *stream, float *vector, const int &size);
int getCountRowsForProcess(const int &countProc, const int &rank, const int &sizeA);
int getShiftIndexForArray(const int &rank, const int *countPartsForEachProc);

#endif //LAB3_OPP_SOURCE_H
