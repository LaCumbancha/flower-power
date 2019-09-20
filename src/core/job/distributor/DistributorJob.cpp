//
// Created by darius on 17/9/19.
//

#include <cstring>
#include "DistributorJob.h"
#include "../../config/data/ClassifierBox.h"
#include "../../../utils/Logger.h"

int DistributorJob::run() {

    int status;
    std::string incoming;
    Logger::info("Distributor job from distribution center #" + std::to_string(_centerId) + " running.");

    while (this->_requestsPipe->read(incoming, &status)) {
        if (status == EXIT_SUCCESS) {
            SellerRequest sellerRequest = SellerRequest::deserialize(incoming);
            handleRequest(sellerRequest);
        }
    }
    return Job::run();
}

void DistributorJob::handleRequest(const SellerRequest &request) {

    Logger::info("Distributor job #" + std::to_string(_centerId) +
                 " received a request from seller #" + request.sellerId + " asking for " +
                 std::to_string(request.rosesBoxAmount)
                 + " of roses classifier boxes and " + std::to_string(request.tulipsBoxAmount) +
                 " of tulip classifier boxes.\n" +
                 "Current stock: " + "[roses boxes stock: " + std::to_string(_rosesStock) + " ; tulip boxes stock: " +
                 std::to_string(_tulipsStock) + "]");

    Pipe* responsePipe = _distributionPipes.find(request.sellerId)->second;
    if (request.rosesBoxAmount > _rosesStock || request.tulipsBoxAmount > _tulipsStock) {
        resupply(request);
    }

    //TODO: send back to the sellers what they requested

    _rosesStock -= request.rosesBoxAmount;
    _tulipsStock -= request.tulipsBoxAmount;

//    std::cout << _rosesStock << std::endl;
//    std::cout << _tulipsStock << std::endl;
}

void DistributorJob::resupply(const SellerRequest &request) {
    int status;
    std::string data;

    Logger::info("Distributor job #" + std::to_string(_centerId) +
                 " has not enough stock to deliver the request of flowers made by seller #" + request.sellerId +
                 " consisting of " + std::to_string(request.rosesBoxAmount)
                 + " roses classifier boxes and " + std::to_string(request.tulipsBoxAmount) +
                 " of tulip classifier boxes: \n" +
                 "[roses boxes stock: " + std::to_string(_rosesStock) + " ; tulip boxes stock: " +
                 std::to_string(_tulipsStock) + "]");

    while ((_rosesStock - request.rosesBoxAmount) < 0 &&
           (_tulipsStock - request.tulipsBoxAmount) < 0) {
        ssize_t readAmount = _classifierPipe->read(data, &status);
        if (readAmount == -1) {
            Logger::error("Distributor job #" + std::to_string(_centerId) + " could not resupply due to a pipe error.");
            //TODO: handle
            return;
        }

        if (readAmount == 0) {
            //TODO: handle end of file
            Logger::info("Distributor job #" + std::to_string(_centerId) + " could not resupply because of closed pipe.");
            return;
        }
        if (status == EXIT_SUCCESS) {
            ClassifierBox cb = ClassifierBox::deserialize(data);
            switch (cb.flowerType) {
                //TODO handle with flowers and not with ints
                case ROSE:
                    _rosesStock ++;
                    break;
                case TULIP:
                    _tulipsStock ++;
                    break;
            }
        }

        Logger::info("Distributor job #" + std::to_string(_centerId) + " stock after resupply: \n" +
                     "[roses boxes stock: " + std::to_string(_rosesStock) + " ; tulip boxes stock: " +
                     std::to_string(_tulipsStock) + "]");
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
