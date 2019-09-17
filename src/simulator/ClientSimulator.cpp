#include "ClientSimulator.h"

ClientSimulator::ClientSimulator(int centerId, int sellerId, Pipe *clientPipe) {
    this->_centerId = centerId;
    this->_sellerId = sellerId;
    this->_clientPipe = clientPipe;

    // Initializing random number generator
    srand(time(NULL) * getpid());
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
