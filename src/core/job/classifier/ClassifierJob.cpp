#include "ClassifierJob.h"
#include "../../config/data/ClassifierBox.h"

ClassifierJob::ClassifierJob(const int center, Pipe *producersPipe, Pipe* distributorPipe) : Job() {

    // Assigning pipe to communicate with the distribution center.
    this->_producersPipe = producersPipe;

    // Assigning pipe to communicate with the distributor
    this->_distributorPipe = distributorPipe;

    // Initializing distribution center data.
    this->_center = center;

}

int ClassifierJob::run() {
    std::string data;
    int status;

    while (this->_producersPipe->read(data, &status)) {
        if (status == EXIT_SUCCESS) {
            FlowerBox box = FlowerBox::deserialize(data);
            Logger::info("Classifier #" + std::to_string(this->_center) + " received a box with " +
                         std::to_string(box.rosesStock) + " roses and " + std::to_string(box.tulipsStock) +
                         " tulips from provider #" + std::to_string(box.producerId) + " (" + box.producerName + ").");

            for (int rose = 1; rose <= box.rosesStock; rose++) {
                this->_roses.emplace_back(box.producerId, box.producerName);
                if (_roses.size() == CLASSIFIER_BOX_SIZE) {
                    ClassifierBox rosesBox = ClassifierBox(ROSE, _roses);
                    _distributorPipe->write(rosesBox.serialize());
                    //TODO: Log
                    _roses = std::vector<Flower>();
                }
            }

            for (int tulip = 1; tulip <= box.tulipsStock; tulip++) {
                this->_tulips.emplace_back(box.producerId, box.producerName);
                if (_tulips.size() == CLASSIFIER_BOX_SIZE) {
                    ClassifierBox tulipsBox = ClassifierBox(TULIP, _tulips);
                    _distributorPipe->write(tulipsBox.serialize());
                    //TODO: Log
                    _tulips = std::vector<Flower>();
                }
            }

            Logger::debug("Current stock in Distribution Center #" + std::to_string(this->_center) + ": "
                          + std::to_string(this->_roses.size()) + " roses and " + std::to_string(this->_tulips.size()) +
                          " tulips.");
        }
    }

    return EXIT_SUCCESS;
}

int ClassifierJob::finish() {
    this->_producersPipe->~Pipe();
    Logger::info("Classifier #" + std::to_string(this->_center) + " pipe destroyed.");
    exit(EXIT_SUCCESS);
}
