#ifndef FLOWER_POWER_PIPE_H
#define FLOWER_POWER_PIPE_H


#include <zconf.h>
#include <cstdlib>

class Pipe {

public:
    explicit Pipe();
    void setReadMode();
    void setWriteMode();
    ~Pipe();

private:
    bool _readMode = true;
    bool _writeMode = true;
    int _readFileDescriptor;
    int _writeFileDescriptor;

};


#endif //FLOWER_POWER_PIPE_H
