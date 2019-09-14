#include "ProducerJob.h"

ProducerJob::ProducerJob(const ProducerFlowers& producerData, Pipe* distributionPipe) : Job() {

    // Assigning pipe to communicate with the distribution center.
    this->_distributionPipe = distributionPipe;

    // Initializing producer data.
    this->_producerId = producerData.producerId;
    this->_producerName = producerData.producerName;
    this->_rosesStock = producerData.rosesStock;
    this->_tulipsStock = producerData.tulipsStock;

    // Initializing random number generator
    srand(time(NULL) * getpid());

}

ProducerFlowers ProducerJob::generateFlowerBox() {

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

    // Create box.
    return ProducerFlowers(this->_producerId, this->_producerName, rosesToBox, tulipsToBox);

}

int ProducerJob::run() {
    while (this->_rosesStock != 0 or this->_tulipsStock != 0) {
        this->_distributionPipe->write(this->generateFlowerBox().serialize());
    }

    exit(EXIT_SUCCESS);

}
