#include "DistributionCenter.h"
#include "../utils/Logger.h"

DistributionCenter::DistributionCenter(Config *config) : Job() {

    // Setting configurations.
    this->_config = config;

    // Getting current pid
    pid_t pid = getpid();

    // Creating logPipe for producers.
    auto producersPipe = new Pipe();

    for (const auto& producerData : config->getProducers()) {

        if (pid != CHILD_PROCESS_PID) {
            pid = fork();

            // Child process.
            if (pid == CHILD_PROCESS_PID) {
                producersPipe->setWriteMode();
                auto producerJob = new ProducerJob(producerData, producersPipe);
                producerJob->run();
            }
        }

    }

    producersPipe->setReadMode();
    this->_producersPipe = producersPipe;

}

pid_t DistributionCenter::run() {
    std::string data;
    int status;

    while(this->_producersPipe->read(data, &status)){
        if (status == EXIT_SUCCESS) {
            ProducerFlowers box = ProducerFlowers::deserialize(data);
            Logger::info("Distribution Center received a box with " + std::to_string(box.rosesStock) + " roses and " + std::to_string(box.tulipsStock) + " tulips from provider #" + std::to_string(box.producerId) + " (" + box.producerName + ").");
        }
    }

    wait(nullptr);
    exit(EXIT_SUCCESS);
}
