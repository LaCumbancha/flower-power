#ifndef FLOWER_POWER_DISTRIBUTIONCENTER_H
#define FLOWER_POWER_DISTRIBUTIONCENTER_H

#include <zconf.h>
#include <iostream>
#include <sys/wait.h>
#include "../utils/Pipe.h"
#include "job/producer/ProducerJob.h"


class DistributionCenter : public Job {

public:
    explicit DistributionCenter(Config *config, int id);
    pid_t run() override;

private:
    int _id;
    Config* _config;
    Pipe* _producersPipe;
    Pipe* _requestsPipe;
    std::vector<Pipe*> _distributionPipes;

};


#endif //FLOWER_POWER_DISTRIBUTIONCENTER_H
