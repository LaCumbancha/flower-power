#ifndef FLOWER_POWER_PRODUCERJOB_H
#define FLOWER_POWER_PRODUCERJOB_H


#include <iostream>
#include "../../config/Config.h"
#include "../Job.h"

using namespace std;

class ProducerJob : public Job {

public:
    explicit ProducerJob(Config* config);
    int run();

};


#endif //FLOWER_POWER_PRODUCERJOB_H
