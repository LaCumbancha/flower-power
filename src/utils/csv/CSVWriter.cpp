#include "CSVWriter.h"

CSVWriter::CSVWriter(std::string filename, char separator) {
    this->_filename = std::move(filename);
    this->_separator = separator;
}

void CSVWriter::saveData(const std::string& data) {

    // Creating CSV file file.
    std::ofstream outfile;
    outfile.open(this->_filename, std::ios::app);

    // Saving data.
    outfile << data << std::endl;

    // Closing file.
    outfile.close();

}

void CSVWriter::clearFile() {
    std::ofstream outfile;
    outfile.open(this->_filename, std::ios::out);
    outfile.close();
}
