#ifndef FLOWER_POWER_DISTRIBUTIONCENTER_H
#define FLOWER_POWER_DISTRIBUTIONCENTER_H

#include <zconf.h>
#include <iostream>
#include <sys/wait.h>
#include "../utils/Pipe.h"
#include "job/producer/ProducerJob.h"


class DistributionCenter : public Job {

public:
    explicit DistributionCenter(Config *config);
    pid_t run() override;

private:
    Config* _config;
    Pipe* _producersPipe;

};


#endif //FLOWER_POWER_DISTRIBUTIONCENTER_H
