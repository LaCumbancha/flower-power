#ifndef FLOWER_POWER_SELLERJOB_H
#define FLOWER_POWER_SELLERJOB_H


#include <iostream>
#include <sys/wait.h>
#include <utility>
#include "../Job.h"
#include "../../config/Config.h"
#include "../../../utils/StatsCenter.h"
#include "../../../utils/ContextStatus.h"
#include "../../../utils/ProcessKiller.h"
#include "../../../utils/Pipe.h"
#include "../../../utils/Logger.h"
#include "../../../simulator/ClientSimulator.h"
#include "../../config/data/SellerRequest.h"
#include "../../config/data/ClassifierBox.h"


class SellerJob : public Job {

public:
    explicit SellerJob(std::string sellerId, int clients, Pipe *requestPipe, Pipe *distributionPipe);
    int run() override;
    ~SellerJob();
private:
    int _clients;
    std::string _sellerId;
    Pipe* _clientPipe{};
    Pipe* _requestPipe;
    Pipe* _distributionPipe;
    pid_t _clientSimulatorPID{};
    bool _distributionPipeIsOpen = true;

    int listenRequests();
    int finish() override;
    void handleRequest(BouquetRequest bouquetRequest);
    void resupply(BouquetRequest request);
    std::string contextState();

    std::vector<Flower> _rosesStock;
    std::vector<Flower> _tulipsStock;

    __sighandler_t handler();
};


#endif //FLOWER_POWER_SELLERJOB_H
