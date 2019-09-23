#include "Flower.h"

Flower::Flower(std::string producerId, std::string producerName) :
    producerId(producerId), producerName(std::move(producerName)) {}

std::string Flower::serialize() {
    std::string data;
    data += this->producerId;
    data += "|";
    data += this->producerName;
    data += "|";

    return data;
}

Flower Flower::deserialize(const std::string &data) {
    std::vector<std::string> values;
    std::string value;

    for (auto character : data) {
        if (character != '|') {
            value += character;
        } else {
            if (!value.empty()) {
                values.push_back(value);
                value = "";
            }
        }
    }

    std::string producerId = values[0];
    std::string producerName = values[1];

    return Flower(producerId, producerName);
}
