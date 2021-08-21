#include "mpisource.h"

inline long double calculatePhiAtPoint(const long double offsetX, const long double offsetY, const long double offsetZ) {
    return (lab4_consts::X0 + offsetX) * (lab4_consts::X0 + offsetX) + (lab4_consts::Y0 + offsetY) * (lab4_consts::Y0 + offsetY) + (lab4_consts::Z0 + offsetZ) * (lab4_consts::Z0 + offsetZ);
}

void calculatingSizeOfCell(const int Nx, const int Ny, const int Nz, long double &hx, long double &hy, long double &hz) {
    hx = lab4_consts::Dx / (Nx - 1);
    hy = lab4_consts::Dy / (Ny - 1);
    hz = lab4_consts::Dz / (Nz - 1);
}

void calculateValueOnBoundaries(long double *oldValuesPhi, long double *newValuesPhi, const int Nx, const int Ny, const int Nz, const long double hx, const long double hy, const long double hz, const int rank, const int size) {
    for (int k = 0; k < Nz; ++k){
        for (int i = 0; i < Nx; ++i) {
            // Левая грань
            oldValuesPhi[i + Nx * Ny * k] = newValuesPhi[i + Nx * Ny * k] = calculatePhiAtPoint(i * hx, 0, rank * Nz * hz + k * hz);
            // Правая грань
            oldValuesPhi[i + Ny * ((Ny - 1) + Nx * k)] = newValuesPhi[i + Ny * ((Ny - 1) + Nx * k)] = calculatePhiAtPoint(i * hx, (Ny - 1) * hy, rank * Nz * hz + k * hz);
        }
    }

    for (int k = 0; k < Nz; ++k) {
        for (int j = 1; j < Ny - 1; ++j) {
            // Задняя грань
            oldValuesPhi[Ny * (j + Nx * k)] = newValuesPhi[Ny * (j + Nx * k)] = calculatePhiAtPoint(0, j * hy, rank * Nz * hz + k * hz);
            // Передняя грань
            oldValuesPhi[(Nx - 1) + Ny * (j + Nx * k)] = newValuesPhi[(Nx - 1) + Ny * (j + Nx * k)] =  calculatePhiAtPoint((Nx - 1) * hx, j * hy,
                                                                                                                           rank * Nz * hz + k * hz);
        }
    }

    // Нижняя грань
    if (rank == 0) {
        for (int j = 0; j < Ny; ++j) {
            for (int i = 0; i < Nx; ++i) {
                oldValuesPhi[i + Ny * j] = newValuesPhi[i + Ny * j] =  calculatePhiAtPoint(i * hx, j * hy, rank * Nz * hz);
            }
        }
    }
    // Вверхняя грань
    if (rank == size - 1) {
        for (int j = 0; j < Ny; ++j) {
            for (int i = 0; i < Nx; ++i) {
                oldValuesPhi[i + Ny * (j + Nx * (Nz - 1))] = newValuesPhi[i + Ny * (j + Nx * (Nz - 1))] = calculatePhiAtPoint(i * hx, j * hy, rank * Nz * hz + (Nz - 1) * hz);
            }
        }
    }
}

void sendBoundaries(const long double *phi, long double *upperBoundary,  long double *lowerBoundary, const int Nx, const int Ny, const int Nz, MPI_Request *reqs, MPI_Request *reqr, const int rank, const int size) {
    // Обмен нижними границами
    if (rank != 0) {
        MPI_Isend(phi, Nx * Ny, MPI_LONG_DOUBLE, rank - 1, 123, MPI_COMM_WORLD, &reqs[0]);
        MPI_Irecv(lowerBoundary, Nx * Ny, MPI_LONG_DOUBLE, rank - 1, 345, MPI_COMM_WORLD, &reqr[1]);
    }

    // Обмен вверхними границами
    if (rank != size - 1) {
        MPI_Isend(phi + Nx * Ny * (Nz - 1), Nx * Ny, MPI_LONG_DOUBLE, rank + 1, 345, MPI_COMM_WORLD, &reqs[1]);
        MPI_Irecv(upperBoundary, Nx * Ny, MPI_LONG_DOUBLE, rank + 1, 123, MPI_COMM_WORLD, &reqr[0]);
    }
}

void waitEndOfCommunication(MPI_Request *reqr, MPI_Request *reqs, const int rank, const int size) {
    if (rank != 0) {
        MPI_Wait(&reqs[0], MPI_STATUS_IGNORE);
        MPI_Wait(&reqr[1], MPI_STATUS_IGNORE);
    }

    if (rank != size - 1) {
        MPI_Wait(&reqs[1], MPI_STATUS_IGNORE);
        MPI_Wait(&reqr[0], MPI_STATUS_IGNORE);
    }
}

long double calculateApproximationAtPoint(const long double *phi, const long double *boundary, const int Nx, const int Ny, const int Nz, const int i, const int j, const int k, const long double hx, const long double hy, const long double hz) {

    // Формула для итерационного процесса метода Якоби
    const long double divider = 2.0 / (hx * hx) + 2.0 / (hy * hy) + 2.0 / (hz * hz) + lab4_consts::a;
    const long double firstTerm = (phi[i + 1 + Ny * (j + Nx * k)] + phi[i - 1 + Ny * (j + Nx * k)]) / (hx * hx); // x
    const long double secondTerm = (phi[i + Ny * (j + 1 + Nx * k)] + phi[i + Ny * (j - 1 + Nx * k)]) / (hy * hy); // y
    long double thirdTerm; // z
    const long double rhoAtPoint = 6.0 - lab4_consts::a * phi[i + Ny * (j + Nx * k)];

    if (boundary != nullptr) {
        thirdTerm = (phi[i + Ny * (j + Nx * (k == 0 ? k + 1 : k - 1))] + boundary[i + Ny * j]) / (hz * hz);
    } else {
        thirdTerm = (phi[i + Ny * (j + Nx * (k + 1))] + phi[i + Ny * (j + Nx * (k - 1))]) / (hz * hz);
    }
    return (firstTerm + secondTerm + thirdTerm - rhoAtPoint) / divider;
}

long double calculatePhiOnBoundaries(long double *oldValuesPhi, long double *newValuesPhi, const long double *upperBoundary, const long double *lowerBoundary, const int Nx, const int Ny, const int Nz, const long double hx, const long double hy, const long double hz, const int rank, const int size) {
    long double maxDif = 0.0;
    for (int j = 1; j < Ny - 1; ++j){
        for (int i = 1; i < Nx - 1; ++i){
            long double differenceOfIterations;

            // Нижняя граница
            if (rank !=  0) {
                newValuesPhi[i + Ny * j] = calculateApproximationAtPoint(oldValuesPhi, lowerBoundary, Nx, Ny, Nz, i, j, 0, hx, hy, hz);
                differenceOfIterations = fabsl(newValuesPhi[i + Ny * j] - oldValuesPhi[i + Ny * j]);
                maxDif = differenceOfIterations > maxDif ? differenceOfIterations : maxDif;
            }

            // Вверхняя граница
            if (rank != size - 1) {
                newValuesPhi[i + Ny * (j + Nx * (Nz - 1))] = calculateApproximationAtPoint(oldValuesPhi, upperBoundary, Nx, Ny, Nz, i, j, Nz - 1, hx, hy, hz);
                differenceOfIterations = fabsl(newValuesPhi[i + Ny * (j + Nx * (Nz - 1))] - oldValuesPhi[i + Ny * (j + Nx * (Nz - 1))]);
                maxDif = differenceOfIterations > maxDif ? differenceOfIterations : maxDif;
            }
        }
    }
    return maxDif;
}

long double calculatePhiInsideArea(long double *oldValuesPhi, long double *newValuesPhi, const int Nx, const int Ny, const int Nz, const long double hx, const long double hy, const long double hz) {
    long double maxDif = 0.0;
    for (int k = 1; k < Nz - 1; ++k) {
        for (int j = 1; j < Ny - 1; ++j) {
            for (int i = 1; i < Nx - 1; ++i) {
                newValuesPhi[i + Ny * (j + Nx * k)] = calculateApproximationAtPoint(oldValuesPhi, nullptr, Nx, Ny, Nz, i, j, k, hx, hy, hz);
                long double differenceOfIterations = fabsl(newValuesPhi[i + Ny * (j + Nx * k)] - oldValuesPhi[i + Ny * (j + Nx * k)]);
                maxDif = differenceOfIterations > maxDif ? differenceOfIterations : maxDif;
            }
        }
    }
    return maxDif;
}

long double calculateDelta(long double *oldValuesPhi, const int nx, const int ny, const int nz, const long double hx, const long double hy, const long double hz, const int rank) {
    long double localDelta = 0.0;
    for (int k = 0; k < nz; ++k) {
        for (int j = 0; j < ny; ++j) {
            for (int i = 0; i < nx; ++i) {
                if (fabsl(oldValuesPhi[i + ny * (j + nx * k)] - calculatePhiAtPoint(i * hx, j * hy, rank * nz * hz + k * hz)) > localDelta){
                    localDelta = fabsl(oldValuesPhi[i + ny * (j + nx * k)] - calculatePhiAtPoint(i * hx, j * hy, rank * nz * hz + k * hz));
                }
            }
        }
    }
    return localDelta;
}