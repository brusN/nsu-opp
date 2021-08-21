#ifndef LAB4_OPP_V2_MPISOURCE_H
#define LAB4_OPP_V2_MPISOURCE_H

#include <mpi.h>
#include <cmath>
#include "source.h"

void calculatingSizeOfCell(const int Nx, const int Ny, const int Nz, long double &hx, long double &hy, long double &hz);

void calculateValueOnBoundaries(long double *oldValuesPhi, long double *newValuesPhi, const int Nx, const int Ny, const int Nz, const long double hx, const long double hy, const long double hz, const int rank, const int size);

void sendBoundaries(const long double *phi, long double *upperBoundary, long double *lowerBoundary, const int Nx, const int Ny, const int Nz, MPI_Request *reqs, MPI_Request *reqr, const int rank, const int size);

void waitEndOfCommunication(MPI_Request *reqr, MPI_Request *reqs, const int rank, const int size);

long double calculateApproximationAtPoint(const long double *phi, const long double *boundary, const int Nx, const int Ny, const int Nz, const int i, const int j, const int k, const long double hx, const long double hy, const long double hz);

long double calculatePhiOnBoundaries(long double *oldValuesPhi, long double *newValuesPhi, const long double *upperBoundary, const long double *lowerBoundary, const int Nx, const int Ny, const int Nz, const long double hx, const long double hy, const long double hz, const int rank, const int size);

long double calculatePhiInsideArea(long double *oldValuesPhi, long double *newValuesPhi, const int Nx, const int Ny, const int Nz, const long double hx, const long double hy, const long double hz);

long double calculateDelta(long double *oldValuesPhi, const int nx, const int ny, const int nz, const long double hx, const long double hy, const long double hz, const int rank);


#endif //LAB4_OPP_V2_MPISOURCE_H
