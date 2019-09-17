#ifndef FLOWER_POWER_CLIENTSIMULATOR_H
#define FLOWER_POWER_CLIENTSIMULATOR_H


#include <cstdlib>
#include <vector>
#include <iostream>
#include "../utils/Pipe.h"
#include "../core/config/data/BouquetRequest.h"


class ClientSimulator {

public:
    explicit ClientSimulator(int centerId, int sellerId, Pipe *clientPipe);
    void run();

private:
    int _centerId;
    int _sellerId;
    Pipe * _clientPipe;

    BouquetRequest simulateBouquetRequest();

};


#endif //FLOWER_POWER_CLIENTSIMULATOR_H
