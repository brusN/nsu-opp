#ifndef LAB3_OPP_V2_SOURCE_H
#define LAB3_OPP_V2_SOURCE_H

#include <string>
#include "file.h"

// (7 input + 1 program name)
#define REQUIRED_NUM_ARGUMENTS 8

void parseInputArguments(int argc, char **argv, int totalProcesses, std::string &fileMatrixAPath, int &nMatrixA, int &mMatrixA, std::string &fileMatrixBPath, int &nMatrixB, int &mMatrixB, std::string &fileMatrixCPath);
void *safeMalloc(size_t sizeofType, size_t count);
float *safeReadFloatMatrixFromFile(const std::string& fileMatrixAPath, float *matrix, int nSize, int mSize);
void safeWriteFloatMatrixInFile(const std::string &fileMatrixCPath, float *matrix, int nSize, int mSize);

#endif //LAB3_OPP_V2_SOURCE_H
