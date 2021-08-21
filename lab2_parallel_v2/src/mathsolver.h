#ifndef LAB2_OPP_MATHSOLVER_H
#define LAB2_OPP_MATHSOLVER_H

#include "matrix.h"
#include "vector.h"
#include <cmath>

namespace lab2_consts {
    const float epslon = 1e-5;
}

float getTerminalCondition(float *matrix, float *vectorX, float *vectorB, const int &matrixSize);
float *getSolveFloatEquationsSystem(float *matrix, float *vectorB, const int &matrixSize);

#endif //LAB2_OPP_MATHSOLVER_H
