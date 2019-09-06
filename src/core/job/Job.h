#ifndef FLOWER_POWER_JOB_H
#define FLOWER_POWER_JOB_H


#include <iostream>
#include "../config/Config.h"

class Job {

public:
    explicit Job(Config* config);
    virtual int run();

protected:
    Config* _config;

};


#endif //FLOWER_POWER_JOB_H
