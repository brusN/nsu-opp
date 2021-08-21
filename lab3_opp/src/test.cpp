#include <iostream>
#include "mathsource.h"
#include "source.h"
#include "mpisource.h"
#include "file.h"
#include <mpi.h>

int main(int argc, char ** argv) {
    int size, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm gridComm;
    int ndims = 2;
    int dims[2] = {0, 0}; // Размерность решетки
    int periods[2] = {0, 0}; // Периодичность для измерения
    int coords[2]; // Координаты процесса в решетке
    int reorder = 0; // Разрешить переупорядочивание процессов
    MPI_Dims_create(size, ndims, dims); // Получаем оптимальную размерность
    MPI_Cart_create(MPI_COMM_WORLD, ndims, dims, periods, reorder, &gridComm); // Создаем коммуникатор с топологией 2D решетка
    int sizeX = dims[0];
    int sizeY = dims[1];

    MPI_Cart_get(gridComm, ndims, dims, periods, coords); // Координаты процесса в решетке (gridComm)
    MPI_Comm rowComm, columnComm;
    MPI_Comm_split(gridComm, coords[1], coords[0], &rowComm);
    MPI_Comm_split(gridComm, coords[0], coords[1], &columnComm);

    float *matrixA = nullptr;
    float *matrixB = nullptr;
    if (rank == 0) {
        //FILE *file = safeOpenFile("matA.bin", "rb");
        //matrixA = initFloatMatrix(2500, 2500);
        //matrixA = readFloatMatrixFromFile(file, matrixA, 2500, 2500);
        //safeCloseFile(file);
    }

    int countRows[2];
    int countElements[2];
    int shiftTable[2];
    getInfoForBroadcastDataForMatrix(2, 2500, 2500, countRows, countElements, shiftTable);
    float *matrixAPart = initFloatMatrix(countRows[coords[0]], 2500);

    if (coords[1] == 0) {
        MPI_Scatterv(matrixA, countElements, shiftTable, MPI_FLOAT, matrixAPart, countElements[coords[0]], MPI_FLOAT, 0, gridComm);
    }

    MPI_Finalize();
    return 0;
}