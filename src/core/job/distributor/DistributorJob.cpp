//
// Created by darius on 17/9/19.
//

#include "DistributorJob.h"
#include "../../config/data/ClassifierBox.h"

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
    if (request.rosesBoxAmount < _rosesStock || request.tulipsBoxAmount < _tulipsStock) {
        resupply(request);
    }
    _rosesStock -= request.rosesBoxAmount;
    _tulipsStock -= request.tulipsBoxAmount;

    std::cout << _rosesStock << std::endl;
    std::cout << _tulipsStock << std::endl;
}

void DistributorJob::resupply(const SellerRequest &request) {
    int status;
    std::string data;

    while ((_rosesStock - request.rosesBoxAmount) >= 0 &&
           (_tulipsStock - request.tulipsBoxAmount) >= 0) {
        ssize_t readAmount = _classifierPipe->read(data, &status);

        if (readAmount == -1) {
            //TODO: handle
            return;
        }

        if (readAmount == 0) {
            //TODO: handle end of file
            std::cout << "End of file" << std::endl;
            return;
        }

        if (status == EXIT_SUCCESS) {
            ClassifierBox cb = ClassifierBox::deserialize(data);
            switch (cb.flowerType) {
                //TODO handle with flowers and not with ints
                case ROSE:
                    _rosesStock += cb._flowers.size();
                    break;
                case TULIP:
                    _tulipsStock += cb._flowers.size();
                    break;
            }
        }
    };
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
