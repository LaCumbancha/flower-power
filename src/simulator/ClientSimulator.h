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
    explicit ClientSimulator(int centerId, int sellerId, int clients, Pipe *clientPipe);
    void run();

private:
    int _centerId;
    int _sellerId;
    int _clients;
    Pipe * _clientPipe;

    static BouquetRequest simulateBouquetRequest();

};


#endif //FLOWER_POWER_CLIENTSIMULATOR_H
