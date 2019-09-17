#include "ClassifierJob.h"

ClassifierJob::ClassifierJob(const int center, Pipe* producersPipe) : Job() {

    // Assigning pipe to communicate with the distribution center.
    this->_producersPipe = producersPipe;

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
                this->_roses.push_back(new Flower(box.producerId, box.producerName));
            }

            for (int tulip = 1; tulip <= box.tulipsStock; tulip++) {
                this->_tulips.push_back(new Flower(box.producerId, box.producerName));
            }

            Logger::debug("Current stock: " + std::to_string(this->_roses.size()) + " roses and " +
                          std::to_string(this->_tulips.size()) + " tulips.");
        }
    }

    exit(EXIT_SUCCESS);
}
