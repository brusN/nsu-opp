#include "mpisource.h"

void parseInputArgs(
        const int &argc, char **argv,
        int &n_sizeA, int &m_sizeA, std::string &fileNameA,
        int &n_sizeB, int &m_sizeB, std::string &fileNameB,
        std::string &fileNameC)
{

    if (argc != MAX_ARGS) {
        std::cerr << "Wrong number of arguments!" << std::endl;
        exit(1);
    }

    m_sizeA = static_cast<int>(strtol(argv[2], nullptr, 10));
    n_sizeB = static_cast<int>(strtol(argv[4], nullptr, 10));
    if (m_sizeA != n_sizeB) {
        std::cerr << "Matrix multiplication is not possible: dimension doesn't match!" << std::endl;
        exit(1);
    }

    n_sizeA = static_cast<int>(strtol(argv[1], nullptr, 10));
    m_sizeB = static_cast<int>(strtol(argv[5], nullptr, 10));
    fileNameA = argv[3];
    fileNameB = argv[6];
    fileNameC = argv[7];
}

void readDataFromFiles(
        float *matrixA, const int &n_sizeA, const int &m_sizeA,
        float *matrixB, int &n_sizeB, int &m_sizeB,
        const std::string &filePathA, const std::string &filePathB)
{
    matrixA = initFloatMatrix(n_sizeA, m_sizeA);
    FILE *fileA = safeOpenFile(filePathA.c_str(), (char *)"rb");
    matrixA = readFloatMatrixFromFile(fileA, matrixA, n_sizeA, m_sizeA);
    safeCloseFile(fileA);

    matrixB = initFloatMatrix(n_sizeB, m_sizeB);
    FILE *fileB = safeOpenFile(filePathB.c_str(), (char *)"rb");
    matrixB = readFloatMatrixFromFile(fileB, matrixB, n_sizeB, m_sizeB);
    safeCloseFile(fileB);

    matrixB = floatMatrixTranspose(matrixB, n_sizeB, m_sizeB);
}

void getInfoForBroadcastDataForMatrix(
        const int &numProcesses,
        const int &n_size, const int &m_size, int *countRows, int *countElements, int *shiftTable)
{
    countRows[0] = getCountRowsForProcess(numProcesses, 0, n_size);
    countElements[0] = countRows[0] * m_size;
    shiftTable[0] = 0;

    for (int curRank = 1; curRank < numProcesses; ++curRank) {
        countRows[curRank] = getCountRowsForProcess(numProcesses, curRank, n_size);
        countElements[curRank] = countRows[curRank] * m_size;
        shiftTable[curRank] = shiftTable[curRank - 1] + countElements[curRank];
    }
}

void splitDataForEachProc(const float *matrixA, const int &n_sizeA, const int &m_sizeA,
                          const float *matrixB, const int &n_sizeB, const int &m_sizeB,
                          float *matrixAPart, float *matrixBPart)
{

}

