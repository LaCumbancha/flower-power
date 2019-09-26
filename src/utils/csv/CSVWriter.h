#ifndef FLOWER_POWER_CSVWRITER_H
#define FLOWER_POWER_CSVWRITER_H


#include <iostream>
#include <vector>
#include <fstream>
#include <dirent.h>
#include <sys/stat.h>


class CSVWriter {

public:
    explicit CSVWriter(std::string filename, char separator = ',');
    void saveData(const std::string& data);
    void clearFile();

private:
    std::string _filename;
    char _separator;

    static void createFolder(const std::string& string);
};


#endif //FLOWER_POWER_CSVWRITER_H
