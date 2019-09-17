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
    explicit SellerJob(int center, const Seller& sellerData, Pipe* requestPipe, Pipe* distributionPipe);
    int run() override;
    int finish() override;

private:
    pid_t _clientSimulatorPID{};
    int _center;
    int _sellerId;
    std::string _sellerName;
    Pipe* _clientPipe{};
    Pipe* _requestPipe;
    Pipe* _distributionPipe;

    int listenRequests();
    void handleRequest(BouquetRequest bouquetRequest);

    // TODO: Delete after implementing shared memory.
    int _rosesStock;
    int _tulipsStock;

};


#endif //FLOWER_POWER_SELLERJOB_H
