#include "mathsolver.h"

float *getSolveFloatEquationsSystem(float *matrix, float *vectorB, const int &matrixSize) {
    const float tao = -1e-2;
    const float epslon = 1e-4;
    const float exitConditionDenominator = getVectorNormal(vectorB, matrixSize);

    // Result vector X
    float *vectorX = createFloatVector(matrixSize);

    // Previous vector X (to find a solution)
    float *tempVector = createFloatVector(matrixSize);
    tempVector = setVectorFloatValues(tempVector, matrixSize, 0.0f);

    float multiplyResult = 0;
    float exitCondition = 0;
    float *exitConditionVector = createFloatVector(matrixSize);

    do {
        // Calculate the new vector X
        for (int i = 0; i < matrixSize; ++i) {
            multiplyResult = 0;
            for (int j = 0; j < matrixSize; ++j) {
                multiplyResult += matrix[i * matrixSize + j] * tempVector[j];
            }
            vectorX[i] = tempVector[i] - tao * (multiplyResult - vectorB[i]);
        }

        // Calculate the vector norm and check the exit condition
        exitCondition = 0;
        exitConditionVector = setVectorFloatValues(exitConditionVector, matrixSize, 0.0f);
        for (int i = 0; i < matrixSize; ++i) {
            multiplyResult = 0;
            for (int j = 0; j < matrixSize; ++j) {
                multiplyResult += matrix[i * matrixSize + j] * vectorX[j];
            }
            exitConditionVector[i] = multiplyResult - vectorB[i];
        }

        for (int i = 0; i < matrixSize; ++i) {
            exitCondition += exitConditionVector[i] * exitConditionVector[i];
        }
        exitCondition = sqrtf(exitCondition) / exitConditionDenominator;

        if (exitCondition < epslon)
            break;
        std::memcpy(tempVector, vectorX, sizeof(float) * matrixSize);
        printf("%f\n", exitCondition);

    } while (true);

    free(tempVector);
    free(exitConditionVector);
    return vectorX;
}
