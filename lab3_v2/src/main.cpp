#include <iostream>
#include <mpi.h>
#include "source.h"
#include <chrono>
#include "mpisource.h"
#include "mathsource.h"

/*
 * Программа реализована только для матриц, размерность которых кратна размерностям решетки процессов
 */

int main(int argc, char **argv) {
    int totalProcesses, worldRank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &totalProcesses);
    MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);

    // Получаем и парсим аргументы программы
    int nSizeA, mSizeA, nSizeB, mSizeB;
    std::string fileMatrixAPath, fileMatrixBPath, fileMatrixCPath;
    parseInputArguments(argc, argv, totalProcesses, fileMatrixAPath, nSizeA, mSizeA, fileMatrixBPath, nSizeB, mSizeB, fileMatrixCPath);

    // Создаем коммутатор типа 2D решетка
    int ndims = 2;
    int dims[ndims];
    int periods[ndims];
    MPI_Comm gridComm = createGridComm(totalProcesses, periods, dims, ndims);

    // Координаты процесса в решетке (x,y): coords[0] ~ x, coords[1] ~ y
    // Получаем координаты процесса в gridComm
    int coords[ndims];
    MPI_Cart_get(gridComm, ndims, dims, periods, coords);

    // Распределяем процессы на отдельные строчный и строковый коммуникаторы
    MPI_Comm rowComm, columnComm;
    MPI_Comm_split(gridComm, coords[1], coords[0], &rowComm);
    MPI_Comm_split(gridComm, coords[0], coords[1], &columnComm);

    // Получаем ранг текущего и корневого процесса в gridComm
    int gridRank, gridRootRank;
    const int rootCoords[2] = {0, 0};
    MPI_Comm_rank(gridComm, &gridRank);
    MPI_Cart_rank(gridComm, rootCoords, &gridRootRank);

    // Считываем матрицы А и В с файлов
    float *matrixA = nullptr;
    float *matrixB = nullptr;
    if (gridRank == gridRootRank) {
        matrixA = safeReadFloatMatrixFromFile(fileMatrixAPath, matrixA, nSizeA, mSizeA);
        matrixB = safeReadFloatMatrixFromFile(fileMatrixBPath, matrixB, nSizeB, mSizeB);
    }

    // Считаем количество строк матрицы А и столбцов матрицы В для каждого процесса
    int countRows = nSizeA / dims[0];
    int countColumns = mSizeB / dims[1];

    // Разрезаем матрицу А на строки
    float *matrixAPart = (float *) safeMalloc(sizeof(float), countRows * mSizeA);
    if (coords[1] == 0) {
        MPI_Scatter(matrixA, countRows * mSizeA, MPI_FLOAT, matrixAPart, countRows * mSizeA, MPI_FLOAT, gridRootRank, rowComm);
    }
    MPI_Bcast(matrixAPart, countRows * mSizeA, MPI_FLOAT, 0, columnComm);

    // Разрезаем матрицу В на столбцы
    float *matrixBPart = (float *) safeMalloc(sizeof(float), countColumns * nSizeB);
    scatterFloatMatrixColumns(matrixB, nSizeB, mSizeB, matrixBPart, countColumns, gridRootRank, coords, columnComm);
    MPI_Bcast(matrixBPart, countColumns * nSizeB, MPI_FLOAT, 0, rowComm);

    // Освобождаем ненужную память на корневом процессе
    if (gridRank == gridRootRank) {
        free(matrixA);
        free(matrixB);
    }

    // Перемножаем полученные подматрицы матриц А и В
    auto startTime = std::chrono::high_resolution_clock::now();
    float *matrixCPart = floatMatrixMultiply(matrixAPart, countRows, mSizeA, matrixBPart, nSizeB, countColumns);

    // Собираем в одну матрицу на корневом процессе
    float *matrixC = nullptr;
    if (gridRank == gridRootRank) {
        matrixC = (float *) safeMalloc(sizeof(float), nSizeA * mSizeB);
    }

    // Получаем информацию для сборки и собираем матрицу С
    int displs[totalProcesses];
    int recvcount[totalProcesses];
    getInfoForGatherv(recvcount, displs, totalProcesses, dims, countRows, countColumns, mSizeB);
    collectBlockOfMatrixC(matrixC, matrixCPart, countRows, countColumns, mSizeB, recvcount, displs, gridRootRank, gridComm);
    auto endTime = std::chrono::high_resolution_clock::now();

    // Считаем общее время работы алгоритма умножения матриц
    std::chrono::duration<double> resultTimeForProc = endTime - startTime;
    double averageTime = resultTimeForProc.count();
    MPI_Allreduce(MPI_IN_PLACE, &averageTime, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    averageTime /= totalProcesses;

    // Освобождаем ненужную память на корневом процессе
    if (gridRank == gridRootRank) {
        //printMatrix(matrixC, nSizeA, mSizeB);
        std::cout << "Average time for proc: " << averageTime << " seconds" << std::endl;
        safeWriteFloatMatrixInFile(fileMatrixCPath, matrixC, nSizeA, mSizeB);
        free(matrixC);
    }

    free(matrixCPart);
    free(matrixAPart);
    free(matrixBPart);
    MPI_Finalize();
    return 0;
}
