#ifndef FLOWER_POWER_CLIENTSIMULATOR_H
#define FLOWER_POWER_CLIENTSIMULATOR_H


#include <cstdlib>
#include <vector>
#include <iostream>
#include <utility>
#include "../utils/Pipe.h"
#include "../utils/Logger.h"
#include "../core/config/data/BouquetRequest.h"
#include "../core/job/Job.h"
#include "../utils/signals/SignalHandler.h"
#include "../utils/signals/StopHandler.h"


class ClientSimulator : public Job {

public:
    explicit ClientSimulator(std::string sellerId, int clients, Pipe * clientPipe);
    int run() override;
    int stopJob() override;
    ~ClientSimulator();

private:
    int _clients;
    std::string _sellerId;
    Pipe * _clientPipe;
    std::vector<BouquetRequest> _onlineSales;
    static BouquetRequest simulateBouquetRequest();

    void loadOnlineSales();
};


#endif //FLOWER_POWER_CLIENTSIMULATOR_H
