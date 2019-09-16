#include "SellerJob.h"
#include "../../../simulator/ClientSimulator.h"

int SellerJob::run() {
    std::cout << "Seller Job RUN" << std::endl;
    return 0;
}

SellerJob::SellerJob(const int center, const Seller &sellerData, Pipe *requestPipe, Pipe *distributionPipe) : Job() {
    this->_center = center;
    this->_distributionPipe = distributionPipe;
    this->_requestPipe = requestPipe;
    this->_sellerId = sellerData.sellerId;
    this->_rosesStock = sellerData.rosesStock;
    this->_tulipsStock = sellerData.tulipsStock;

    Pipe* clientPipe = new Pipe();
    pid_t pid = fork();

    if (pid == CHILD_PROCESS_PID) {
        clientPipe->setWriteMode();
        ClientSimulator clientSimulator = ClientSimulator(0, sellerData.sellerId, 0, clientPipe);
        clientSimulator.Run();
    } else {
        clientPipe->setReadMode();
        std::string incomming;
        int status;

        int i = clientPipe->read(incomming, &status);
        std::cout << incomming << std::endl;
    }
}


