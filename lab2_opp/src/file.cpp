#include "file.h"

void safeCloseFile(FILE* file) {
    if (fclose(file) != 0) {
        perror("Error file closing: ");
        exit(1);
    }
}

FILE *safeOpenFile(char *filePath, char *arg) {
    FILE *file = fopen(filePath, arg);
    if (file == nullptr) {
        perror("Erromr file opening: ");
        exit(1);
    }
    return file;
}
