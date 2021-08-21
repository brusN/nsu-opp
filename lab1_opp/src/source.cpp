#include "source.h"

float *initFloatMatrix(const int &sizeA, const int &sizeB) {
    return (float *) malloc(sizeof(float) * sizeA * sizeB);
}

float *initFloatVector(const int &size) {
    return (float *) malloc(sizeof(float) * size);
}

float *readFloatMatrixFromFile(FILE *stream, float *matrix, const int &sizeA, const int &sizeB) {
    if (fread(matrix, sizeof(float), sizeA * sizeB, stream) == 0) {
        std::cerr << "Error reading stream! [read float matrix]" << std::endl;
        exit(1);
    }
    return matrix;
}

float *readFloatVectorFromFile(FILE *stream, float *vector, const int &size) {
    if (fread(vector, sizeof(float), size, stream) == 0) {
        std::cerr << "Error reading stream! [read float vector]" << std::endl;
        exit(1);
    }
    return vector;
}

int getCountRowsForProcess(const int &countProc, const int &rank, const int &sizeA) {
    int countOverflowProcesses = sizeA % countProc;
    int countRows = sizeA / countProc;
    if (rank < countOverflowProcesses)
        ++countRows;
    return countRows;
}

int getShiftIndexForArray(const int &rank, const int *countPartsForEachProc) {
    int shiftIndex = 0;
    for (int curRank = 1; curRank <= rank; ++curRank)
        shiftIndex += countPartsForEachProc[curRank - 1];
    return shiftIndex;
}

float getFloatVectorNormal(const float *vector, const int &size) {
    float vectorNormal = 0.0f;
    for (int i = 0; i < size; ++i)
        vectorNormal += vector[i] * vector[i];
    return sqrtf(vectorNormal);
}

float *setVectorFloatValues(float *vector, const int &vectorSize, const float &value) {
    for (int i = 0; i < vectorSize; ++i)
        vector[i] = value;
    return vector;
}
