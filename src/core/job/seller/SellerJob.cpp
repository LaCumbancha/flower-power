#include "SellerJob.h"
#include "../../../simulator/ClientSimulator.h"

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
        std::cout << "\nProceso padre" << std::endl;
        while( clientPipe->read(incomming, &status) != 0) {
          std::cout << incomming << std::endl;
        };
    }
    return 0;
}

SellerJob::SellerJob(const int center, const Seller &sellerData, Pipe *requestPipe, Pipe *distributionPipe) : Job() {
    this->_center = center;
    this->_distributionPipe = distributionPipe;
    this->_requestPipe = requestPipe;
    this->_sellerId = sellerData.sellerId;
    this->_rosesStock = sellerData.rosesStock;
    this->_tulipsStock = sellerData.tulipsStock;
}


