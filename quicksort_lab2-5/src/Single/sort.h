#ifndef QUICKSORT_LAB2_5_SORT_H
#define QUICKSORT_LAB2_5_SORT_H

#include <omp.h>
#include <algorithm>
#include <iostream>

template<typename T>
void quicksort(T *array, const int point1, const int point2) {
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

        if (j > 0)
            quicksort(array, point1, j);
        if (i < point2)
            quicksort(array, i, point2);
    }
}

#endif //QUICKSORT_LAB2_5_SORT_H
