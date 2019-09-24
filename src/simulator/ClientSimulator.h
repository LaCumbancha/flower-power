#ifndef FLOWER_POWER_CLIENTSIMULATOR_H
#define FLOWER_POWER_CLIENTSIMULATOR_H


#include <cstdlib>
#include <vector>
#include <iostream>
#include "../utils/Pipe.h"
#include "../utils/Logger.h"
#include "../core/config/data/BouquetRequest.h"


class ClientSimulator {

public:
    explicit ClientSimulator(std::string sellerId, int clients, Pipe * clientPipe);
    void run();
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
