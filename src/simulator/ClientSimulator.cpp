#include "ClientSimulator.h"

ClientSimulator::ClientSimulator(int distributionCenterId, int sellerId, int sellerProcessId, Pipe *clientPipe) {
    this->_centerId = distributionCenterId;
    this->_sellerId = sellerId;
    this->_sellerPID = sellerProcessId;
    this->_clientPipe = clientPipe;
}

void ClientSimulator::run() {

    for (int i = 0; i < 10; i++) {
        BouquetRequest request = simulateBouquetRequest();
        this->_clientPipe->write(request.serialize());
    }
}

BouquetRequest ClientSimulator::simulateBouquetRequest() {
    int rosesAmount = rand() % 10;
    int tulipsAmount = rand() % 10;
    BouquetRequest request(rosesAmount, tulipsAmount);
    return request;
}
