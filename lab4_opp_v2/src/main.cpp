#include <mpi.h>
#include <cstdlib>
#include <cstring>
#include <chrono>
#include "mpisource.h"

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Задаем размеры блоков каждому процессу
    long double hx, hy, hz;
    const int Nx = lab4_consts::dimension;
    const int Ny = lab4_consts::dimension;
    int Nz = lab4_consts::dimension;
    calculatingSizeOfCell(Nx, Ny, Nz, hx, hy, hz);
    Nz /= size;

    auto oldValuesPhi = (long double *) safeCalloc(Nx * Ny * Nz, sizeof(long double));
    auto newValuesPhi = (long double *) safeMalloc(sizeof(long double), Nx * Ny * Nz);
    auto lowerBoundary = (long double*) safeMalloc(sizeof(long double), Nx * Ny);
    auto upperBoundary = (long double*) safeMalloc(sizeof(long double), Nx * Ny);

    // Считаем значения на границах
    calculateValueOnBoundaries(oldValuesPhi, newValuesPhi, Nx, Ny, Nz, hx, hy, hz, rank, size);

    // Отправляем нижнюю и верхнюю границы
    MPI_Request reqs[2], reqr[2];
    sendBoundaries(oldValuesPhi, upperBoundary, lowerBoundary, Nx, Ny, Nz, reqs, reqr, rank, size);
    waitEndOfCommunication(reqr, reqs, rank, size);

    long double maxInsideDif, maxOutsideDif, localMax, globalMax = lab4_consts::epsilon;
    auto startTime = std::chrono::high_resolution_clock::now(); //// <---------- Начало замера времени
    while (globalMax >= lab4_consts::epsilon) {
        // Считаем приближение
        maxOutsideDif = calculatePhiOnBoundaries(oldValuesPhi, newValuesPhi, upperBoundary, lowerBoundary, Nx, Ny, Nz, hx, hy, hz, rank, size);

        // Обмен границами
        sendBoundaries(oldValuesPhi, upperBoundary, lowerBoundary, Nx, Ny, Nz, reqs, reqr, rank, size);

        // Расчёт условия выхода
        if (Nz > 2) {
            maxInsideDif = calculatePhiInsideArea(oldValuesPhi, newValuesPhi, Nx, Ny, Nz, hx, hy, hz);
        } else {
            maxInsideDif = maxOutsideDif;
        }
        localMax = maxInsideDif > maxOutsideDif ? maxInsideDif : maxOutsideDif;
        MPI_Allreduce(&localMax, &globalMax, 1, MPI_LONG_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
        //memcpy(oldValuesPhi, newValuesPhi, Nx * Ny * Nz * sizeof(long double));
        std::swap(oldValuesPhi, newValuesPhi);

        // Ожидание окончания обмена
        waitEndOfCommunication(reqr, reqs, rank, size);
    }
    auto endTime = std::chrono::high_resolution_clock::now(); //// <---------- Конец замера времени
    std::chrono::duration<double> resultTimeForProc = endTime - startTime;

    // Считаем среднее время
    double averageTime = resultTimeForProc.count();
    MPI_Allreduce(MPI_IN_PLACE, &averageTime, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    averageTime /= size;

    // Считаем оценку точности полученного решения
    long double localDelta, globalDelta;
    localDelta = calculateDelta(oldValuesPhi, Nx, Ny, Nz, hx, hy, hz, rank);
    MPI_Allreduce(&localDelta, &globalDelta, 1, MPI_LONG_DOUBLE, MPI_MAX, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "Delta: " << globalDelta << std::endl;
        std::cout << "Time: " << averageTime << " sec" << std::endl;
    }

    free(oldValuesPhi);
    free(newValuesPhi);
    free(upperBoundary);
    free(lowerBoundary);
    MPI_Finalize();
    return 0;
}