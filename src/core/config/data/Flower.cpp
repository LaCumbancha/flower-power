#include "Flower.h"

Flower::Flower(int producerId, std::string producerName) :
    producerId(producerId), producerName(std::move(producerName)) {}

std::string Flower::serialize() {
    std::string data;
    data += std::to_string(this->producerId);
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

    int producerId = std::stoi(values[0]);
    std::string producerName = values[1];

    return Flower(producerId, producerName);
}
