#ifndef QUICKSORT_LAB2_5_SORT_PARALLEL_H
#define QUICKSORT_LAB2_5_SORT_PARALLEL_H

#include <omp.h>
#include <algorithm>
#include <iostream>

const int exitConditionSize = 500000;

template<typename T>
void quicksort_parallel(T *array, const int point1, const int point2) {
    if (point1 < point2) {
        T i = point1;
        T j = point2;
        T pivot = array[(i + j) / 2];

        do {
            while (array[i] < pivot)
                ++i;
            while (array[j] > pivot)
                --j;
            if (i <= j)
                std::swap(array[i++], array[j--]);
        } while (i <= j);

        #pragma omp task if(point2 - point1 >= exitConditionSize) shared(array)
        {
            if (j > point1) {
                quicksort_parallel(array, point1, j);
            }
        }
        #pragma omp task if(point2 - point1 >= exitConditionSize) shared(array)
        {
            if (i < point2) {
                quicksort_parallel(array, i, point2);
            }
        }
        //#pragma omp taskwait
    }
}

/*!l
* quickSort - параллельная реализация алгоритма быстрой сортировки
* @param a сортируемый массив
* @param n индекс последнего элемента массива (не размер массива!)
*/
template <class T>
void quickSort(T* a, const long n) {
    long i = 0, j = n-1;
    T pivot = a[n / 2];
    do {
        while (a[i] < pivot) {
            i++;
        }
        while (a[j] > pivot) {
            j--;
        }
        if (i <= j) {
            std::swap(a[i], a[j]);
            i++; j--;
        }
    } while (i <= j);
    #pragma omp task shared(a)
    {
        if (j > 0) {
            quickSort(a, j+1);
        }
    }
    #pragma omp task shared(a)
    {
        if (n > i) {
            quickSort(a + i, n - i);
        }
    }
    #pragma omp taskwait
}
#endif //QUICKSORT_LAB2_5_SORT_PARALLEL_H
