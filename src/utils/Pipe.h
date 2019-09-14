#ifndef FLOWER_POWER_PIPE_H
#define FLOWER_POWER_PIPE_H


#include <zconf.h>
#include <cstdlib>
#include <string>

class Pipe {

public:
    explicit Pipe();
    void setReadMode();
    void setWriteMode();
    ssize_t write(const std::string& data);
    ssize_t read(std::string& data, int* status);
    ~Pipe();

private:
    bool _readMode = true;
    bool _writeMode = true;
    int _readFileDescriptor;
    int _writeFileDescriptor;

};


#endif //FLOWER_POWER_PIPE_H
