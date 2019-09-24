#include "ClientSimulator.h"
#include "../core/config/Paths.h"



ClientSimulator::ClientSimulator(std::string sellerId, int clients, Pipe * clientPipe) {
    this->_sellerId = std::move(sellerId);
    this->_clients = clients;
    this->_clientPipe = clientPipe;

    // Initializing random number generator
    srand(time(NULL) * getpid());

    // Registering SIGTERM handler.
    auto handler = new StopHandler(this);
    SignalHandler::getInstance()->registerHandler(SIGTERM, handler);
}

int ClientSimulator::run() {

    loadOnlineSales();
    for (auto onlineSale : _onlineSales) {
        Logger::info("Client simulator #" + _sellerId + ": generating request for an online sale consisting of " +
                     std::to_string(onlineSale.rosesAmount) + " roses and " + std::to_string(onlineSale.tulipsAmount) +
                     " tulips.");
        this->_clientPipe->write(onlineSale.serialize());
    }

    for (int client = 1; client <= this->_clients; client++) {

        // Uncomment the following line to measure stats in real time.
        sleep(1);

        BouquetRequest request = simulateBouquetRequest();
        Logger::info("Client simulator #" + _sellerId + ": generating request for Client #" + std::to_string(client) +
                     " in Sale Point #" + this->_sellerId + ": " + std::to_string(request.rosesAmount) + " roses and " +
                     std::to_string(request.tulipsAmount) + " tulips.");
        this->_clientPipe->write(request.serialize());
    }
    Logger::info("Client simulator #" + _sellerId + " finished it's work.");
    return EXIT_SUCCESS;
}

BouquetRequest ClientSimulator::simulateBouquetRequest() {
    // Generating random flower request.
    int rosesAmount = rand() % 10;
    int tulipsAmount = rand() % 10;

    return BouquetRequest(rosesAmount, tulipsAmount, false);
}

ClientSimulator::~ClientSimulator() {
    Logger::debug("Client simulator #" + _sellerId + " destroyed.");
    delete _clientPipe;
    exit(EXIT_SUCCESS);
}

void ClientSimulator::loadOnlineSales() {
    auto reader = CSVReader(onlineSalesPath);
    std::vector<std::vector<std::string>> salesData = reader.getData();

    for (auto data: salesData) {
        if ((data[0] + "." + data[1]) == this->_sellerId) {
            BouquetRequest bouquetRequest = BouquetRequest(std::stoi(data[2]), std::stoi(data[3]), true);
            _onlineSales.push_back(bouquetRequest);
        }
    }
}

int ClientSimulator::stopJob() {
    delete this;
    return EXIT_SUCCESS;
}
