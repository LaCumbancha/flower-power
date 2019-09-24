#include "DistributorJob.h"

#include <utility>
#include "../../../utils/signals/SignalHandler.h"
#include "../../../utils/signals/StopHandler.h"

DistributorJob::DistributorJob(int center, Pipe *classifierPipe, Pipe *requestsPipe, std::map<std::string, Pipe*> distributionPipes) {

    // Registering SIGTERM handler.
    auto handler = new StopHandler(this);
    SignalHandler::getInstance()->registerHandler(SIGTERM, handler);

    // Assigning pipe to communicate with the classifier.
    this->_classifierPipe = classifierPipe;

    // Assigning pipe to get requests from the sellers.
    this->_requestsPipe = requestsPipe;

    // Assigning private pipe to communicate with each seller.
    this->_distributionPipes = std::move(distributionPipes);

    // Initializing distribution center data.
    this->_center = center;

    // Initializing state.
    this->initializeStatus();

}

int DistributorJob::run() {

    int status;
    std::string incoming;

    Logger::info("Distributor job from Distribution Center #" + std::to_string(_center) + " running.");

    while (this->_requestsPipe->read(incoming, &status) > 0) {
        if (status == EXIT_SUCCESS) {
            SellerRequest sellerRequest = SellerRequest::deserialize(incoming);
            handleRequest(sellerRequest);
        }
    }

    while (resupplyPipeIsOpen) {
        takeClassifierBox();
    }
    Logger::info("Distributor job from Distribution Center #" + std::to_string(_center) + " remaining stock: " +
                 std::to_string(_rosesStock.size()) + " roses boxes and " + std::to_string(_tulipsStock.size()) +
                 " tulips boxes.");
    return Job::run();
}

void DistributorJob::handleRequest(const SellerRequest &request) {

    // Uncomment the following line to measure stats in real time.
    // sleep(1);

    Logger::info("Distributor #" + std::to_string(_center) + " received a request from Seller #"
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
        Logger::info("Distributor job #" + std::to_string(_center) + " destroyed the pipe connecting to the seller #" +
                     request.sellerId + " due to the fact that his resupply request cannot be satisfied.");
        return;
    }

    for (int i = 0; i < request.rosesBoxAmount && !_rosesStock.empty(); i++) {
        ClassifierBox cb = _rosesStock.back();
        _rosesStock.pop_back();
        ssize_t wroteAmount = responsePipe->write(cb.serialize());
        if (wroteAmount == -1) {
            Logger::error("Distributor #" + std::to_string(_center) + " could not supply the Seller #" +
                          request.sellerId + " due to a pipe error.");
            return;
        }

    }

    for (int i = 0; i < request.tulipsBoxAmount && !_tulipsStock.empty(); i++) {
        ClassifierBox cb = _tulipsStock.back();
        _tulipsStock.pop_back();
        ssize_t wroteAmount = responsePipe->write(cb.serialize());
        if (wroteAmount == -1) {
            Logger::error("Distributor #" + std::to_string(_center) + " could not supply the Seller #" +
                          request.sellerId + " due to a pipe error.");
            return;
        }
    }

}

void DistributorJob::resupply(const SellerRequest &request) {

    Logger::info("Distributor #" + std::to_string(_center) + " has not enough stock to deliver the request "
                 + "of flowers made by Seller #" + request.sellerId + ", consisting of " + std::to_string(request.rosesBoxAmount)
                 + " roses boxes and " + std::to_string(request.tulipsBoxAmount) + " of tulips boxes. Current DC stock: " +
                 std::to_string(_rosesStock.size()) + " roses boxes and " + std::to_string(_tulipsStock.size()) +
                 " tulips boxes.");

    while ((_rosesStock.size() < request.rosesBoxAmount) ||
           (_tulipsStock.size() < request.tulipsBoxAmount)) {
        takeClassifierBox();
    };
}

int DistributorJob::finish() {

    delete _classifierPipe;
    Logger::info("Distributor job #" + std::to_string(_center) + " pipe connected to classifier destroyed.");

    delete _requestsPipe;
    Logger::info("Distributor job #" + std::to_string(_center) + " requests pipe destroyed.");

    for (const auto& distributionPipe : _distributionPipes ) {
        delete distributionPipe.second;
        Logger::info("Distributor job #" + std::to_string(_center) + " distribution pipe connected to seller #" +
                     distributionPipe.first + " destroyed.");
    }

    exit(EXIT_SUCCESS);
}

DistributorJob::~DistributorJob() {
    this->finish();
}

void DistributorJob::takeClassifierBox() {
    int status;
    std::string data;
    ssize_t readAmount = _classifierPipe->read(data, &status);

    if (readAmount == -1) {
        Logger::error("Distributor #" + std::to_string(_center) + " could not resupply due to a pipe error.");
        return;
    }

    if (readAmount == 0) {
        Logger::info("Distributor job #" + std::to_string(_center) +
                     " could not fully resupply because of closed pipe. Remnant stock: " + "[roses boxes stock: " +
                     std::to_string(_rosesStock.size()) + " ; tulip boxes stock: " +
                     std::to_string(_tulipsStock.size()) + "]");
        resupplyPipeIsOpen = false;

        return;
    }

    if (status == EXIT_SUCCESS) {
        Logger::debug("Distibutor job  #" + std::to_string(_center) + " just read " + std::to_string(readAmount) + " from serialized classifier box: " + data);
        ClassifierBox cb = ClassifierBox::deserialize(data);
        switch (cb.flowerType) {
            case ROSE:
                _rosesStock.push_back(cb);
                break;
            case TULIP:
                _tulipsStock.push_back(cb);
                break;
        }

        Logger::info("Distributor #" + std::to_string(_center) + " stock after resupply: " +
                     std::to_string(_rosesStock.size()) + " roses boxes and " + std::to_string(_tulipsStock.size()) +
                     " tulips boxes.");
    }
}

std::string DistributorJob::contextState() {
    std::string state = 'D' + std::to_string(this->_center) + ',';

    for (auto rose : this->_rosesStock) {
        state += rose.serialize() + '!';
    }

    state += ',';

    for (auto tulip : this->_tulipsStock) {
        state += tulip.serialize() + '!';
    }

    return state;
}

int DistributorJob::stopJob() {
    Logger::info("Distributor Job #" + std::to_string(this->_center) + " saved a stock of "
                 + std::to_string(this->_rosesStock.size()) + " roses boxes and "
                 + std::to_string(this->_tulipsStock.size()) + " tulips boxes.");
    ContextStatus::saveContext(this->contextState());
    delete this;
    return EXIT_SUCCESS;
}

void DistributorJob::initializeStatus() {

    std::string previousState = ContextStatus::retrieveContext('D' + std::to_string(this->_center));

    if (previousState.empty()) {
        Logger::info("Creating new empty state for Distributor #" + std::to_string(this->_center));
    } else {
        Logger::info("Load previous state for Distributor #" + std::to_string(this->_center));
        this->loadPreviousState(previousState);
    }

}

void DistributorJob::loadPreviousState(const string& previousState) {

    std::string buffer;
    std::vector<std::string> flowers;
    std::vector<std::vector<std::string>> boxes;

    for (auto character : previousState) {

        if (character == '!') {
            flowers.push_back(buffer + '\0');
            buffer = "";
        } else if (character == ',') {
            boxes.push_back(flowers);
            flowers.clear();
        } else {
            buffer += character;
        }

    }

    boxes.push_back(flowers);

    for (const auto& rose : boxes[0]) {
        this->_rosesStock.push_back(ClassifierBox::deserialize(rose));
    }

    for (const auto& tulip : boxes[1]) {
        this->_tulipsStock.push_back(ClassifierBox::deserialize(tulip));
    }

    Logger::info("Distributor Job #" + std::to_string(this->_center) + " retrieved a stock of "
                 + std::to_string(this->_rosesStock.size()) + " roses boxes and "
                 + std::to_string(this->_tulipsStock.size()) + " tulips boxes.");


}
