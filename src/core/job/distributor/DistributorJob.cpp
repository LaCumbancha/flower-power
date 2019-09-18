//
// Created by darius on 17/9/19.
//

#include "DistributorJob.h"

int DistributorJob::run() {

    int status;
    std::string incoming;
    while (this->_requestsPipe->read(incoming, &status)) {
        if (status == EXIT_SUCCESS) {
            SellerRequest sellerRequest = SellerRequest::deserialize(incoming);
            handleRequest(sellerRequest);
        }
    }
    return Job::run();
}

void DistributorJob::handleRequest(const SellerRequest &request) {
    Pipe* responsePipe = _distributionPipes.find(request.sellerId)->second;
    _rosesStock -= request.rosesBoxAmount;
    _tulipsStock -= request.tulipsBoxAmount;
    std::cout << _rosesStock << std::endl;
    std::cout << _tulipsStock << std::endl;
}

int DistributorJob::finish() {
    return Job::finish();
}

int DistributorJob::getRosesStock() const {
    return _rosesStock;
}

void DistributorJob::setRosesStock(int rosesStock) {
    _rosesStock = rosesStock;
}

int DistributorJob::getTulipsStock() const {
    return _tulipsStock;
}

void DistributorJob::setTulipsStock(int tulipsStock) {
    _tulipsStock = tulipsStock;
}
