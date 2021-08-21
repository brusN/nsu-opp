#include <iostream>
#include "test.h"

/*
 * argv[1] - размер массива
 * argv[2] - путь к бинарному файлу со сгенирированным массивом
 */

int main(int argc, char **argv) {
    FILE *outFile = fopen(argv[2], "wb");
    if (outFile == NULL) {
        std::cerr << "Can't create the file!";
        fclose(outFile);
        exit(1);
    }

    const int arraySize = strtol(argv[1], nullptr, 10);
    auto array = generateRandomArray<int>(arraySize, INT32_MAX, 0);
    fwrite(array, sizeof(int), arraySize, outFile);
    fclose(outFile);

    std::cout << "Array has generated!" << std::endl;
    return 0;
}