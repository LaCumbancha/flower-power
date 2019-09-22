#include <iostream>
#include <cstring>
#include "Pipe.h"

Pipe::Pipe() {
    int fileDescriptors[2];

    if (pipe(fileDescriptors) < EXIT_SUCCESS) {
        exit(EXIT_FAILURE);
    }

    this->_readFileDescriptor = fileDescriptors[0];
    this->_writeFileDescriptor = fileDescriptors[1];
}

void Pipe::setReadMode() {
    this->_readMode = true;
    ::close(this->_writeFileDescriptor);
}

void Pipe::setWriteMode() {
    this->_writeMode = true;
    ::close(this->_readFileDescriptor);
}

ssize_t Pipe::write(const std::string& data) {
    std::string stream;
    stream += "|";
    stream += std::to_string(data.size());
    stream += "|";
    stream += data;
    return ::write(this->_writeFileDescriptor, stream.c_str(), stream.length() + 1);
}

// Expected protocol: |<size in chars>|<element 1>|<element 2>|...|<element n>|
ssize_t Pipe::read(std::string& data, int* status) {
    int bufferSize = 0;
    char character;
    ssize_t readReturn = ::read(this->_readFileDescriptor, &character, 1);

    if (readReturn <= 0) {
        *status = EXIT_FAILURE;
        return readReturn;
    }

    if (character == '|') {

        std::string size;
        while(readReturn += ::read(this->_readFileDescriptor, &character, 1)) {
            if (character != '|') {
                size += character;
            } else {
                bufferSize = std::stoi(size);
                break;
            }
        }

        char* buffer;
        buffer = new char[bufferSize + 1];
        memset(buffer, '\0', bufferSize + 1);
        readReturn += ::read(this->_readFileDescriptor, buffer, bufferSize);

        data = buffer;
        delete[] buffer;
        *status = EXIT_SUCCESS;
        return readReturn;

    } else {
        *status = EXIT_FAILURE;
        return readReturn;
    }

}

Pipe::~Pipe() {
    if (this->_writeMode) {
        ::close(this->_writeFileDescriptor);
    }

    if (this->_readMode) {
        ::close(this->_readFileDescriptor);
    }
}
