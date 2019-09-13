#ifndef FLOWER_POWER_JOB_H
#define FLOWER_POWER_JOB_H

#include <iostream>
#include "../config/Config.h"

class Job {

public:
    explicit Job() = default;
    virtual int run();

};


#endif //FLOWER_POWER_JOB_H
