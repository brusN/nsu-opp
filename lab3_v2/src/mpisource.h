#ifndef LAB3_OPP_V2_MPISOURCE_H
#define LAB3_OPP_V2_MPISOURCE_H

#include <mpi.h>

MPI_Comm createGridComm(int totalProcesses, int *periods, int *dims, int ndims);

void scatterFloatMatrixColumns(float *matrix, int nSize, int mSize, float *matrixPart, int countColumns, int gridRootRank,
                          int *coords, MPI_Comm &comm);

void collectBlockOfMatrixC(float *matrixC, float *matrixCPart, int countRows, int countColumns, int mSizeB, int *recvcount,
                      int *displs, int gridRootRank, MPI_Comm &gridComm);

void getInfoForGatherv(int *recvcount, int *displs, int totalProcesses, int *dims, int countRows, int countColumns,
                       int mSizeC);

#endif //LAB3_OPP_V2_MPISOURCE_H
