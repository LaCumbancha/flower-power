#include <cstring>
#include "ProducerJob.h"
#include "../../../utils/signals/SignalHandler.h"
#include "../../../utils/signals/StopHandler.h"

ProducerJob::ProducerJob(const int centerId, const FlowerBox *producerData, Pipe *producerPipe) : Job() {

    try {


    // Registering SIGTERM handler.
    auto handler = new StopHandler(this);
    SignalHandler::getInstance()->registerHandler(SIGTERM, handler);

    // Assigning pipe to communicate with the distribution center.
    this->_producerPipe = producerPipe;

    // Initializing distribution center data.
    this->_centerId = centerId;

    // Initializing producer data.
    this->_producerId = producerData->producerId;
    this->_producerName = producerData->producerName;
    this->initializeStatus(producerData);

    Logger::debug("Producer #" + std::to_string(this->_centerId) + "." + std::to_string(this->_producerId) + " (" +
                  this->_producerName + ") current stock: " + std::to_string(this->_rosesStock) + " roses and " +
                  std::to_string(this->_tulipsStock) + " tulips.");

    // Initializing random number generator
    srand(time(NULL) * getpid());
    } catch (const std::exception& e) {
        std::cerr << "ProducerJob construction exception: " << e.what() << std::endl;
    }
}

FlowerBox ProducerJob::generateFlowerBox() {

    // Uncomment the following line to measure stats in real time.
    // sleep(1);

    // Take a random number of roses.
    int rosesToBox = rand() % 10;
    if (this->_rosesStock < rosesToBox) {
        rosesToBox = this->_rosesStock;
    }

    // Complete with tulips (if there are any).
    int tulipsToBox = 10 - rosesToBox;
    if (this->_tulipsStock < tulipsToBox) {
        tulipsToBox = this->_tulipsStock;
    }

    // If there is enough space, put more roses in box.
    if (rosesToBox + tulipsToBox < 10) {
        if (this->_rosesStock >= 10 - tulipsToBox) {
            rosesToBox = 10 - tulipsToBox;
        } else {
            rosesToBox = this->_rosesStock;
        }
    }

    // Decrease stock.
    this->_tulipsStock = this->_tulipsStock - tulipsToBox;
    this->_rosesStock = this->_rosesStock - rosesToBox;

    Logger::debug("Producer #" + std::to_string(this->_centerId) + "." + std::to_string(this->_producerId) + " (" +
                  this->_producerName + ") current stock: " + std::to_string(this->_rosesStock) + " roses and " +
                  std::to_string(this->_tulipsStock) + " tulips.");

    // Create box.
    return FlowerBox(this->_producerId, this->_producerName, rosesToBox, tulipsToBox);

}

int ProducerJob::run() {
    bool producerPipeIsOpen = true;
    while (producerPipeIsOpen && (this->_rosesStock != 0 || this->_tulipsStock != 0)) {
        auto box = this->generateFlowerBox();

        Logger::info("Producer #" + std::to_string(this->_centerId) + "." + std::to_string(this->_producerId) + " (" +
                     this->_producerName + ") sent a box with " + std::to_string(box.rosesStock) + " roses and " +
                     std::to_string(box.tulipsStock) + " tulips to the Distribution Center #" +
                     std::to_string(this->_centerId) + ".");
        int wroteAmount = this->_producerPipe->write(box.serialize());
        if (wroteAmount == -1) {
            producerPipeIsOpen = false;
        }
    }

    return EXIT_SUCCESS;
}

int ProducerJob::finish() {
    delete this->_producerPipe;
    Logger::info("Producer #" + std::to_string(this->_centerId) + "." + std::to_string(this->_producerId) + " (" +
                 this->_producerName + ") pipe destroyed.");

    exit(EXIT_SUCCESS);
}

ProducerJob::~ProducerJob() {
    this->finish();
}

std::string ProducerJob::contextState() {
    return 'P' + std::to_string(this->_centerId) + '.' + std::to_string(this->_producerId) + ',' +
           std::to_string(this->_rosesStock) + ',' + std::to_string(this->_tulipsStock);
}

int ProducerJob::stopJob() {
    Logger::debug("Producer Job #" + std::to_string(this->_centerId) + "." + std::to_string(this->_producerId) +
                  " saved a stock of " + std::to_string(this->_rosesStock) + " roses and "
                  + std::to_string(this->_tulipsStock) + " tulips.");

    ContextStatus::saveContext(this->contextState());
    delete this;
    return EXIT_SUCCESS;
}

void ProducerJob::initializeStatus(const FlowerBox* producerData) {

    try {
        std::string producerId = std::to_string(this->_centerId) + '.' + std::to_string(this->_producerId);
        std::string previousState = ContextStatus::retrieveContext('P' + producerId);

        if (previousState.empty()) {
            Logger::info("Creating new state for Producer #" + producerId);
            this->_rosesStock = producerData->rosesStock;
            this->_tulipsStock = producerData->tulipsStock;
        } else {
            Logger::info("Load previous state for Producer #" + producerId);
            this->loadPreviousState(previousState);
        }
    } catch (const std::exception &e) {
        std::cerr << "Producer Job initializeStatus exception: " << e.what() << std::endl;
    }
}

void ProducerJob::loadPreviousState(const string& previousState) {
    std::string buffer;
    std::vector<std::string> values;

    try {
        for (auto character : previousState) {

            if (character == ',') {
                values.push_back(buffer + '\0');
                buffer = "";
            } else {
                buffer += character;
            }

        }

        values.push_back(buffer);

        Logger::debug("Producer Job #" + std::to_string(this->_centerId) + "." + std::to_string(this->_producerId) +
                      " retrieved a stock of " + values[0] + " roses and " + values[1] + " tulips.");

        this->_rosesStock = std::stoi(values[0]);
        this->_tulipsStock = std::stoi(values[1]);
    } catch (const std::invalid_argument& e) {
        std::cerr << "Producer Job loadPreviousState invalid argument exception: " << e.what() << std::endl;
        std::cerr << values[0] << " - " << values[1] << std::endl;
        std::cerr << previousState << std::endl;
    }
}
