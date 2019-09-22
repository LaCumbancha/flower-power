#include "StatsCenter.h"

Pipe* StatsCenter::_statsPipe = new Pipe();
Pipe* StatsCenter::_sellerPipe = new Pipe();

void StatsCenter::run() {
    std::string data;
    int status;

    // Reading incoming stats.
    StatsCenter::_sellerPipe->setReadMode();
    StatsCenter::_statsPipe->setWriteMode();
    while(StatsCenter::_statsPipe->read(data, &status)){
        if (status == EXIT_SUCCESS) {
            if (isSaleIncoming(data)) {
                StatsCenter::updateStats(data);
            } else if (isRequestIncoming(data)) {
                StatsCenter::wroteStats(data);
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

void StatsCenter::wroteStats(const std::string& request) {
    if (isProducerRequest(request)) {
        // TODO: Calculate best producer.
    } else if (isFlowerRequest(request)) {
        // TODO: Calculate best flower.
    }
}

void StatsCenter::addSale(Flower flower) {
    // TODO: Add flower data.
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

bool StatsCenter::isSaleIncoming(std::string data) {
    return false;
}

bool StatsCenter::isRequestIncoming(std::string data) {
    return false;
}

bool StatsCenter::isRoseIncoming(std::string flower) {
    return false;
}

bool StatsCenter::isTulipIncoming(std::string flower) {
    return false;
}

bool StatsCenter::isProducerRequest(std::string request) {
    return false;
}

bool StatsCenter::isFlowerRequest(std::string request) {
    return false;
}
