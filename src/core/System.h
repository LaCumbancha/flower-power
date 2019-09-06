#ifndef FLOWER_POWER_SYSTEM_H
#define FLOWER_POWER_SYSTEM_H


#include "../menu/Menu.h"
#include "config/Config.h"
#include "job/seller/SellerJob.h"
#include "job/producer/ProducerJob.h"

class System {

public:
    explicit System() = default;

    pid_t run();
    Config* getConfig();

private:
    Config _config = Config();
    SellerJob sellerJob = SellerJob(&_config);
    ProducerJob producerJob = ProducerJob(&_config);

};


#endif //FLOWER_POWER_SYSTEM_H
