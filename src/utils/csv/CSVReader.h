#ifndef FLOWER_POWER_CSVREADER_H
#define FLOWER_POWER_CSVREADER_H


#include <iostream>
#include <vector>

using namespace std;

class CSVReader {

public:
    explicit CSVReader(std::string filename, char separator = ',');
    std::vector<std::vector<std::string>> getData();
    std::vector<std::string> getDataByLines();

private:
    std::string _filename;
    char _separator;

};


#endif //FLOWER_POWER_CSVREADER_H
