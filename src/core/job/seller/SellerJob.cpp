#include "SellerJob.h"

int SellerJob::run() {
    Pipe* clientPipe = new Pipe();
    pid_t pid = fork();

    if (pid == CHILD_PROCESS_PID) {
        clientPipe->setWriteMode();
        ClientSimulator clientSimulator = ClientSimulator(0, _sellerId, 0, clientPipe);
        clientSimulator.Run();
    } else {
        clientPipe->setReadMode();
        std::string incomming;
        int status;
        while( clientPipe->read(incomming, &status) != 0) {
            if ( status != EXIT_FAILURE) {
                BouquetRequest bouquetRequest = BouquetRequest(incomming);
                handleRequest(bouquetRequest);
            }
        };
    }
    Logger::info("Running Seller job.");
    return EXIT_SUCCESS;
}

SellerJob::SellerJob(const int center, const Seller &sellerData, Pipe *requestPipe, Pipe *distributionPipe) : Job() {
    this->_center = center;
    this->_distributionPipe = distributionPipe;
    this->_requestPipe = requestPipe;
    this->_sellerId = sellerData.sellerId;
    this->_rosesStock = sellerData.rosesStock;
    this->_tulipsStock = sellerData.tulipsStock;
}

void SellerJob::handleRequest(BouquetRequest bouquetRequest) {
    bool success;
    if (_rosesStock < bouquetRequest.rosesAmount || _tulipsStock < bouquetRequest.tulipsAmount) {
        //TODO request stock and handle case of distributor not having anymore stock
    }

    _rosesStock -= bouquetRequest.rosesAmount;
    _tulipsStock -= bouquetRequest.tulipsAmount;
    //TODO: Log properly
}

int SellerJob::finish() {
    exit(EXIT_SUCCESS);
}


