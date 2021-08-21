#ifndef LAB3_OPP_MPISOURCE_H
#define LAB3_OPP_MPISOURCE_H

#define MAX_ARGS 8

#include <iostream>
#include <string>
#include <mpi.h>
#include "source.h"
#include "mathsource.h"
#include "file.h"

void parseInputArgs(const int &argc, char **argv, int &n_sizeA, int &m_sizeA, std::string &fileNameA, int &n_sizeB, int &m_sizeB, std::string &fileNameB, std::string &fileNameC);
void readDataFromFiles(float *matrixA, const int &n_sizeA, const int &m_sizeA, float *matrixB, int &n_sizeB, int &m_sizeB, const std::string &filePathA, const std::string &filePathB);
void splitDataForEachProc(const float *matrixA, const int &n_sizeA, const int &m_sizeA, const float *matrixB, const int &n_sizeB, const int &m_sizeB,  float *matrixAPart, float *matrixBPart);
void getInfoForBroadcastDataForMatrix(const int &numProcesses, const int &n_size, const int &m_size, int *countRows, int *countElements, int *shiftTable);
void sendDataForEachProc();

#endif //LAB3_OPP_MPISOURCE_H
