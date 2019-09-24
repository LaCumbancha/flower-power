#ifndef FLOWER_POWER_SELLERJOB_H
#define FLOWER_POWER_SELLERJOB_H


#include <iostream>
#include <sys/wait.h>
#include <utility>
#include "../Job.h"
#include "../../config/Config.h"
#include "../../config/data/SellerRequest.h"
#include "../../config/data/ClassifierBox.h"
#include "../../../simulator/ClientSimulator.h"
#include "../../../utils/StatsCenter.h"
#include "../../../utils/ContextStatus.h"
#include "../../../utils/ProcessKiller.h"
#include "../../../utils/Pipe.h"
#include "../../../utils/Logger.h"
#include "../../../utils/signals/SignalHandler.h"
#include "../../../utils/signals/StopHandler.h"


class SellerJob : public Job {

public:
    explicit SellerJob(std::string sellerId, int clients, Pipe *requestPipe, Pipe *distributionPipe);
    int run() override;

    int stopJob() override;
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

    int _remitoNumber = 0;
    void writeRemito(BouquetRequest request);

    std::vector<Flower> _rosesStock;
    std::vector<Flower> _tulipsStock;

    void initializeStatus(const string& basicString);

    void loadPreviousState(const string& previousState);
};


#endif //FLOWER_POWER_SELLERJOB_H
