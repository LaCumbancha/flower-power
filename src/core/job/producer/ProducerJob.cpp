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
    ProducerDTO data;
    data.producerId = this->_id;
    data.rosesStock = this->_rosesStock;
    data.tulipsStock = this->_tulipsStock;

    this->_distributionPipe->write(static_cast<const void*>(&data), sizeof(data));
    exit(EXIT_SUCCESS);
}
