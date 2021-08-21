#include <cmath>
#include <cstdlib>
#include "vector.h"


float *createFloatVector(const int &vectorSize) {
    return (float *)malloc(sizeof(float) * vectorSize);
}

float *setVectorFloatValues(float *vector, const int &vectorSize, const float &value) {
    for (int i = 0; i < vectorSize; ++i) {
        vector[i] = value;
    }
    return vector;
}

void readVectorFloatValues(float *vector, const int &vectorSize, FILE *stream) {
    fread(vector, sizeof(float), vectorSize, stream);
}

void printVector(float *vector, const int &vectorSize, FILE *stream) {
    fwrite(vector, sizeof(float), vectorSize, stream);
}

float getVectorNormal(float *vector, const int &vectorSize) {
    float vectorNormal = 0;
    for (int i = 0; i < vectorSize; ++i)
        vectorNormal += vector[i] * vector[i];
    return sqrtf(vectorNormal);
}

float *copyFloatVector(float *srcVector, float *dstVector, const int &vectorSize) {
    std::memcpy(dstVector, srcVector, sizeof(float) * vectorSize);
    return dstVector;
}
