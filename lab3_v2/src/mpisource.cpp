#include "mpisource.h"

MPI_Comm createGridComm(int totalProcesses, int *periods, int *dims, int ndims) {

    // Получаем оптимальную размерность
    for (int i = 0; i < ndims; ++i) {
        dims[i] = 0;
        periods[i] = 0;
    }
    MPI_Dims_create(totalProcesses, ndims, dims);

    // Создаем новый коммуникатор
    MPI_Comm gridComm;
    MPI_Cart_create(MPI_COMM_WORLD, ndims, dims, periods, 0, &gridComm);
    return gridComm;
}

void scatterFloatMatrixColumns(float *matrix, int nSize, int mSize, float *matrixPart, int countColumns, int gridRootRank, int *coords, MPI_Comm &comm) {
    // Используем MPI_Type_vector, чтобы не транспонировать матрицу
    MPI_Datatype columnMatrix, columnMatrixType;
    MPI_Type_vector(nSize, countColumns, mSize, MPI_FLOAT, &columnMatrix);
    MPI_Type_commit(&columnMatrix);

    // Для корректной работы MPI_Scatter
    MPI_Type_create_resized(columnMatrix, 0, countColumns * sizeof(float), &columnMatrixType);
    MPI_Type_commit(&columnMatrixType);

    // Рассылаем по процессам
    if (coords[0] == 0)
        MPI_Scatter(matrix, 1, columnMatrixType, matrixPart, countColumns * nSize, MPI_FLOAT, gridRootRank, comm);

    MPI_Type_free(&columnMatrix);
    MPI_Type_free(&columnMatrixType);
}

void getInfoForGatherv(int *recvcount, int *displs, int totalProcesses, int *dims, int countRows, int countColumns, int mSizeC) {
    int shift = 0;
    int curX = 0;
    int curY = 0;
    displs[0] = 0;
    recvcount[0] = 1;
    for (int curRank = 1; curRank < totalProcesses; ++curRank) {
        curX = curRank % dims[1];
        curY = curRank / dims[1];
        if (curX == 0) {
            shift = curY * mSizeC * countRows;
            displs[curRank] = shift;
        } else {
            displs[curRank] = shift + curX * countColumns;
        }
        recvcount[curRank] = 1;
    }
}

void collectBlockOfMatrixC(float *matrixC, float *matrixCPart, int countRows, int countColumns, int mSizeB, int *recvcount, int *displs, int gridRootRank, MPI_Comm &gridComm) {
    MPI_Datatype matrixCBlock, matrixCBlockType;
    MPI_Type_vector(countRows, countColumns, mSizeB, MPI_FLOAT, &matrixCBlock);
    MPI_Type_commit(&matrixCBlock);

    MPI_Type_create_resized(matrixCBlock, 0, sizeof(float), &matrixCBlockType);
    MPI_Type_commit(&matrixCBlockType);

    MPI_Gatherv(matrixCPart, countRows * countColumns, MPI_FLOAT, matrixC, recvcount, displs, matrixCBlockType, gridRootRank, gridComm);

    MPI_Type_free(&matrixCBlock);
    MPI_Type_free(&matrixCBlockType);
}

void fillDisplsAndRecvcountsTables(int* displs, int* recvcounts, int n1, int n3, int N3, int* dims, int procSize){
    int temp = 0;
    int currentStart = 0;
    for (int i = 1; i < procSize; ++i) {
        if (i % dims[1] != 0){
            temp = currentStart + (i % dims[1])*n3;
            displs[i] = temp;
        } else {
            temp = N3*n1*(i / dims[1]);
            currentStart = temp;
            displs[i] = temp;
        }
        recvcounts[i] = 1;
    }
    displs[0] = 0;
    recvcounts[0] = 1;
}
