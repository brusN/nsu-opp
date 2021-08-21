#include <omp.h>
#include <iostream>

int main() {
    const int arraySize = 2500;
    int array[arraySize];
    for (int i = 0; i < arraySize; ++i)
        array[i] = 1;
    int kek = 0;
    #pragma omp parallel
    {
        ++
        #pragma omp barrier
        #pragma omp single
        std::cout << "kek1\n";
    }
    return 0;
}