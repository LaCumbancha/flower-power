#include "Config.h"
#include "../../utils/Logger.h"


int Config::getClients() {
    return this->_clients;
}

void Config::setClients(int clients) {
    this->_clients = clients;
}

int Config::getSalePoints() {
    return this->_salePoints;
}

void Config::setSalePoints(int salePoints) {
    this->_salePoints = salePoints;
}

int Config::getDistributionCenters() {
    return _distributionCenters;
}

void Config::setDistributionCenters(int distributionCenters) {
    this->_distributionCenters = distributionCenters;
}

std::vector<FlowerBox*> Config::getProducers() {
    return _producers;
}
void Config::setProducers(std::vector<FlowerBox*> producersDTO) {
    this->_producers = producersDTO;
}

void Config::loadData() {
    try {
        // Loading producers from CSV.
        auto reader = CSVReader(this->_producersPath);
        std::vector<std::vector<std::string>> producersData = reader.getData();

        // Parsing and storing producers.
        for (auto producerData: producersData) {
            int producerId = std::stoi(producerData[0]);
            std::string producerName = producerData[1];
            int rosesStock = std::stoi(producerData[2]);
            int tulipsStock = std::stoi(producerData[3]);
            this->_producers.push_back(new FlowerBox(producerId, producerName, rosesStock, tulipsStock));
        }
    } catch (const std::exception& e){
        std::cerr << "Config loadData exception: " << e.what() << std::endl;
    }
}
