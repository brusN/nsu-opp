#ifndef LAB2_OPP_VECTOR_H
#define LAB2_OPP_VECTOR_H

#include <iostream>
#include <fstream>
#include <cmath>
#include <cstring>
#include <cstdlib>

float *createFloatVector(const int &vectorSize);
float* setVectorFloatValues(float *vector, const int &vectorSize, const float &value);
void readVectorFloatValues(float *vector, const int &vectorSize, FILE *stream);
float *copyFloatVector(float *srcVector, float *dstVector, const int &vectorSize);
float getVectorNormal(float *vector, const int &vectorSize);
void printVector(float *vector, const int &vectorSize, FILE *stream);

#endif //LAB2_OPP_VECTOR_H
