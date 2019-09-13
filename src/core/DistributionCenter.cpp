#include "DistributionCenter.h"

DistributionCenter::DistributionCenter(Config *config) : Job() {

    // Setting configurations.
    this->_config = config;

    // Getting current pid
    pid_t pid = getpid();

    // Creating pipe for producers.
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
    exit(EXIT_SUCCESS);
}
