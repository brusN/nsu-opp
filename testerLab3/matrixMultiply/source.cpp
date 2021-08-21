#include <iostream>
#include "source.h"

void parseInputArguments(int argc, char **argv, int totalProcesses,
                         std::string &fileMatrixAPath, int &nMatrixA, int &mMatrixA,
                         std::string &fileMatrixBPath, int &nMatrixB, int &mMatrixB,
                         std::string &fileMatrixCPath) {
    if (argc != REQUIRED_NUM_ARGUMENTS) {
        std::cerr << "Wrong number of input arguments! Please check and try again!" << std::endl;
        exit(1);
    }

    mMatrixA = static_cast<int>(strtol(argv[3], nullptr, 10));
    nMatrixB = static_cast<int>(strtol(argv[5], nullptr, 10));

    if (mMatrixA != nMatrixB) {
        std::cerr << "Cant multiply this matrices! Wrong dimensions!" << std::endl;
        exit(1);
    }

    fileMatrixAPath = argv[1];
    nMatrixA = static_cast<int>(strtol(argv[2], nullptr, 10));

    fileMatrixBPath = argv[4];
    mMatrixB = static_cast<int>(strtol(argv[6], nullptr, 10));

    fileMatrixCPath = argv[7];

    if (nMatrixA % totalProcesses != 0 || mMatrixB % totalProcesses != 0) {
        std::cerr << "Can't multiply this matrices!" << std::endl
        << "It is impossible to get the same number of rows of matrix A or columns of matrix B for each process" << std::endl;
        exit(1);
    }
}

void *safeMalloc(size_t sizeofType, size_t count) {
    void *allocatedMemory = malloc(sizeofType * count);
    if (allocatedMemory == nullptr) {
        std::cerr << "Error while allocating memory by using malloc!" << std::endl;
        std::cerr << "Count elements: " << count << " | size of (in bytes)" << sizeofType << std::endl;
        exit(1);
    }
    return allocatedMemory;
}

float *safeReadFloatMatrixFromFile(FILE* stream, float *matrix, int sizeN, int sizeM) {
    if (fread(matrix, sizeof(float), sizeN * sizeM, stream) == 0) {
        std::cerr << "Error while reading matrix from file!" << std::endl;
        exit(1);
    }
    return matrix;
}

float * safeReadFloatMatrixFromFile(const std::string& fileMatrixAPath, float *matrix, int nSize, int mSize) {
    FILE *fileMatrixA = safeOpenFile(fileMatrixAPath.c_str(), "rb");
    matrix = (float *)safeMalloc(sizeof(float), nSize * mSize);
    if (fread(matrix, sizeof(float), nSize * mSize, fileMatrixA) == 0) {
        std::cerr << "Error while reading matrix from file " << fileMatrixAPath << std::endl;
        exit(1);
    }
    safeCloseFile(fileMatrixA);
    return matrix;
}

void safeWriteFloatMatrixInFile(const std::string &fileMatrixCPath, float *matrix, int nSize, int mSize) {
    FILE *fileMatrixC = safeOpenFile(fileMatrixCPath.c_str(), "wb");
    int countBytesForWritting = nSize * mSize;
    matrix = (float *)safeMalloc(sizeof(float), countBytesForWritting);
    if (fwrite(matrix, sizeof(float), nSize * mSize, fileMatrixC) != countBytesForWritting) {
        std::cerr << "Can't write " << countBytesForWritting << " bytes in file " << fileMatrixCPath << std::endl;
        exit(1);
    }
    safeCloseFile(fileMatrixC);
}


