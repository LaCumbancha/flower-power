#include "CSVReader.h"

#include <fstream>
#include <vector>
#include <string>

CSVReader::CSVReader(std::string filename, char separator) {
    this->_filename = std::move(filename);
    this->_separator = separator;
}

std::vector<std::vector<std::string>> CSVReader::getData() {

    std::ifstream file(this->_filename);
    std::vector<std::vector<std::string>> data;
    std::string line, buffer;

    // Iterate through each line and split the content using the separator.
    while (getline(file, line)) {
        std::vector<std::string> record;

        for (auto character : line) {

            if (character != this->_separator) {
                buffer += character;
            } else {
                if (!buffer.empty()) {
                    record.push_back(buffer);
                    buffer = "";
                }
            }

        }

        if (!buffer.empty()) {
            record.push_back(buffer);
            buffer = "";
        }

        data.push_back(record);
    }

    // Close the File
    file.close();

    return data;
}

std::vector<std::string> CSVReader::getDataByLines() {

    std::ifstream file(this->_filename);
    std::vector<std::string> data;
    std::string line;

    // Save each line.
    while (getline(file, line)) {
        data.push_back(line);
    }

    // Close the File
    file.close();

    return data;
}

