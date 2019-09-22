#include "DistributionCenter.h"
#include "job/distributor/DistributorJob.h"

DistributionCenter::DistributionCenter(Config *config, int id) : Job() {

    // Setting configurations.
    this->_id = id;
    this->_config = config;

    // Getting current pid
    pid_t pid;

    /*********** Producer processes creation. **********/
    // Creating pipe for producers.
    auto producersPipe = new Pipe();
    for (auto producerData : config->getProducers()) {

        std::string producerId = std::to_string(this->_id) + "." + std::to_string(producerData->producerId);
        pid = fork();

        if (pid == CHILD_PROCESS_PID) {
            // Child process.
            producersPipe->setWriteMode();
            auto producerJob = new ProducerJob(this->_id, producerData, producersPipe);
            producerJob->run();
            producerJob->finish();
        }
        this->_producersPIDs.push_back(pid);
        Logger::info("Producer #" + producerId + " running in process with PID #" + std::to_string(pid) + ".");
    }
    producersPipe->setReadMode();
    this->_producersPipe = producersPipe;

    /*********** Seller processes creation. **********/
    // Creating pipe for sellers' requests.
    auto requestsPipe = new Pipe();
    int salePoints = config->getSalePoints();
    for (int salePoint = 1 ; salePoint <= salePoints ; salePoint++) {
        // Creating pipe for sellers' stock distribution.
        auto distributionPipe = new Pipe();

        std::string sellerId = std::to_string(this->_id) + "." + std::to_string(salePoint);
        pid = fork();

        if (pid == CHILD_PROCESS_PID) {
            // Child process.
            requestsPipe->setWriteMode();
            distributionPipe->setReadMode();
            auto sellerJob = new SellerJob(sellerId, config->getClients(), requestsPipe, distributionPipe);
            sellerJob->run();
            sellerJob->finish();
        }
        this->_distributionPipes.insert(std::pair<std::string, Pipe*>(sellerId, distributionPipe));
        Logger::info("Seller #" + sellerId + " running in process with PID #" + std::to_string(pid) + ".");
        this->_sellersPIDs.push_back(pid);
    }
    requestsPipe->setReadMode();
    this->_requestsPipe = requestsPipe;

    /******** Classifier-Distributor pipe creation *********/
    this->_innerPipe = new Pipe();
}

int DistributionCenter::run() {

    /*********** Classifier processes creation. **********/
    pid_t pid = fork();

    if (pid == CHILD_PROCESS_PID) {
        // Child process.
        this->_innerPipe->setWriteMode();
        auto classifierJob = new ClassifierJob(this->_id, this->_producersPipe, this->_innerPipe);
        classifierJob->run();
        classifierJob->finish();
    }

    Logger::info("Classifier #" + std::to_string(this->_id) + " running in process with PID #" + std::to_string(pid) + ".");
    this->_classifierPID = pid;

    /********** Distributor process creation *************/
    pid = fork();
    if (pid == CHILD_PROCESS_PID) {
        this->_innerPipe->setReadMode();
        auto distributorJob = new DistributorJob(this->_id, _innerPipe, _requestsPipe, _distributionPipes);
        distributorJob->run();
        distributorJob->finish();
    }

    this->closePipes();

    return EXIT_SUCCESS;
}

int DistributionCenter::finish() {
    int signals, processStatus;

    // Awaiting for producers processes.
    signals = 0;
    for (auto producerPID : this->_producersPIDs) {
        waitpid(producerPID, &processStatus, 0);

        if (processStatus != EXIT_SUCCESS) {
            signals += 1;
            Logger::error("Producer in process " + std::to_string(producerPID) + " finished with error code " +
                          std::to_string(processStatus));
        }
    }

    if (signals == 0) {
        Logger::info("Every Producer in Center # " + std::to_string(this->_id) + " finished successfully without errors.");
    }

    // Awaiting for sellers processes.
    signals = 0;
    for (auto sellerPID : this->_sellersPIDs) {
        waitpid(sellerPID, &processStatus, 0);

        if (processStatus != EXIT_SUCCESS) {
            signals += 1;
            Logger::error("Seller in process " + std::to_string(sellerPID) + " finished with error code " +
                          std::to_string(processStatus));
        }
    }

    if (signals == 0) {
        Logger::info("Every Seller in Center # " + std::to_string(this->_id) + " finished successfully without errors.");
    }

    // Awaiting for classifier process.
    waitpid(this->_classifierPID, &processStatus, 0);

    if (processStatus != EXIT_SUCCESS) {
        Logger::error("Classifier in process " + std::to_string(this->_classifierPID) + " finished with error code " +
                      std::to_string(processStatus));
    } else {
        Logger::info("Classifier in Center #" + std::to_string(this->_id) + " successfully ended without errors.");
    }

    exit(EXIT_SUCCESS);
}

void DistributionCenter::closePipes() {
    // Closing Producers pipe.
    delete this->_producersPipe;
//    this->_producersPipe->~Pipe();
    Logger::info("Producers pipe in Distribution Center #" + std::to_string(this->_id) + " destroyed.");

    // Closing classifier - distributor pipe.
//    this->_innerPipe->~Pipe();
    delete this->_innerPipe;
    Logger::info("Classifier-distributor pipe in Distribution Center #" + std::to_string(this->_id) + " destroyed.");

    // Closing Requests pipe.
    delete this->_requestsPipe;
//    this->_requestsPipe->~Pipe();
    Logger::info("Requests pipe in Distribution Center #" + std::to_string(this->_id) + " destroyed.");

    // Closing Distribution pipes.
    int pipeIdx = 0;
    for (const auto& pipe : this->_distributionPipes) {
        pipeIdx++;
        delete pipe.second;
//        pipe.second->~Pipe();
        Logger::info("Distribution pipe #" + std::to_string(pipeIdx) + " in Distribution Center #" +
                     std::to_string(this->_id) + " destroyed.");
    }
}


