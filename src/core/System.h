#ifndef FLOWER_POWER_SYSTEM_H
#define FLOWER_POWER_SYSTEM_H


#include "../menu/Menu.h"
#include "producer/ProducerJob.h"
#include "seller/SellerJob.h"
#include "config/Config.h"

class System {

public:
    int run();
    Config* getConfig();

private:
    ProducerJob producerJob = ProducerJob();
    SellerJob sellerJob = SellerJob();
    Config _config = Config();

};


#endif //FLOWER_POWER_SYSTEM_H
