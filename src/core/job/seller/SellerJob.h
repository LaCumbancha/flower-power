#ifndef FLOWER_POWER_SELLERJOB_H
#define FLOWER_POWER_SELLERJOB_H


#include <iostream>
#include <sys/wait.h>
#include "../Job.h"
#include "../../config/Config.h"
#include "../../../utils/Pipe.h"
#include "../../../utils/Logger.h"
#include "../../../simulator/ClientSimulator.h"


class SellerJob : public Job {

public:
    explicit SellerJob(int center, int sellerId, int clients, Pipe *requestPipe, Pipe *distributionPipe);
    int run() override;
    int finish() override;

private:
    int _center;
    int _clients;
    int _sellerId;
    Pipe* _clientPipe{};
    Pipe* _requestPipe;
    Pipe* _distributionPipe;
    pid_t _clientSimulatorPID{};

    int listenRequests();
    void handleRequest(BouquetRequest bouquetRequest);

    // TODO: Delete after implementing shared memory.
    int _rosesStock = 1000;
    int _tulipsStock = 1000;

};


#endif //FLOWER_POWER_SELLERJOB_H
