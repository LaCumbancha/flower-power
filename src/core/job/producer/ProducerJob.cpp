#include "ProducerJob.h"

ProducerJob::ProducerJob(const int centerId, const FlowerBox *producerData, Pipe *producerPipe) : Job() {

    // Assigning pipe to communicate with the distribution center.
    this->_producerPipe = producerPipe;

    // Initializing producer data.
    this->_producerId = producerData->producerId;
    this->_producerName = producerData->producerName;
    this->_rosesStock = producerData->rosesStock;
    this->_tulipsStock = producerData->tulipsStock;

    // Initializing distribution center data.
    this->_centerId = centerId;

    Logger::debug("Producer #" + std::to_string(this->_centerId) + "." + std::to_string(this->_producerId) + " (" +
                  this->_producerName + ") current stock: " + std::to_string(this->_rosesStock) + " roses and " +
                  std::to_string(this->_tulipsStock) + " tulips.");

    // Initializing random number generator
    srand(time(NULL) * getpid());

}

FlowerBox ProducerJob::generateFlowerBox() {

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

    ContextStorage::saveContext(this->contextState());

    while (this->_rosesStock != 0 or this->_tulipsStock != 0) {
        auto box = this->generateFlowerBox();

        Logger::info("Producer #" + std::to_string(this->_centerId) + "." + std::to_string(this->_producerId) + " (" +
                     this->_producerName + ") sent a box with " + std::to_string(box.rosesStock) + " roses and " +
                     std::to_string(box.tulipsStock) + " tulips to the Distribution Center #" +
                     std::to_string(this->_centerId) + ".");
        this->_producerPipe->write(box.serialize());
    }

    return EXIT_SUCCESS;
}

int ProducerJob::finish() {
    this->_producerPipe->~Pipe();
    Logger::info("Producer #" + std::to_string(this->_centerId) + "." + std::to_string(this->_producerId) + " (" +
                 this->_producerName + ") pipe destroyed.");

    exit(EXIT_SUCCESS);
}

std::string ProducerJob::contextState() {
    return 'P' + std::to_string(this->_centerId) + '.' + std::to_string(this->_producerId) + ',' +
           std::to_string(this->_rosesStock) + ',' + std::to_string(this->_tulipsStock);
}
