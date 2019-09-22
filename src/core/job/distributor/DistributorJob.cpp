#include "DistributorJob.h"

int DistributorJob::run() {

    int status;
    std::string incoming;
    Logger::info("Distributor job from Distribution Center #" + std::to_string(_centerId) + " running.");

    while (this->_requestsPipe->read(incoming, &status)) {
        if (status == EXIT_SUCCESS) {
            SellerRequest sellerRequest = SellerRequest::deserialize(incoming);
            handleRequest(sellerRequest);
        }
    }
    return Job::run();
}

void DistributorJob::handleRequest(const SellerRequest &request) {

    Logger::info("Distributor #" + std::to_string(_centerId) + " received a request from Seller #"
                 + request.sellerId + " asking for " + std::to_string(request.rosesBoxAmount) + " roses boxes and " +
                 std::to_string(request.tulipsBoxAmount) + " tulips boxes. Current DC stock: " +
                 std::to_string(_rosesStock.size()) + " roses boxes and " + std::to_string(_tulipsStock.size()) +
                 " tulips boxes.");

    auto responsePipeInfo = _distributionPipes.find(request.sellerId);
    Pipe *responsePipe = responsePipeInfo->second;
    if ((request.rosesBoxAmount > _rosesStock.size() || request.tulipsBoxAmount > _tulipsStock.size()) && resupplyPipeIsOpen) {
        resupply(request);
    }

    if (!resupplyPipeIsOpen && (request.rosesBoxAmount > _rosesStock.size() || request.tulipsBoxAmount > _tulipsStock.size())) {
        delete responsePipe;
        _distributionPipes.erase(responsePipeInfo);
        Logger::info("Distributor job #" + std::to_string(_centerId) + " destroyed the pipe connecting to the seller #" +
                      request.sellerId + " due to the fact that his resupply request cannot be satisfied.");
        return;
    }

    for (int i = 0; i < request.rosesBoxAmount && !_rosesStock.empty(); i++) {
        ClassifierBox cb = _rosesStock.back();
        _rosesStock.pop_back();
        ssize_t wroteAmount = responsePipe->write(cb.serialize());
        if (wroteAmount == -1) {
            Logger::error("Distributor #" + std::to_string(_centerId) + " could not supply the Seller #" +
                          request.sellerId + " due to a pipe error.");
            return;
        }

    }

    for (int i = 0; i < request.tulipsBoxAmount && !_tulipsStock.empty(); i++) {
        ClassifierBox cb = _tulipsStock.back();
        _tulipsStock.pop_back();
        ssize_t wroteAmount = responsePipe->write(cb.serialize());
        if (wroteAmount == -1) {
            Logger::error("Distributor #" + std::to_string(_centerId) + " could not supply the Seller #" +
                          request.sellerId + " due to a pipe error.");
            return;
        }
    }

}

void DistributorJob::resupply(const SellerRequest &request) {
    int status;

    Logger::info("Distributor #" + std::to_string(_centerId) + " has not enough stock to deliver the request "
                 +"of flowers made by Seller #" + request.sellerId + ", consisting of " + std::to_string(request.rosesBoxAmount)
                 + " roses boxes and " + std::to_string(request.tulipsBoxAmount) + " of tulips boxes. Current DC stock: " +
                 std::to_string(_rosesStock.size()) + " roses boxes and " + std::to_string(_tulipsStock.size()) +
                 " tulips boxes.");

    while ((_rosesStock.size() < request.rosesBoxAmount) ||
           (_tulipsStock.size() < request.tulipsBoxAmount)) {

        std::string data;
        ssize_t readAmount = _classifierPipe->read(data, &status);


        if (readAmount == -1) {
            Logger::error("Distributor #" + std::to_string(_centerId) + " could not resupply due to a pipe error.");
            return;
        }

        if (readAmount == 0) {
            Logger::info("Distributor job #" + std::to_string(_centerId) +
                         " could not fully resupply because of closed pipe. Remnant stock: " + "[roses boxes stock: " +
                         std::to_string(_rosesStock.size()) + " ; tulip boxes stock: " +
                         std::to_string(_tulipsStock.size()) + "]");
            resupplyPipeIsOpen = false;

            return;
        }

        if (status == EXIT_SUCCESS) {
            Logger::debug("Distibutor job  #"  + std::to_string(_centerId) + " just read " + std::to_string(readAmount) + " from serialized classifier box: \n" + data);
            ClassifierBox cb = ClassifierBox::deserialize(data);
            switch (cb.flowerType) {
                case ROSE:
                    _rosesStock.push_back(cb);
                    break;
                case TULIP:
                    _tulipsStock.push_back(cb);
                    break;
            }

            Logger::info("Distributor #" + std::to_string(_centerId) + " stock after resupply: " +
                         std::to_string(_rosesStock.size()) + " roses boxes and " + std::to_string(_tulipsStock.size()) +
                         " tulips boxes.");
        }
    };
}

int DistributorJob::finish() {

    ContextStorage::saveContext(this->contextState());

    delete _classifierPipe;
    Logger::info("Distributor job #" + std::to_string(_centerId) + " pipe connected to classifier destroyed.");

    delete _requestsPipe;
    Logger::info("Distributor job #" + std::to_string(_centerId) + " requests pipe destroyed.");

    for (const auto& distributionPipe : _distributionPipes ) {
        delete distributionPipe.second;
        Logger::info("Distributor job #" + std::to_string(_centerId) + " distribution pipe connected to seller #" +
                     distributionPipe.first + " destroyed.");
    }
    exit(EXIT_SUCCESS);
}

std::string DistributorJob::contextState() {
    std::string state = 'D' + std::to_string(this->_centerId) + ',';

    for (auto rose : this->_rosesStock) {
        state += rose.serialize() + '!';
    }

    state += ',';

    for (auto tulip : this->_tulipsStock) {
        state += tulip.serialize() + '!';
    }

    return state;
}

