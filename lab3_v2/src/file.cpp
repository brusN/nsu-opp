#include "file.h"

void safeCloseFile(FILE* file) {
    if (fclose(file) != 0) {
        perror("Error file closing: ");
        exit(1);
    }
}

FILE *safeOpenFile(const char *filePath, const char *arg) {
    FILE *file = fopen(filePath, arg);
    if (file == nullptr) {
        printf("Error while opening the file with name %s\n", filePath);
        perror("Description: ");
        exit(1);
    }
    return file;
}
