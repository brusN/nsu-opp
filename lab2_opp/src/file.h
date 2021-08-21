#ifndef LAB2_OPP_FILE_H
#define LAB2_OPP_FILE_H

#include <iostream>

void safeCloseFile(FILE* file);
FILE* safeOpenFile(char* filePath, char* arg);

#endif //LAB2_OPP_FILE_H
