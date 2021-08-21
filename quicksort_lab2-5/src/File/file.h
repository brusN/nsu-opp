#ifndef QUICKSORT_LAB2_5_FILE_H
#define QUICKSORT_LAB2_5_FILE_H

#include <iostream>

FILE *safeFileOpen(const char *path, const char *args);
void safeFileRead(void *ptr, size_t partSize, size_t countParts, FILE* file);

#endif //QUICKSORT_LAB2_5_FILE_H
