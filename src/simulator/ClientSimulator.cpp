#include "ClientSimulator.h"

#include <utility>


ClientSimulator::ClientSimulator(std::string sellerId, int clients, Pipe &clientPipe) {
    this->_sellerId = std::move(sellerId);
    this->_clients = clients;
    this->_clientPipe = clientPipe;

    // Initializing random number generator
    srand(time(NULL) * getpid());
}

void ClientSimulator::run() {

    for (int client = 1; client <= this->_clients; client++) {
        BouquetRequest request = simulateBouquetRequest();

        Logger::info("Generating request for Client #" + std::to_string(client) + " in Sale Point #" + this->_sellerId +
                     ": " +
                     std::to_string(request.rosesAmount) + " roses and " + std::to_string(request.tulipsAmount) +
                     " tulips.");
        this->_clientPipe.write(request.serialize());
    }
    Logger::info("Client simulator #" + _sellerId + " finished it's work.");
}

BouquetRequest ClientSimulator::simulateBouquetRequest() {
    // Generating random flower request.
    int rosesAmount = rand() % 10;
    int tulipsAmount = rand() % 10;

    return BouquetRequest(rosesAmount, tulipsAmount);
}

ClientSimulator::~ClientSimulator() {
    Logger::debug("Client simulator #" + _sellerId + " destroyed.");
    exit(EXIT_SUCCESS);
}
