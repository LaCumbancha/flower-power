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

Pipe::~Pipe() {
    if (this->_writeMode) {
        close(this->_writeFileDescriptor);
    }

    if (this->_readMode) {
        close(this->_readFileDescriptor);
    }
}

