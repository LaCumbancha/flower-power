#include "SellerJob.h"

SellerJob::SellerJob(const int center, const Seller &sellerData, Pipe *requestPipe, Pipe *distributionPipe) : Job() {
    this->_center = center;
    this->_distributionPipe = distributionPipe;
    this->_requestPipe = requestPipe;
    this->_sellerId = sellerData.sellerId;
    this->_rosesStock = sellerData.rosesStock;
    this->_tulipsStock = sellerData.tulipsStock;
}

int SellerJob::run() {
    this->_clientPipe = new Pipe();
    pid_t pid = fork();

    if (pid == CHILD_PROCESS_PID) {
        // Child process.
        this->_clientPipe->setWriteMode();
        ClientSimulator clientSimulator = ClientSimulator(this->_center, this->_sellerId, this->_clientPipe);
        clientSimulator.run();
    } else {
        // Seller process.
        Logger::info("Client Simulator # " + std::to_string(this->_center) + "." + std::to_string(this->_sellerId) +
                     " running in process " + std::to_string(pid) + ".");
        this->_clientSimulatorPID = pid;
        this->_clientPipe->setReadMode();

        // Listening for incoming requests.
        this->listenRequests();
    }

    return EXIT_SUCCESS;
}

int SellerJob::listenRequests() {
    std::string incoming;
    int status;
    Logger::info("Seller # " + std::to_string(this->_center) + "." + std::to_string(this->_sellerId) +
                 " started to listen for requests.");
    while (this->_clientPipe->read(incoming, &status)) {
        if (status == EXIT_SUCCESS) {
            BouquetRequest bouquetRequest = BouquetRequest::deserialize(incoming);
            this->handleRequest(bouquetRequest);
        }
    }

    return EXIT_SUCCESS;
}

void SellerJob::handleRequest(BouquetRequest bouquetRequest) {
    Logger::info("Seller # " + std::to_string(this->_center) + "." + std::to_string(this->_sellerId) +
                 " received a request for " + std::to_string(bouquetRequest.rosesAmount) + " roses and " +
                 std::to_string(bouquetRequest.tulipsAmount) + " tulips.");

    if (this->_rosesStock < bouquetRequest.rosesAmount || this->_tulipsStock < bouquetRequest.tulipsAmount) {
        // TODO: Request stock to Distribution Center and handle case of not having anymore stock.
    }

    // TODO: Remove when Stock Manager added.
    this->_rosesStock -= bouquetRequest.rosesAmount;
    this->_tulipsStock -= bouquetRequest.tulipsAmount;
}

int SellerJob::finish() {
    int processStatus;

    // Awaiting for client simulator process.
    waitpid(this->_clientSimulatorPID, &processStatus, 0);

    if (processStatus != EXIT_SUCCESS) {
        Logger::error("Client simulator in process " + std::to_string(this->_clientSimulatorPID) +
                      " finished with error code " + std::to_string(processStatus));
    } else {
        Logger::info("Client Simulator #" + std::to_string(this->_center) + "." + std::to_string(this->_sellerId) +
                     " successfully ended without errors.");
    }

    this->_distributionPipe->~Pipe();
    this->_requestPipe->~Pipe();
    exit(EXIT_SUCCESS);
}
