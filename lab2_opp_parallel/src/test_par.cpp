#include <iostream>
#include <omp.h>
#include "matrix.h"
#include "vector.h"
#include "file.h"

int main() {
    const int matrixSize = 500;
    float *matrix = createFloatMatrix(matrixSize);

    FILE *outFile = fopen("test.txt", "w");
    matrix = setMatrixFloatValues(matrix, matrixSize, 2.f);
    printMatrix(matrix, matrixSize, outFile);

    fclose(outFile);
    free(matrix);
    return 0;
}