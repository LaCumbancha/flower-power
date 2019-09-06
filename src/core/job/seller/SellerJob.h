#ifndef FLOWER_POWER_SELLERJOB_H
#define FLOWER_POWER_SELLERJOB_H


#include <iostream>
#include "../../config/Config.h"
#include "../Job.h"

using namespace std;

class SellerJob : public Job {

public:
    explicit SellerJob(Config* config);
    int run();

};


#endif //FLOWER_POWER_SELLERJOB_H
