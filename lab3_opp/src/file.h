#ifndef LAB1_OPP_FILE_H
#define LAB1_OPP_FILE_H

#include <iostream>

FILE *safeOpenFile(const char *filePath, const char *arg);
void safeCloseFile(FILE *file);

#endif //LAB1_OPP_FILE_H
