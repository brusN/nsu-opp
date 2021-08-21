#include <iostream>
#include <mpi.h>
#include "file.h"
#include "source.h"
#include "mpisource.h"
#include "mathsource.h"

int main(int argc, char **argv) {

    int size, rank;
    const int rootRank = ROOT_RANK_NUM;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Парсим аргументы командной строки
    int n_sizeA, m_sizeA, n_sizeB, m_sizeB, n_sizeC, m_sizeC;
    std::string fileAName, fileBName, fileCName;
    parseInputArgs(argc, argv, n_sizeA, m_sizeA, fileAName, n_sizeB, m_sizeB, fileBName, fileCName);
    n_sizeC = n_sizeA;
    m_sizeC = m_sizeB;

    // Создание коммуникатор с топологией 2D решетки
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

    // Создаем коммуникаторы для строк и столбцов
    MPI_Cart_get(gridComm, ndims, dims, periods, coords); // Координаты процесса в решетке (gridComm)
    MPI_Comm rowComm, columnComm;
    MPI_Comm_split(gridComm, coords[1], coords[0], &rowComm);
    MPI_Comm_split(gridComm, coords[0], coords[1], &columnComm);

    // Получаем номер текущего процесса и корневого процесса в решетке
    int gridRank;
    int gridRootRank;
    const int rootCoords[2] = {0, 0};
    MPI_Comm_rank(gridComm, &gridRank);
    MPI_Cart_rank(gridComm, rootCoords, &gridRootRank);

    // Считываем данные с файла в rootRank для дальнейшей рассылки остальным
    // Матрица B будет транспонирована сразу после чтения из файла
    float *matrixA = nullptr;
    float *matrixB = nullptr;

    if (gridRank == gridRootRank) {
        matrixA = initFloatMatrix(n_sizeA, m_sizeA);
        FILE *fileA = safeOpenFile(fileAName.c_str(), (char *) "rb");
        matrixA = readFloatMatrixFromFile(fileA, matrixA, n_sizeA, m_sizeA);
        safeCloseFile(fileA);

        matrixB = initFloatMatrix(n_sizeB, m_sizeB);
        FILE *fileB = safeOpenFile(fileBName.c_str(), (char *) "rb");
        matrixB = readFloatMatrixFromFile(fileB, matrixB, n_sizeB, m_sizeB);
        safeCloseFile(fileB);

    }

    // Разрезаем на куски матрицу A и матрицу B
    float *matrixAPart = nullptr;
    int countRowsA[sizeX]; // Кол-во строк матрицы A для каждого процесса
    int countElementsForEachProcA[sizeX]; // Кол-во элементов матрицы A для каждого процесса
    int shiftTableA[sizeX]; // Сдвиг относительно начала матрицы А для распределения частей между процессами

    float *matrixBPart = nullptr;
    int countColumnsB[sizeY]; // Кол-во столбцов матрицы B для каждого процесса
    int countElementsForEachProcB[sizeY]; // Кол-во элементов MPI_Type_vector для каждого процесса
    int shiftTableB[sizeY]; // Сдвиг для MPI_Type_vector
    int countVectorType[sizeY]; // Для Scatterv рассылки MPI_Type_vector

    // Получаем информацию на корневом процессе для рассылки остальным
    if (gridRank == gridRootRank) {
        // Заполняем массивы для матрицы А
        getInfoForBroadcastDataForMatrix(sizeX, n_sizeA, m_sizeA, countRowsA, countElementsForEachProcA, shiftTableA);

        // Чтобы не транспонировать матрицу, воспользуемся MPI_Type_vector
        // Заполняем массивы для Scatterv рассылки MPI_Type_vector
        countColumnsB[0] = getCountRowsForProcess(sizeY, 0, m_sizeB);
        countElementsForEachProcB[0] = 1;
        countVectorType[0] = 1;
        for (int curRank = 1; curRank < sizeY; ++curRank) {
            countColumnsB[curRank] = getCountRowsForProcess(sizeY, curRank, m_sizeB);
            shiftTableB[curRank] = shiftTableB[curRank - 1] + 1;
            countElementsForEachProcB[curRank] = 1;
            countVectorType[curRank] = 1;
        }
    }

    MPI_Bcast(countRowsA, sizeX, MPI_INT, gridRootRank, MPI_COMM_WORLD);
    MPI_Bcast(countElementsForEachProcA, sizeX, MPI_INT, gridRootRank, MPI_COMM_WORLD);
    MPI_Bcast(shiftTableA, sizeX, MPI_INT, gridRootRank, MPI_COMM_WORLD);

    MPI_Bcast(countColumnsB, sizeY, MPI_INT, gridRootRank, MPI_COMM_WORLD);
    MPI_Bcast(shiftTableB, sizeY, MPI_INT, gridRootRank, MPI_COMM_WORLD);
    MPI_Bcast(countElementsForEachProcB, sizeY, MPI_INT, gridRootRank, MPI_COMM_WORLD);
    MPI_Bcast(countVectorType, sizeY, MPI_INT, gridRootRank, MPI_COMM_WORLD);

    matrixAPart = initFloatMatrix(countRowsA[coords[0]], m_sizeA);
    matrixBPart = initFloatMatrix(countColumnsB[coords[1]], m_sizeB);

    if (coords[1] == 0) {
        // Распределяем части матрицы А вдоль оси Х
        MPI_Scatterv(matrixA, countElementsForEachProcA, shiftTableA, MPI_FLOAT, matrixAPart,countElementsForEachProcA[coords[0]], MPI_FLOAT, gridRootRank, rowComm);
    }

    // Рассылаем остальным узлам вдоль оси Y
    MPI_Bcast(matrixAPart, countElementsForEachProcA[coords[0]], MPI_FLOAT, coords[0], columnComm);

    // Создаем новый тип данных для рассылки столбцов матрицы В
    MPI_Datatype columnMatrixB, columnMatrixBType;
    MPI_Type_vector(n_sizeB, countColumnsB[coords[1]], m_sizeB, MPI_FLOAT, &columnMatrixB);
    MPI_Type_commit(&columnMatrixB);

    //  Для корректной работы Scatterv
    MPI_Type_create_resized(columnMatrixB, 0,countColumnsB[coords[1]] * sizeof(float), &columnMatrixBType);
    MPI_Type_commit(&columnMatrixBType);

    // Распределяем части матрицы B вдоль оси Y
    if (coords[0] == 0) {
        MPI_Scatterv(matrixB, countVectorType, shiftTableB, columnMatrixBType, matrixBPart, 1, columnMatrixBType, gridRootRank, columnComm);
    }
    // Рассылаем остальным узлам вдоль оси Х
    MPI_Bcast(matrixBPart, countElementsForEachProcA[coords[1]], MPI_FLOAT, coords[1], rowComm);


    // Освобождаем память под матрицы А и В на корневом узле
    if (gridRank == gridRootRank) {
        free(matrixA);
        free(matrixB);
    }

    // Получение подматриц матрицы С
    float *matrixCPart = initFloatMatrix(countRowsA[coords[0]], countColumnsB[coords[1]]);
    for (int i = 0; i < countRowsA[coords[0]]; ++i) {
        for (int j = 0; j < countColumnsB[coords[1]]; ++j) {
            matrixCPart[i * countColumnsB[coords[1]] + j] = 0.0f;
            for (int k = 0; k < m_sizeA; ++k) {
                matrixCPart[i * countColumnsB[coords[1]] + j] += matrixAPart[i * m_sizeA + k] * matrixBPart[j * m_sizeA + k];
            }
        }
    }

    free(matrixCPart);
    free(matrixAPart);
    free(matrixBPart);
    MPI_Finalize();
    return 0;
}
