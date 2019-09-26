#include "CSVWriter.h"
#include "../Logger.h"

CSVWriter::CSVWriter(std::string filename, char separator) {
    this->_filename = std::move(filename);
    this->_separator = separator;
    CSVWriter::createFolder(this->_filename);
}

void CSVWriter::createFolder(const std::string& file) {
    auto folder = file.substr(0, file.find_last_of('/')).c_str();
    std::cerr << "Folder: " << folder << std::endl;
    DIR* directory = opendir(folder);
    if (directory == nullptr && mkdir(folder, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != EXIT_SUCCESS) {
        Logger::error("Couldn't create tmp folder!");
    } else {
        closedir(directory);
    }
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
