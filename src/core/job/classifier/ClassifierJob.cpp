#include "ClassifierJob.h"
#include "../../config/data/ClassifierBox.h"
#include "../../../utils/signals/StopHandler.h"
#include "../../../utils/signals/SignalHandler.h"

ClassifierJob::ClassifierJob(const int center, Pipe *producersPipe, Pipe* distributorPipe) : Job() {

    // Registering SIGTERM handler.
    auto handler = new StopHandler(this);
    SignalHandler::getInstance()->registerHandler(SIGTERM, handler);

    // Assigning pipe to communicate with the distribution center.
    this->_producersPipe = producersPipe;

    // Assigning pipe to communicate with the distributor
    this->_distributorPipe = distributorPipe;

    // Initializing distribution center data.
    this->_center = center;

    // Initializing state.
    this->initializeStatus();

}

int ClassifierJob::run() {
    std::string data;
    int status;
    int readAmount;

    while ((readAmount = this->_producersPipe->read(data, &status)) > 0) {
        if (status == EXIT_SUCCESS) {

            // Uncomment the following line to measure stats in real time.
            // sleep(1);

            FlowerBox box = FlowerBox::deserialize(data);
            std::string producerId = std::to_string(this->_center) + "." + std::to_string(box.producerId);
            Logger::debug("Classifier #" + std::to_string(this->_center) + " received a box with " +
                         std::to_string(box.rosesStock) + " roses and " + std::to_string(box.tulipsStock) +
                         " tulips from provider #" + std::to_string(box.producerId) + " (" + box.producerName + ").");

            for (int rose = 1; rose <= box.rosesStock; rose++) {
                this->_roses.emplace_back(producerId, box.producerName);
                if (_roses.size() == CLASSIFIER_BOX_SIZE) {
                    ClassifierBox rosesBox = ClassifierBox(ROSE, _roses);
                    int wroteAmount = _distributorPipe->write(rosesBox.serialize());
                    if (wroteAmount == -1) {
                        Logger::info("Classifier #" + std::to_string(this->_center) +
                                     " just found the distributor pipe was closed!");
                        return EXIT_SUCCESS;
                    }
                    Logger::debug("Classifier #" + std::to_string(this->_center) +
                                 " sent a 'classifier box' of roses to the distributor #" +
                                 std::to_string(this->_center) + ": " + rosesBox.serialize());
                    _roses = std::vector<Flower>();
                }
            }

            for (int tulip = 1; tulip <= box.tulipsStock; tulip++) {
                this->_tulips.emplace_back(producerId, box.producerName);
                if (_tulips.size() == CLASSIFIER_BOX_SIZE) {
                    ClassifierBox tulipsBox = ClassifierBox(TULIP, _tulips);
                    int wroteAmount = _distributorPipe->write(tulipsBox.serialize());
                    if (wroteAmount == -1) {
                        Logger::info("Classifier #" + std::to_string(this->_center) +
                                     " just found the distributor pipe was closed!");
                        return EXIT_SUCCESS;
                    }
                    Logger::debug("Classifier #" + std::to_string(this->_center) +
                                  " sent a 'classifier box' of tulips to the distributor #" +
                                  std::to_string(this->_center) + ": " + tulipsBox.serialize());
                    _tulips = std::vector<Flower>();
                }
            }

            Logger::debug("Current stock in Distribution Center #" + std::to_string(this->_center) + ": "
                          + std::to_string(this->_roses.size()) + " roses and " + std::to_string(this->_tulips.size()) +
                          " tulips.");
        }
    }

    if (readAmount == -1) {
        Logger::error("Classifier #" + std::to_string(this->_center) + " could not keep receiving boxes from producers due to a pipe error.");
        return EXIT_FAILURE;
    }

    if (readAmount == 0) {
        Logger::debug("Classifier #" + std::to_string(this->_center) + " producers pipe got closed.");
    }

    return EXIT_SUCCESS;
}

int ClassifierJob::finish() {
    this->_producersPipe->~Pipe();
    Logger::info("Classifier #" + std::to_string(this->_center) + " pipe connected to producer's processes destroyed.");

    this->_distributorPipe->~Pipe();
    Logger::info("Classifier #" + std::to_string(this->_center) + " pipe connected to the distributor process destroyed.");

    exit(EXIT_SUCCESS);
}

std::string ClassifierJob::contextState() {
    std::string state = 'C' + std::to_string(this->_center) + ',';

    for (auto rose : this->_roses) {
        state += rose.serialize() + '!';
    }

    state += ',';

    for (auto tulip : this->_tulips) {
        state += tulip.serialize() + '!';
    }

    return state;
}

int ClassifierJob::stopJob() {
    Logger::info("Classifier Job #" + std::to_string(this->_center) + " saved a stock of "
                 + std::to_string(this->_roses.size()) + " roses and " + std::to_string(this->_tulips.size()) + " tulips.");
    ContextStatus::saveContext(this->contextState());

    // Finishing job.
    this->finish();

    return EXIT_SUCCESS;
}

void ClassifierJob::initializeStatus() {

    std::string previousState = ContextStatus::retrieveContext('C' + std::to_string(this->_center));
    Logger::warn("Classifier #" + std::to_string(this->_center) + ". Previous state: " + previousState);

    if (previousState.empty()) {
        Logger::info("Creating new empty state for Classifier #" + std::to_string(this->_center));
        this->_roses = std::vector<Flower>();
        this->_tulips = std::vector<Flower>();
    } else {
        Logger::info("Load previous state for Classifier #" + std::to_string(this->_center));

        try {
            this->loadPreviousState(previousState);
        } catch (const std::exception &e) {
            Logger::error("Classifier #" + std::to_string(this->_center) + " failed to load previous state due to: " + e.what());
            this->_roses = std::vector<Flower>();
            this->_tulips = std::vector<Flower>();
        }
    }

}

void ClassifierJob::loadPreviousState(const string& previousState) {

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

    this->_roses = std::vector<Flower>();
    this->_tulips = std::vector<Flower>();

    for (const auto& rose : boxes[0]) {
        this->_roses.push_back(Flower::deserialize(rose));
    }

    for (const auto& tulip : boxes[1]) {
        this->_tulips.push_back(Flower::deserialize(tulip));
    }

    Logger::info("Classifier Job #" + std::to_string(this->_center) + " retrieved a stock of "
                 + std::to_string(this->_roses.size()) + " roses and " + std::to_string(this->_tulips.size()) + " tulips.");

}
