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


class DistributionCenter : public Job {

public:
    explicit DistributionCenter(Config *config, int id);
    pid_t run() override;
    void finish();

    const std::vector<Pipe *> &getDistributionPipes() const;
    void setDistributionPipes(const std::vector<Pipe *> &distributionPipes);

private:
    int _id;
    pid_t _classifierPID;
    pid_t _stockManagerPID;
    Config* _config;
    Pipe* _producersPipe;
    Pipe* _requestsPipe;
    std::vector<Pipe*> _distributionPipes;
    std::vector<pid_t> _producersPIDs;
    std::vector<pid_t> _sellersPIDs;

    void closePipes();
};


#endif //FLOWER_POWER_DISTRIBUTIONCENTER_H
