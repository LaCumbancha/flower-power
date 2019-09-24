#ifndef FLOWER_POWER_CSVWRITER_H
#define FLOWER_POWER_CSVWRITER_H


#include <iostream>
#include <vector>
#include <fstream>


class CSVWriter {

public:
    explicit CSVWriter(std::string filename, char separator = ',');
    void saveData(const std::string& data);
    void clearFile();

private:
    std::string _filename;
    char _separator;

};


#endif //FLOWER_POWER_CSVWRITER_H
