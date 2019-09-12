#ifndef FLOWER_POWER_SELLERJOB_H
#define FLOWER_POWER_SELLERJOB_H


#include <iostream>
#include "../../config/Config.h"
#include "../Job.h"

class SellerJob : public Job {

public:
    explicit SellerJob();
    int run() override;

};


#endif //FLOWER_POWER_SELLERJOB_H
