#ifndef FLOWER_POWER_CSVREADER_H
#define FLOWER_POWER_CSVREADER_H


#include <iostream>
#include <vector>

using namespace std;

class CSVReader {

public:
    explicit CSVReader(string filename, char separator = ',');
    vector<vector<string>> getData();

private:
    string _filename;
    char _separator;

};


#endif //FLOWER_POWER_CSVREADER_H
