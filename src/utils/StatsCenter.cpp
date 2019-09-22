#include "StatsCenter.h"

Pipe* StatsCenter::_statsPipe = new Pipe();
Pipe* StatsCenter::_sellerPipe = new Pipe();

void StatsCenter::run() {
    std::string data;
    int status;

    // Reading incoming stats.
    StatsCenter::_sellerPipe->setReadMode();
    StatsCenter::_statsPipe->setWriteMode();
    while(StatsCenter::_sellerPipe->read(data, &status)){
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

void StatsCenter::updateStats(const std::string& flower) {
    if (isRoseIncoming(flower)) {
        // TODO: Add rose.
    } else if (isTulipIncoming(flower)) {
        // TODO: Add tulip.
    }
}

void StatsCenter::outputStats(const std::string& request) {
    if (isProducerRequest(request)) {
        // TODO: Calculate best producer.
        StatsCenter::_statsPipe->write("Ningún producto vendió nada porque #macrisis.");
    } else if (isFlowerRequest(request)) {
        // TODO: Calculate best flower.
        StatsCenter::_statsPipe->write("No se vendió ninguna flor porque #macrisis.");
    }
}

void StatsCenter::addSale(Flower flower, FlowerType type) {
    if (type == ROSE) StatsCenter::_sellerPipe->write("R|" + flower.serialize());
    else if (type == TULIP) StatsCenter::_sellerPipe->write("T|" + flower.serialize());
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

bool StatsCenter::isSaleIncoming(const std::string& data) {
    return data.substr(0, 2) == "F|";
}

bool StatsCenter::isRequestIncoming(const std::string& data) {
    return data.substr(0, 2) == "R|";
}

bool StatsCenter::isRoseIncoming(const std::string& flower) {
    return flower.substr(2, 2) == "R|";
}

bool StatsCenter::isTulipIncoming(const std::string& flower) {
    return flower.substr(2, 2) == "T|";
}

bool StatsCenter::isProducerRequest(const std::string& request) {
    return request.substr(2, 2) == "P|";
}

bool StatsCenter::isFlowerRequest(const std::string& request) {
    return request.substr(2, 2) == "F|";
}
