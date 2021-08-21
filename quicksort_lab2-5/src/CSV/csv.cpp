#include "csv.h"

CSVFile::CSVFile(const string &filePath, const char &splitter): splitter(splitter), initPath(filePath) {
    data.open(filePath);
    if (!data.is_open()) {
        std::cerr << "CSV file open error!" << std::endl;
        data.close();
        exit(1);
    }
}

CSVFile::~CSVFile() {
    data.close();
}

void CSVFile::addNote(const string &column1, const string &column2) {
    data << column1 << splitter << column2 << std::endl;
}

void CSVFile::reset() {
    data.close();
    data.open(initPath, std::ios::out);
}
