#include <iostream>
#include <mpi.h>
#include "source.h"

int main(int argc, char **argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int rows[4] = {4, 5, 6, 7};
    int num = 0;
    for (int i = 0; i < rows[rank]; ++i) {
        ++num;
    }
    printf("[Before] num %d from rank %d\n", num, rank);
    MPI_Allreduce(MPI_IN_PLACE, &num, 1, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);
    printf("[After] num %d from rank %d\n", num, rank);
    MPI_Finalize();
    return 0;
}