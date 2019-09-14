#ifndef FLOWER_POWER_PIPE_H
#define FLOWER_POWER_PIPE_H


#include <zconf.h>
#include <cstdlib>

class Pipe {

public:
    explicit Pipe();
    void setReadMode();
    void setWriteMode();
    ssize_t write(const void* data, int size);
    ssize_t read(void* buffer, int size);
    ~Pipe();

private:
    bool _readMode = true;
    bool _writeMode = true;
    int _readFileDescriptor;
    int _writeFileDescriptor;

};


#endif //FLOWER_POWER_PIPE_H
