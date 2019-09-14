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
    std::string data;
    int status;

    while(this->_producersPipe->read(data, &status)){
        if (status == EXIT_SUCCESS) {
            ProducerFlowers box = ProducerFlowers::deserialize(data);
            std::cout << "Recibí una caja con " << box.rosesStock << " rosas y " << box.tulipsStock << " tulipanes del productor " << box.producerName << " (" << box.producerId << ")." << std::endl;
        }
    }

    wait(nullptr);
    exit(EXIT_SUCCESS);
}
