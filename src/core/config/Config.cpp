#include "Config.h"

#include <utility>
#include "../../utils/CSVReader.h"


std::vector<Seller> Config::getSalePoints() {
    return _sellers;
}

int Config::getDistributionCenters() {
    return _distributionCenters;
}

std::vector<ProducerFlowers> Config::getProducers() {
    return _producers;
}

void Config::setSellers(std::vector<Seller> sellers) {
    this->_sellers = std::move(sellers);
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
