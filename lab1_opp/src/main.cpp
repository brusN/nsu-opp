#include <iostream>
#include <cstring>
#include <cstdlib>
#include <chrono>
#include <mpi.h>
#include "file.h"
#include "source.h"
#include "mathsource.h"

int main(int argc, char **argv) {
    int matrixSize = 2500;
    int numProcesses, rank;
    const int rootRank = 0;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int countRows[numProcesses]; // Кол-во строк матрицы для каждого процесса
    int countElementsForEachProc[numProcesses]; // Кол-во элементов матрицы для каждого процесса
    int shiftTable[numProcesses]; // Сдвиг относительно начала матрицы А для распределения частей между процессами
    int shiftIndexTable[numProcesses]; // Сдвиг относительно начала вектора B для распределения частей между процессами
    if (rank == rootRank) {
        countRows[0] = getCountRowsForProcess(numProcesses, rootRank, matrixSize);
        countElementsForEachProc[0] = countRows[0] * matrixSize;
        shiftTable[0] = 0;
        shiftIndexTable[0] = 0;
        for (int curRank = 1; curRank < numProcesses; ++curRank) {
            countRows[curRank] = getCountRowsForProcess(numProcesses, curRank, matrixSize);
            countElementsForEachProc[curRank] = countRows[curRank] * matrixSize;
            shiftTable[curRank] = shiftTable[curRank - 1] + countElementsForEachProc[curRank - 1];
            shiftIndexTable[curRank] = shiftIndexTable[curRank - 1] + countRows[curRank - 1];
        }
    }
    MPI_Bcast(countRows, numProcesses, MPI_INT, rootRank, MPI_COMM_WORLD);
    MPI_Bcast(countElementsForEachProc, numProcesses, MPI_INT, rootRank, MPI_COMM_WORLD);
    MPI_Bcast(shiftTable, numProcesses, MPI_INT, rootRank, MPI_COMM_WORLD);
    MPI_Bcast(shiftIndexTable, numProcesses, MPI_INT, rootRank, MPI_COMM_WORLD);

    float *matrixA = nullptr; // Выделение памяти только для rootRank
    float *vectorX = nullptr; // Выведеление памяти только для rootRank
    float *vectorB = nullptr; // Выделение памяти только для rootRank
    float *matrixAPart = initFloatMatrix(countRows[rank], matrixSize); // Часть матрицы А для процесса с рангом rank
    float *vectorBPart = initFloatVector(countRows[rank]); // Часть вектора В для процесса с рангом rank
    float *vectorXPart = initFloatVector(countRows[rank]); // Часть вектора X для процесса с рангом rank
    for (int i = 0; i < countRows[rank]; ++i)
        vectorXPart[i] = 0.0f;

    if (rank == rootRank) {
        // Считываем целиком матрицу А и храним в процессе А
        FILE *matrixAValuesFile = safeOpenFile((char *)"matA.bin", (char *) "rb");
        matrixA = initFloatMatrix(matrixSize, matrixSize);
        matrixA = readFloatMatrixFromFile(matrixAValuesFile, matrixA, matrixSize, matrixSize);

        // Сразу транспонируем матрицу А, чтобы разрезать на колонки
        matrixA = floatMatrixTranspose(matrixA, matrixSize, matrixSize);
        safeCloseFile(matrixAValuesFile);

        // Считываем целиком вектор В и храним в процессе А
        FILE *vectorBValuesFile = safeOpenFile((char *)"vecB.bin", (char *) "rb");
        vectorB = initFloatVector(matrixSize);
        vectorB = readFloatVectorFromFile(vectorBValuesFile, vectorB, matrixSize);
        safeCloseFile(vectorBValuesFile);
    }

    // Разрезаем матрицу А на части и рассылаем для каждого процесса
    MPI_Scatterv(matrixA, countElementsForEachProc, shiftTable, MPI_FLOAT, matrixAPart, countElementsForEachProc[rank], MPI_FLOAT, rootRank, MPI_COMM_WORLD);

    // Разрезаем вектор В на части и рассылаем для каждого процесса
    MPI_Scatterv(vectorB, countRows, shiftIndexTable, MPI_FLOAT, vectorBPart, countRows[rank], MPI_FLOAT, rootRank, MPI_COMM_WORLD);

    // Освобождаем память под полные матрицу А и вектор В в rootRank
    if (rank == rootRank) {
        free(matrixA);
        free(vectorB);
    }

    // Параллельно вычисляем норму вектора В
    float exitConditionDenominator = 0.0f;
    for (int i = 0; i < countRows[rank]; ++i) {
        exitConditionDenominator += vectorBPart[i] * vectorBPart[i];
    }
    MPI_Allreduce(MPI_IN_PLACE, &exitConditionDenominator, 1, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);
    if (rank == rootRank) {
        exitConditionDenominator = sqrtf(exitConditionDenominator);
    }
    MPI_Bcast(&exitConditionDenominator, 1, MPI_FLOAT, rootRank, MPI_COMM_WORLD);

    // Начало итерационного алгоритма
    const float tao = lab2_consts::tao;
    const float epsilon = lab2_consts::epslon;
    float exitCondition = 0.0f;
    float *exitConditionVectorPart = initFloatVector(countRows[rank]);
    bool hasNextIteration = true;
    float tempResultVector[matrixSize]; // Частичная сумма
    float resultVector[matrixSize]; // Вектор, полученный умножением матрицы A на вектор X [Есть сумма частичных сумм]
    size_t countIter = 0;

    //// Начало замера времени
    auto startTimeForProc = std:: chrono::high_resolution_clock::now();

    do {
        exitCondition = 0.0f;
        memset(tempResultVector, 0, matrixSize * sizeof(float));
        for (int i = 0; i < countRows[rank]; ++i) {
            for (int j = 0; j < matrixSize; ++j) {
                tempResultVector[j] += matrixAPart[i * matrixSize + j] * vectorXPart[i];
            }
        }
        MPI_Reduce_scatter(tempResultVector, resultVector, countRows, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);
        for (int i = 0; i < countRows[rank]; ++i) {
            vectorXPart[i] -= tao * (resultVector[i] - vectorBPart[i]);
            exitConditionVectorPart[i] = resultVector[i] - vectorBPart[i];
            exitCondition += exitConditionVectorPart[i] * exitConditionVectorPart[i];
        }
        MPI_Allreduce(MPI_IN_PLACE, &exitCondition, 1, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);

        if (rank == rootRank) {
            ++countIter;
            exitCondition = sqrtf(exitCondition) / exitConditionDenominator;
            //printf("%f\n ", exitCondition);
            if (exitCondition < epsilon) {
                hasNextIteration = false;
            }
        }
        MPI_Bcast(&hasNextIteration, 1, MPI_CXX_BOOL, rootRank, MPI_COMM_WORLD);
    } while (hasNextIteration);

    //// Конец замера времени
    auto endTimeForProc = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> resultTimeForProc = endTimeForProc - startTimeForProc;
    double resultTimeInSecForProc = resultTimeForProc.count();
    //std::cout << resultTimeInSecForProc << " ";
    MPI_Allreduce(MPI_IN_PLACE, &resultTimeInSecForProc, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

    // Выделяем память под вектор Х
    if (rank == rootRank) {
        vectorX = initFloatVector(matrixSize);
    }

    // Собираем результирующий вектор из частей
    MPI_Gatherv(vectorXPart, countRows[rank], MPI_FLOAT, vectorX, countRows, shiftIndexTable, MPI_FLOAT, rootRank, MPI_COMM_WORLD);

    // Выводим результат в файл
    if (rank == rootRank) {
        FILE *outFile = safeOpenFile((char *)"vecX.bin", (char *)"wb");
        fwrite(vectorX, sizeof(float), matrixSize, outFile);
        safeCloseFile(outFile);
        resultTimeInSecForProc /= numProcesses; // Берем среднее значение времени
        std::cout << "Result time for iter algorithm: " << resultTimeInSecForProc << std::endl;
        std::cout << "Iter num: " << countIter << std::endl;
    }


    free(vectorX);
    free(exitConditionVectorPart);
    free(vectorXPart);
    free(vectorBPart);
    free(matrixAPart);
    MPI_Finalize();
    return 0;
}
