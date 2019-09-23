#include "SellerJob.h"

#include <utility>
#include "../../config/data/SellerRequest.h"
#include "../../../utils/StatsCenter.h"

SellerJob::SellerJob(std::string sellerId, int clients, Pipe *requestPipe, Pipe *distributionPipe) : Job() {
    this->_clients = clients;
    this->_sellerId = std::move(sellerId);
    this->_requestPipe = requestPipe;
    this->_distributionPipe = distributionPipe;
    this->_rosesStock = std::vector<Flower>();
    this->_tulipsStock = std::vector<Flower>();
}

int SellerJob::run() {
    this->_clientPipe = new Pipe();
    pid_t pid = fork();

    if (pid == CHILD_PROCESS_PID) {
        // Child process.
        this->_clientPipe->setWriteMode();
        auto clientSimulator = ClientSimulator(this->_sellerId, this->_clients, _clientPipe);
        clientSimulator.run();
    } else {
        // Seller process.
        Logger::info("Client Simulator # " + this->_sellerId + " running in process " + std::to_string(pid) + ".");
        this->_clientSimulatorPID = pid;
        this->_clientPipe->setReadMode();

        // Listening for incoming requests.
        this->listenRequests();
        this->finish();
    }

    return EXIT_SUCCESS;
}

int SellerJob::listenRequests() {
    std::string incoming;
    int status;
    bool canHandleRequests = true;
    Logger::info("Seller # " + this->_sellerId + " started to listen for requests.");
    while (canHandleRequests && (this->_clientPipe->read(incoming, &status) > 0)) {
        if (status == EXIT_SUCCESS) {
            BouquetRequest bouquetRequest = BouquetRequest::deserialize(incoming);
            this->handleRequest(bouquetRequest);
        }
        canHandleRequests = _distributionPipeIsOpen || !_rosesStock.empty() || !_tulipsStock.empty();
    }

    return EXIT_SUCCESS;
}

void SellerJob::handleRequest(BouquetRequest bouquetRequest) {
    // Uncomment the following line to measure stats real time.
    // sleep(10);
    Logger::info("Seller # " + this->_sellerId + " received a request for " + std::to_string(bouquetRequest.rosesAmount) +
            " roses and " + std::to_string(bouquetRequest.tulipsAmount) + " tulips.");

    if ((this->_rosesStock.size() < bouquetRequest.rosesAmount ||
         this->_tulipsStock.size() < bouquetRequest.tulipsAmount) && _distributionPipeIsOpen) {
        resupply(bouquetRequest);
        return;
    }

    for (int i = 0; i < bouquetRequest.rosesAmount && !_rosesStock.empty(); i++) {
        Flower flower = _rosesStock.back();
        Logger::info("A rose from Producer #" + flower.producerId + " (" + flower.producerName + ") was sold by Seller #" + this->_sellerId);
        _rosesStock.pop_back();

        // Adding information to Stats Center.
        StatsCenter::addSale(flower, ROSE);
        Logger::debug("Seller #" + this->_sellerId + " added a rose to the Stats Center.");
    }

    for (int i = 0; i < bouquetRequest.tulipsAmount && !_tulipsStock.empty(); i++) {
        Flower flower = _tulipsStock.back();
        Logger::info("A tulip from Producer #" + flower.producerId + " (" + flower.producerName + ") was sold by Seller #" + this->_sellerId);
        _rosesStock.pop_back();

        // Adding information to Stats Center.
        StatsCenter::addSale(flower, TULIP);
        Logger::debug("Seller #" + this->_sellerId + " added a tulip to the Stats Center.");
    }
}

void SellerJob::resupply(BouquetRequest request) {
    unsigned int rosesBoxAmount =
            div((int) (request.rosesAmount - _rosesStock.size()), CLASSIFIER_BOX_SIZE).quot + 1;
    unsigned int tulipsBoxAmount =
            div((int) (request.tulipsAmount - _tulipsStock.size()), CLASSIFIER_BOX_SIZE).quot + 1;

    SellerRequest sellerRequest = SellerRequest(_sellerId, rosesBoxAmount, tulipsBoxAmount);
    ssize_t wroteAmount = _requestPipe->write(sellerRequest.serialize());
    if (wroteAmount == -1) {
        Logger::error("Seller # " + this->_sellerId +
                      " tried to ask for a resupply to the distribution center while the requests pipe is closed.");
        return;
    }

    unsigned int receivedBoxes = 0;
    while (_distributionPipeIsOpen && receivedBoxes < (rosesBoxAmount + tulipsBoxAmount)) {
        std::string serializedCb;
        int status;
        int readAmount = _distributionPipe->read(serializedCb, &status);

        if (readAmount == -1) {
            Logger::debug("Seller # " + this->_sellerId + " : broken pipe.");
            return;
        }

        if (readAmount == 0) {
            Logger::debug(
                    "Seller # " + this->_sellerId + " could not resupply due to the distributor pipe being closed.");
            _distributionPipeIsOpen = false;
            return;
        }

        if (status == EXIT_SUCCESS) {
            ClassifierBox cb = ClassifierBox::deserialize(serializedCb);
            switch (cb.flowerType) {
                case ROSE:
                    for (const auto &flower: cb._flowers) {
                        _rosesStock.push_back(flower);
                    }
                    break;
                case TULIP:
                    for (const auto &flower: cb._flowers) {
                        _tulipsStock.push_back(flower);
                    }
                    break;
            }
            receivedBoxes++;
        }
    }
    Logger::debug("Seller # " + this->_sellerId + " stock after resupply:" +
                  "[rose flowers: " + std::to_string(_rosesStock.size()) + " | tulips flowers: " +
                  std::to_string(_tulipsStock.size()) + "]");
}


int SellerJob::finish() {
    int processStatus;

    // Awaiting for client simulator process.
    waitpid(this->_clientSimulatorPID, &processStatus, 0);

    if (processStatus != EXIT_SUCCESS) {
        Logger::error("Client simulator in process " + std::to_string(this->_clientSimulatorPID) +
                      " finished with error code " + std::to_string(processStatus));
    } else {
        Logger::info("Client Simulator #" + this->_sellerId + " successfully ended without errors.");
    }
    delete _clientPipe;

    delete _distributionPipe;

    delete _requestPipe;

    exit(EXIT_SUCCESS);
}
