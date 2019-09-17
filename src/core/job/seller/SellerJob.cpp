#include "SellerJob.h"

int SellerJob::run() {
    Logger::info("Running Seller job.");
    exit(EXIT_SUCCESS);
}

SellerJob::SellerJob(const int center, const Seller &sellerData, Pipe *requestPipe, Pipe *distributionPipe) : Job() {
    this->_center = center;
    this->_distributionPipe = distributionPipe;
    this->_requestPipe = requestPipe;
    this->_sellerId = sellerData.sellerId;
    this->_rosesStock = sellerData.rosesStock;
    this->_tulipsStock = sellerData.tulipsStock;
}


