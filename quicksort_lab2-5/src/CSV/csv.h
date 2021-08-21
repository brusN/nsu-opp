#ifndef QUICKSORT_LAB2_5_CSV_H
#define QUICKSORT_LAB2_5_CSV_H

#include <iostream>
#include <fstream>
#include <string>

using std::string;

class CSVFile {
private:
    string initPath;
    std::ofstream data;
    char splitter;
public:
    explicit CSVFile(const string &filePath, const char &splitter);
    ~CSVFile();
    void addNote(const string &column1, const string &column2);
    void reset();
};

#endif //QUICKSORT_LAB2_5_CSV_H
