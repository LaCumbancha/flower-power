#include "Config.h"
#include "../../utils/CSVReader.h"


int Config::getSalePoints() {
    return _salePoints;
}

int Config::getDistributionCenters() {
    return _distributionCenters;
}

std::vector<ProducerFlowers> Config::getProducers() {
    return _producers;
}

void Config::setSalePoints(int salePoints) {
    this->_salePoints = salePoints;
}

void Config::setDistributionCenters(int distributionCenters) {
    this->_distributionCenters = distributionCenters;
}

void Config::setProducers(std::vector<ProducerFlowers> producersDTO) {
    this->_producers = std::move(producersDTO);
}

void Config::loadData() {

    // Loading producers from CSV.
    auto reader = CSVReader(this->_producersPath);
    std::vector<std::vector<std::string>> producersData = reader.getData();

    // Parsing producers.
    std::vector<ProducerFlowers> producers;
    for (auto producerData: producersData) {
        int producerId = std::stoi(producerData[0]);
        std::string producerName = producerData[1];
        int rosesStock = std::stoi(producerData[2]);
        int tulipsStock = std::stoi(producerData[3]);
        producers.push_back(ProducerFlowers(producerId, producerName, rosesStock, tulipsStock));
    }

    // Storing producers.
    this->_producers = producers;

}
