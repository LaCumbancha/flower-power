//
// Created by darius on 15/9/19.
//

#ifndef FLOWER_POWER_CLIENTSIMULATOR_H
#define FLOWER_POWER_CLIENTSIMULATOR_H


#include "../utils/Pipe.h"
#include <cstdlib>

class ClientSimulator {

private:
    int _distributionCenterId;
    int _sellerId;
    int _sellerProcessId;
    Pipe * _clientPipe;

    struct BouquetRequest {
        int rosesAmount = 0;
        int tulipsAmount = 0;

        BouquetRequest(int rosesAmount, int tulipsAmount) : rosesAmount(rosesAmount), tulipsAmount(tulipsAmount) {}

        std::string to_string() {
            return "Rosas: " + std::to_string(rosesAmount) + " - Tulipanes: " + std::to_string(tulipsAmount);
        }
    };

    BouquetRequest SimulateBouquetRequest();

public:
    ClientSimulator(int distributionCenterId, int sellerId, int sellerProcessId, Pipe *clientPipe) :
            _distributionCenterId(distributionCenterId),
            _sellerId(sellerId),
            _sellerProcessId(sellerProcessId),
            _clientPipe(clientPipe) {};

    void Run();
};


#endif //FLOWER_POWER_CLIENTSIMULATOR_H
