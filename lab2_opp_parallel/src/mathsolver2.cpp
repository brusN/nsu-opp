#include "mathsolver.h"

float *getSolveFloatEquationsSystem(float *matrix, float *vectorB, const int &matrixSize, const int &numThreads) {
    const float tao = lab2_consts::tao;
    const float epslon = lab2_consts::epslon;
    const float exitConditionDenominator = getVectorNormal(vectorB, matrixSize);

    // Result vector X
    float *vectorX = createFloatVector(matrixSize);
    vectorX = setVectorFloatValues(vectorX, matrixSize, 0.0f);

    float multiplyResult = 0;
    float exitCondition = 0;
    float *exitConditionVector = createFloatVector(matrixSize);
    bool hasNextIteration = true;

    size_t iterCount = 0;

    #pragma omp parallel private(multiplyResult) num_threads(numThreads)
    do {
        // Calculate the new vector X
        #pragma omp single
        exitCondition = 0;

        #pragma omp for reduction(+: exitCondition)
        for (int i = 0; i < matrixSize; ++i) {
            multiplyResult = 0;
            for (int j = 0; j < matrixSize; ++j)
                multiplyResult += matrix[i * matrixSize + j] * vectorX[j];
            // Считаем следующее приближение
            vectorX[i] = vectorX[i] - tao * (multiplyResult - vectorB[i]);

            // Проверяем выполняемость условия выхода
            // Цикл сделает на одну итерацию больше
            // Но при этом выигрыш в том, что не надо расчитывать вектор нормали ещё раз для нового вектора
            exitConditionVector[i] = multiplyResult - vectorB[i];
            exitCondition += exitConditionVector[i] * exitConditionVector[i];
        }

        #pragma omp single
        {
            exitCondition = sqrtf(exitCondition) / exitConditionDenominator;
            if (exitCondition < epslon)
                hasNextIteration = false;
            //printf("%f\n", exitCondition);
            ++iterCount;
        }
    } while (hasNextIteration);

    std::cout << "Кол-во итераций: " << iterCount << std::endl;

    free(exitConditionVector);
    return vectorX;
}
