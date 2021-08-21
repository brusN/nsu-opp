#include <iostream>
#include <ctime>
#include "../src/file.h"
#include "../src/source.h"

int main(int argc, char **argv) {
    if (argc != 4) {
        std::cerr << "Wrong number of arguments!" << std::endl;
        exit(1);
    }
    srand(time(nullptr));

    int sizeN = static_cast<int>(strtol(argv[1], nullptr, 10));
    int sizeM = static_cast<int>(strtol(argv[2], nullptr, 10));
    std::string outFileName = argv[3];

    float *matrix = initFloatMatrix(sizeN, sizeM);
    for (int i = 0; i < sizeN; ++i) {
        for (int j = 0; j < sizeM; ++j) {
            matrix[i * sizeM + j] = 5.0f;
        }
    }

    FILE *outputFile = safeOpenFile(outFileName.c_str(), "wb");
    fwrite(matrix, sizeof(float), sizeN * sizeM, outputFile);
    safeCloseFile(outputFile);
    free(matrix);

    std::cout << "Matrix size of [" << sizeN << " x " << sizeN << "] has generates successfully!";
    return 0;
}