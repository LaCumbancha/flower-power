#include "Config.h"
#include "../../utils/CSVReader.h"


int Config::getSalePoints() {
    return _salePoints;
}

int Config::getDistributionCenters() {
    return _distributionCenters;
}

std::vector<ProducerDTO> Config::getProducers() {
    return _producers;
}

void Config::setSalePoints(int salePoints) {
    this->_salePoints = salePoints;
}

void Config::setDistributionCenters(int distributionCenters) {
    this->_distributionCenters = distributionCenters;
}

void Config::setProducers(std::vector<ProducerDTO> producersDTO) {
    this->_producers = std::move(producersDTO);
}

void Config::loadData() {

    // Loading producers from CSV.
    auto reader = CSVReader(this->_producersPath);
    std::vector<std::vector<std::string>> producersData = reader.getData();

    // Parsing producers.
    std::vector<ProducerDTO> producers;
    for (auto producerData: producersData) {
        ProducerDTO producerDTO;
        producerDTO.producerId = std::stoi(producerData[0]);
        producerDTO.producerName = producerData[1];
        producerDTO.rosesStock = std::stoi(producerData[2]);
        producerDTO.tulipsStock = std::stoi(producerData[3]);
        producers.push_back(producerDTO);
    }

    // Storing producers.
    this->_producers = producers;

}
