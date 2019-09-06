#ifndef FLOWER_POWER_SYSTEM_H
#define FLOWER_POWER_SYSTEM_H


#include "../menu/Menu.h"
#include "producer/ProducerJob.h"
#include "seller/SellerJob.h"

class System {

public:
    int run();

private:
    ProducerJob producerJob = ProducerJob();
    SellerJob sellerJob = SellerJob();

};


#endif //FLOWER_POWER_SYSTEM_H
