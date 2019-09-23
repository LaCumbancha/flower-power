#ifndef FLOWER_POWER_JOB_H
#define FLOWER_POWER_JOB_H

#include <iostream>
#include "../config/Config.h"

class Job {

public:
    explicit Job() = default;
    virtual int run();
    virtual int finish();
    virtual int stopJob();

};


#endif //FLOWER_POWER_JOB_H
