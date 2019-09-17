#include "ClientSimulator.h"


ClientSimulator::ClientSimulator(int centerId, int sellerId, int clients, Pipe *clientPipe) {
    this->_centerId = centerId;
    this->_sellerId = sellerId;
    this->_clients = clients;
    this->_clientPipe = clientPipe;

    // Initializing random number generator
    srand(time(NULL) * getpid());
}

void ClientSimulator::run() {

    for (int client = 1; client <= this->_clients; client++) {
        BouquetRequest request = simulateBouquetRequest();

        Logger::info("Generating request for Client #" + std::to_string(client) + " in Sale Point #" +
                     std::to_string(this->_centerId) + "." + std::to_string(this->_sellerId) + ": " +
                     std::to_string(request.rosesAmount) + " roses and " + std::to_string(request.tulipsAmount) + " tulips.");
        this->_clientPipe->write(request.serialize());
    }

    exit(EXIT_SUCCESS);
}

BouquetRequest ClientSimulator::simulateBouquetRequest() {
    // Generating random flower request.
    int rosesAmount = rand() % 10;
    int tulipsAmount = rand() % 10;

    return BouquetRequest(rosesAmount, tulipsAmount);
}
