#include <iostream>
#include "../File/file.h"

/*
 * argv[1] - размер массива
 * argv[2] - путь к бинарному файлу со проверяемым массивом
 */

int main(int argc, char **argv) {
    const int arraySize = strtol(argv[1], nullptr, 10);

    // Считывание массива
    FILE *checkFile = safeFileOpen(argv[2], "rb");
    int *array = (int*)malloc(sizeof(int) * arraySize);
    safeFileRead(array, sizeof(int), arraySize, checkFile);
    fclose(checkFile);

    // Проверка массива
    int temp = array[0];
    for (int i = 1; i < arraySize; ++i) {
        if (temp > array[i]) {
            std::cout << "Array has sorted incorrectly!" << std::endl;
            exit(0);
        }
        temp = array[i];
    }

    std::cout << "Array has sorted correctly!" << std::endl;
    return 0;
}