#include "file.h"

FILE *safeFileOpen(const char *path, const char *args) {
    FILE *newFile = fopen(path, args);
    if (path == nullptr) {
        std::cerr << "File hasn't opened! Try again";
        fclose(newFile);
        exit(1);
    }
    return newFile;
}

void safeFileRead(void *ptr, size_t partSize, size_t countParts, FILE* file) {
    if (fread(ptr, partSize, countParts, file) == 0) {
        std::cerr << "fread() error!" << std::endl;
        fclose(file);
        exit(1);
    }
}
