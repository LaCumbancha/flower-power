#include <iostream>
#include "ClassifierBox.h"
#include "../../../utils/Logger.h"

std::string ClassifierBox::serialize() {
    std::string body;

    body = std::to_string(flowerType) + "|";
    for (const auto& flower : _flowers) {
        body += flower.producerId
                + "|"
                + flower.producerName
                + "|";
    }

    return body;
}

ClassifierBox ClassifierBox::deserialize(const std::string& serializedCB) {
    std::vector<std::string> values;
    std::string value;

    for (auto character : serializedCB) {
        if (character != '|') {
            value += character;
        } else {
            if (!value.empty()) {
                values.push_back(value);
                value = "";
            }
        }
    }

    auto flowerType = (FlowerType) std::stoi(values[0]);
    std::vector<Flower> flowers;
    try {
        for (unsigned long i = 1; i < values.size(); i += 2) {
            Flower flower = Flower(values.at(i), values.at(i+1));
            flowers.push_back(flower);
        }
        return ClassifierBox(flowerType, flowers);
    } catch (std::exception& e) {
        Logger::error("Classifier box deserialization error: " + std::string(e.what()) + " -- serializedCB: " + serializedCB);
        return ClassifierBox(flowerType, flowers);
    }

}