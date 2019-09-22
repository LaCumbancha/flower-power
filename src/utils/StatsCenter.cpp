#include "StatsCenter.h"

Pipe *StatsCenter::_statsPipe = new Pipe();
Pipe *StatsCenter::_sellerPipe = new Pipe();
std::vector<Flower> StatsCenter::_roses = std::vector<Flower>();
std::vector<Flower> StatsCenter::_tulips = std::vector<Flower>();

void StatsCenter::run() {
    std::string data;
    int status;

    // Reading incoming stats.
    StatsCenter::_sellerPipe->setReadMode();
    StatsCenter::_statsPipe->setWriteMode();
    while (StatsCenter::_sellerPipe->read(data, &status)) {
        if (status == EXIT_SUCCESS) {
            if (isSaleIncoming(data)) {
                StatsCenter::updateStats(data);
            } else if (isRequestIncoming(data)) {
                StatsCenter::outputStats(data);
            }
        }
    }

    Logger::info("Closing Stats Center.");
    exit(EXIT_SUCCESS);
}

void StatsCenter::updateStats(const std::string &flowerData) {
    auto flower = Flower::deserialize(flowerData.substr(4, flowerData.size()));
    if (isRoseIncoming(flowerData)) {
        StatsCenter::_roses.push_back(flower);
    } else if (isTulipIncoming(flowerData)) {
        StatsCenter::_tulips.push_back(flower);
    }
}

void StatsCenter::outputStats(const std::string &request) {
    if (isProducerRequest(request)) {

        std::map<std::string, int> flowersByProducers;

        for (const auto &rose : StatsCenter::_roses) {
            if (flowersByProducers.find(rose.producerId) == flowersByProducers.end()) {
                flowersByProducers.insert(std::pair<std::string, int>(rose.producerId, 1));
            } else {
                flowersByProducers.find(rose.producerId)->second += 1;
            }
        }

        for (const auto &tulip : StatsCenter::_tulips) {
            if (flowersByProducers.find(tulip.producerId) == flowersByProducers.end()) {
                flowersByProducers.insert(std::pair<std::string, int>(tulip.producerId, 1));
            } else {
                flowersByProducers.find(tulip.producerId)->second += 1;
            }
        }

        std::string bestProducerId;
        int maxSold = 0;
        for (const auto &producer : flowersByProducers) {
            if (producer.second >= maxSold) {
                maxSold = producer.second;
                bestProducerId = producer.first;
            }
        }

        std::string bestProducerName;
        for (const auto& flower : StatsCenter::_roses) {
            if (flower.producerId == bestProducerId) bestProducerName = bestProducerId;
        }

        for (const auto& flower : StatsCenter::_tulips) {
            if (flower.producerId == bestProducerId) bestProducerName = bestProducerId;
        }

        if (maxSold == 0) {
            StatsCenter::_statsPipe->write("No producer has sold any flower #macrisis.");
        } else {
            StatsCenter::_statsPipe->write("Producer #" + bestProducerId + " (" + bestProducerName + ") is the producer" +
                                           " with most flower sold, with " + std::to_string(maxSold) + " units.");
        }

    } else if (isFlowerRequest(request)) {

        if (StatsCenter::_roses.size() > StatsCenter::_tulips.size()) {
            StatsCenter::_statsPipe->write(
                    "The roses are the most sold flowers with " + std::to_string(StatsCenter::_roses.size()) +
                    " units.");
        } else if (StatsCenter::_tulips.size() > StatsCenter::_roses.size()) {
            StatsCenter::_statsPipe->write(
                    "The tulips are the most sold flowers with " + std::to_string(StatsCenter::_tulips.size()) +
                    " units.");
        } else {
            StatsCenter::_statsPipe->write(
                    "Both roses and tulips are evenly sold with " + std::to_string(StatsCenter::_tulips.size()) +
                    " units.");
        }

    }
}

void StatsCenter::addSale(Flower flower, FlowerType type) {
    if (type == ROSE) StatsCenter::_sellerPipe->write("F|R|" + flower.serialize());
    else if (type == TULIP) StatsCenter::_sellerPipe->write("F|T|" + flower.serialize());
}

std::string StatsCenter::getMostSoldFlower() {
    StatsCenter::_sellerPipe->write("R|F|");

    std::string data;
    int status;

    StatsCenter::_statsPipe->read(data, &status);
    if (status == EXIT_SUCCESS) return data;

    Logger::error("Stats Center failed to retrieve most sold flower. Current answer: " + data);
    return "Error";
}

std::string StatsCenter::getMostSoldProducer() {
    StatsCenter::_sellerPipe->write("R|P|");

    std::string data;
    int status;

    StatsCenter::_statsPipe->read(data, &status);
    if (status == EXIT_SUCCESS) return data;

    Logger::error("Stats Center failed to retrieve most sold producer. Current answer: " + data);
    return "Error";
}

void StatsCenter::addingMode() {
    StatsCenter::_statsPipe->setReadMode();
    StatsCenter::_sellerPipe->setWriteMode();
}

bool StatsCenter::isSaleIncoming(const std::string &data) {
    return data.substr(0, 2) == "F|";
}

bool StatsCenter::isRequestIncoming(const std::string &data) {
    return data.substr(0, 2) == "R|";
}

bool StatsCenter::isRoseIncoming(const std::string &flower) {
    return flower.substr(2, 2) == "R|";
}

bool StatsCenter::isTulipIncoming(const std::string &flower) {
    return flower.substr(2, 2) == "T|";
}

bool StatsCenter::isProducerRequest(const std::string &request) {
    return request.substr(2, 2) == "P|";
}

bool StatsCenter::isFlowerRequest(const std::string &request) {
    return request.substr(2, 2) == "F|";
}

void StatsCenter::close() {
    StatsCenter::_statsPipe->~Pipe();
    StatsCenter::_sellerPipe->~Pipe();
}
