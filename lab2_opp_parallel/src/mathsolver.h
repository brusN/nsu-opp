#ifndef LAB2_OPP_MATHSOLVER_H
#define LAB2_OPP_MATHSOLVER_H

#include "matrix.h"
#include "vector.h"
#include <cmath>

#define NUM_THREADS 8

namespace lab2_consts {
    const float tao = -1e-2;
    const float epslon = 1e-2;
}

float getTerminalCondition(float *matrix, float *vectorX, float *vectorB, const int &matrixSize);
float *getSolveFloatEquationsSystem(float *matrix, float *vectorB, const int &matrixSize, const int &numThreads);

#endif //LAB2_OPP_MATHSOLVER_H
