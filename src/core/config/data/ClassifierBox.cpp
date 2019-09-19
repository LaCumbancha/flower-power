//
// Created by darius on 18/9/19.
//

#include "ClassifierBox.h"

std::string ClassifierBox::serialize() {
    std::string body;

    body = std::to_string(flowerType) + "|";
    for (const auto& flower : _flowers) {
        body += std::to_string(flower.producerId)
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

    auto flowerType = (FlowerType)std::stoi(values[1]);

    std::vector<Flower> flowers;
    for(auto iter = values.begin() + 2; iter < values.end(); iter += 2) {
        Flower flower = Flower(std::stoi(*iter), *(iter+1));
        flowers.push_back(flower);
    }
    return ClassifierBox(flowerType, flowers);
}
