#include "SellerJob.h"
#include "../../../utils/signals/SignalHandler.h"
#include "../../../utils/signals/StopHandler.h"

#include <utility>
#include "../../config/data/SellerRequest.h"
#include "../../../utils/StatsCenter.h"
#include "../../config/Paths.h"
#include "../../../utils/Closer.h"

SellerJob::SellerJob(std::string sellerId, int clients, Pipe *requestPipe, Pipe *distributionPipe) : Job() {
    this->_clients = clients;
    this->_sellerId = std::move(sellerId);
    this->_requestPipe = requestPipe;
    this->_distributionPipe = distributionPipe;

    // Registering SIGTERM handler.
    auto handler = new StopHandler(this);
    SignalHandler::getInstance()->registerHandler(SIGTERM, handler);

    // Initializing seller data.
    this->initializeStatus(this->_sellerId);
}

int SellerJob::run() {

    this->_clientPipe = new Pipe();
    pid_t pid = fork();

    if (pid == CHILD_PROCESS_PID) {
        // Child process.
        pid = getpid();
        this->_clientPipe->setWriteMode();
        auto clientSimulator = new ClientSimulator(this->_sellerId, this->_clients, _clientPipe);
        clientSimulator->run();

        Logger::info("Client Simulator #" + this->_sellerId + " running in process with PID #" + std::to_string(pid) + " destroyed.");
        ProcessKiller::removePID(pid);
        Closer::finishAuxJobs(pid);

        delete clientSimulator;
        exit(EXIT_SUCCESS);
    } else {
        // Seller process.
        Logger::info("Client Simulator # " + this->_sellerId + " running in process " + std::to_string(pid) + ".");
        this->_clientSimulatorPID = pid;
        ProcessKiller::addPID(pid);
        this->_clientPipe->setReadMode();

        // Listening for incoming requests.
        this->listenRequests();
    }

    return EXIT_SUCCESS;
}

int SellerJob::listenRequests() {
    std::string incoming;
    int status;
    bool canHandleRequests = true;
    Logger::info("Seller #" + this->_sellerId + " started to listen for requests.");
    while (canHandleRequests && (this->_clientPipe->read(incoming, &status) > 0)) {
        if (status == EXIT_SUCCESS) {
            BouquetRequest bouquetRequest = BouquetRequest::deserialize(incoming);
            this->handleRequest(bouquetRequest);
        }
        canHandleRequests = _distributionPipeIsOpen || !_rosesStock.empty() || !_tulipsStock.empty();
    }

    return EXIT_SUCCESS;
}

void SellerJob::handleRequest(BouquetRequest bouquetRequest) {

    // Uncomment the following line to measure stats in real time.
    // sleep(1);

    if (bouquetRequest.onlineSale) {
        Logger::info("Seller # " + this->_sellerId + " just received an online purchase for " +
                     std::to_string(bouquetRequest.rosesAmount) + " roses and " +
                     std::to_string(bouquetRequest.tulipsAmount) + " tulips.");
        writeDeliveryNote(bouquetRequest);
    } else {
        Logger::info("Seller # " + this->_sellerId + " received an in place request for " +
                     std::to_string(bouquetRequest.rosesAmount) + " roses and " +
                     std::to_string(bouquetRequest.tulipsAmount) + " tulips.");
    }

    if ((this->_rosesStock.size() < bouquetRequest.rosesAmount ||
         this->_tulipsStock.size() < bouquetRequest.tulipsAmount) && _distributionPipeIsOpen) {
        resupply(bouquetRequest);
        return;
    }

    for (int i = 0; i < bouquetRequest.rosesAmount && !_rosesStock.empty(); i++) {
        Flower flower = _rosesStock.back();
        Logger::info("A rose from Producer #" + flower.producerId + " (" + flower.producerName + ") was sold by Seller #" + this->_sellerId);
        _rosesStock.pop_back();

        // Adding information to Stats Center.
        StatsCenter::addSale(flower, ROSE);
        Logger::debug("Seller #" + this->_sellerId + " added a rose to the Stats Center.");
    }

    for (int i = 0; i < bouquetRequest.tulipsAmount && !_tulipsStock.empty(); i++) {
        Flower flower = _tulipsStock.back();
        Logger::info("A tulip from Producer #" + flower.producerId + " (" + flower.producerName + ") was sold by Seller #" + this->_sellerId);
        _rosesStock.pop_back();

        // Adding information to Stats Center.
        StatsCenter::addSale(flower, TULIP);
        Logger::debug("Seller #" + this->_sellerId + " added a tulip to the Stats Center.");
    }
}

void SellerJob::resupply(BouquetRequest request) {
    unsigned int rosesBoxAmount =
            div((int) (request.rosesAmount - _rosesStock.size()), CLASSIFIER_BOX_SIZE).quot + 1;
    unsigned int tulipsBoxAmount =
            div((int) (request.tulipsAmount - _tulipsStock.size()), CLASSIFIER_BOX_SIZE).quot + 1;

    SellerRequest sellerRequest = SellerRequest(_sellerId, rosesBoxAmount, tulipsBoxAmount);
    ssize_t wroteAmount = _requestPipe->write(sellerRequest.serialize());
    if (wroteAmount == -1) {
        Logger::error("Seller # " + this->_sellerId +
                      " tried to ask for a resupply to the distribution center while the requests pipe is closed.");
        return;
    }

    unsigned int receivedBoxes = 0;
    while (_distributionPipeIsOpen && receivedBoxes < (rosesBoxAmount + tulipsBoxAmount)) {
        std::string serializedCb;
        int status;
        int readAmount = _distributionPipe->read(serializedCb, &status);

        if (readAmount == -1) {
            Logger::debug("Seller # " + this->_sellerId + " : broken pipe.");
            return;
        }

        if (readAmount == 0) {
            Logger::debug(
                    "Seller # " + this->_sellerId + " could not resupply due to the distributor pipe being closed.");
            _distributionPipeIsOpen = false;
            return;
        }

        if (status == EXIT_SUCCESS) {
            ClassifierBox cb = ClassifierBox::deserialize(serializedCb);
            switch (cb.flowerType) {
                case ROSE:
                    for (const auto &flower: cb._flowers) {
                        _rosesStock.push_back(flower);
                    }
                    break;
                case TULIP:
                    for (const auto &flower: cb._flowers) {
                        _tulipsStock.push_back(flower);
                    }
                    break;
            }
            receivedBoxes++;
        }
    }
    Logger::debug("Seller # " + this->_sellerId + " stock after resupply:" +
                  "[rose flowers: " + std::to_string(_rosesStock.size()) + " | tulips flowers: " +
                  std::to_string(_tulipsStock.size()) + "]");
}

int SellerJob::finish() {
    int processStatus;

    // Awaiting for client simulator process.
    waitpid(this->_clientSimulatorPID, &processStatus, 0);

    if (processStatus != EXIT_SUCCESS) {
        Logger::error("Client simulator in process " + std::to_string(this->_clientSimulatorPID) +
                      " finished with error code " + std::to_string(processStatus));
    } else {
        Logger::info("Client Simulator #" + this->_sellerId + " successfully ended without errors.");
    }

    this->closePipes();
}

void SellerJob::writeDeliveryNote(BouquetRequest request) {
    // Creating Delivery Note folders.
    DIR* directory = opendir(deliveryNoteFolder.c_str());
    if (directory == nullptr && mkdir(deliveryNoteFolder.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != EXIT_SUCCESS) {
        Logger::error("Couldn't create Delivery Notes folder!");
    } else {
        closedir(directory);
    }

    std::ofstream deliveryNoteFile;
    std::string deliveryNoteFileName = deliveryNoteFolder + "delivery" + _sellerId + "-" + std::to_string(_deliveryNoteNumber) + ".txt";
    Logger::debug("REMITO: " + deliveryNoteFileName);
    deliveryNoteFile.open(deliveryNoteFileName);
    deliveryNoteFile << " Delivery Note #" << _deliveryNoteNumber << std::endl;
    deliveryNoteFile << "-----------------------------------" << std::endl;
    deliveryNoteFile << " Flower Shop #" << _sellerId << std::endl;
    deliveryNoteFile << "-----------------------------------" << std::endl;
    deliveryNoteFile << " Roses:                          " << request.rosesAmount << std::endl;
    deliveryNoteFile << " Tulips:                         " << request.tulipsAmount << std::endl;
    deliveryNoteFile << "-----------------------------------" << std::endl;
    deliveryNoteFile.close();
    deliveryNoteFile.clear();
    _deliveryNoteNumber++;
}

std::string SellerJob::contextState() {
    std::string state = 'S' + this->_sellerId + ',';

    for (auto rose : this->_rosesStock) {
        state += rose.serialize() + '!';
    }

    state += ',';

    for (auto tulip : this->_tulipsStock) {
        state += tulip.serialize() + '!';
    }

    return state;
}

int SellerJob::stopJob() {
    Logger::info("Seller Job #" + this->_sellerId + " saved a stock of " + std::to_string(this->_rosesStock.size())
                 + " roses and " + std::to_string(this->_tulipsStock.size()) + " tulips.");
    ContextStatus::saveContext(this->contextState());

    // Finishing job.
    this->finish();
    this->closePipes();

    return EXIT_SUCCESS;
}

void SellerJob::initializeStatus(const string& sellerId) {

    std::string previousState = ContextStatus::retrieveContext('S' + sellerId);

    Logger::warn("Seller #" + sellerId + ". Previous state: " + previousState);

    if (previousState.empty()) {
        Logger::info("Creating new state for Seller #" + sellerId);
        this->_rosesStock = std::vector<Flower>();
        this->_tulipsStock = std::vector<Flower>();
    } else {
        Logger::info("Load previous state for Seller #" + sellerId);

        try {
            this->loadPreviousState(previousState);
        } catch (const std::exception &e) {
            Logger::error("Seller #" + sellerId + " failed to load previous state due to: " + e.what());
            this->_rosesStock = std::vector<Flower>();
            this->_tulipsStock = std::vector<Flower>();
        }
    }

}

void SellerJob::loadPreviousState(const string& previousState) {

    std::string buffer;
    std::vector<std::string> flowers;
    std::vector<std::vector<std::string>> boxes;

    for (auto character : previousState) {

        if (character == '!') {
            flowers.push_back(buffer);
            buffer = "";
        } else if (character == ',') {
            boxes.push_back(flowers);
            flowers.clear();
        } else {
            buffer += character;
        }

    }

    boxes.push_back(flowers);

    this->_rosesStock = std::vector<Flower>();
    this->_tulipsStock = std::vector<Flower>();

    for (const auto& rose : boxes[0]) {
        this->_rosesStock.push_back(Flower::deserialize(rose));
    }

    for (const auto& tulip : boxes[1]) {
        this->_tulipsStock.push_back(Flower::deserialize(tulip));
    }

    Logger::info("Seller Job #" + this->_sellerId + " retrieved a stock of " + std::to_string(this->_rosesStock.size())
                 + " roses and " + std::to_string(this->_tulipsStock.size()) + " tulips.");

}

void SellerJob::closePipes() {
    this->_clientPipe->~Pipe();
    Logger::info("Client Simulator pipe in Seller #" + this->_sellerId + " destroyed.");

    this->_distributionPipe->~Pipe();
    Logger::info("Distribution pipe in Seller #" + this->_sellerId + " destroyed.");

    this->_requestPipe->~Pipe();
    Logger::info("Request pipe in Seller #" + this->_sellerId + " destroyed.");
}
