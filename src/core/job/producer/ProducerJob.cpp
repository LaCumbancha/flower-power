#include "ProducerJob.h"

ProducerJob::ProducerJob(const ProducerDTO& producerData, Pipe* distributionPipe) : Job() {

    // Assigning pipe to communicate with the distribution center.
    this->_distributionPipe = distributionPipe;

    // Initializing producer data.
    this->_id = producerData.producerId;
    this->_name = producerData.producerName;
    this->_rosesStock = producerData.rosesStock;
    this->_tulipsStock = producerData.tulipsStock;

}

int ProducerJob::run() {
    std::cout << "Running producer " << _name << " (" << _id << ") with " << _rosesStock << " roses and " << _tulipsStock << " tulips." << std::endl;
    exit(EXIT_SUCCESS);
}
