#ifndef FLOWER_POWER_DISTRIBUTIONCENTER_H
#define FLOWER_POWER_DISTRIBUTIONCENTER_H

#include <zconf.h>
#include <iostream>
#include <sys/wait.h>
#include "../utils/Pipe.h"
#include "../utils/Logger.h"
#include "job/seller/SellerJob.h"
#include "job/producer/ProducerJob.h"
#include "job/classifier/ClassifierJob.h"
#include "job/distributor/DistributorJob.h"
#include "../utils/ProcessKiller.h"


class DistributionCenter : public Job {

public:
    explicit DistributionCenter(Config *config, int id);
    int run() override;
    int finish() override;

private:
    int _id;
    pid_t _classifierPID{};
    pid_t _stockManagerPID{};
    Config* _config;
    Pipe* _producersPipe;
    Pipe* _requestsPipe;
    Pipe* _innerPipe;
    std::vector<pid_t> _producersPIDs;
    std::map<string, Pipe *> _distributionPipes;
    std::vector<pid_t> _sellersPIDs;
    void closePipes();
};


#endif //FLOWER_POWER_DISTRIBUTIONCENTER_H
