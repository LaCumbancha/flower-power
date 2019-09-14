#include "ProducerFlowers.h"


ProducerFlowers::ProducerFlowers(int producerId, std::string producerName, int rosesStock, int tulipsStock) :
        producerId(producerId), producerName(std::move(producerName)), rosesStock(rosesStock), tulipsStock(tulipsStock) {}

std::string ProducerFlowers::serialize() {
    std::string data;
    data += std::to_string(this->producerId);
    data += "|";
    data += this->producerName;
    data += "|";
    data += std::to_string(this->rosesStock);
    data += "|";
    data += std::to_string(this->tulipsStock);
    data += "|";

    return data;
}

ProducerFlowers ProducerFlowers::deserialize(std::string data) {
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
    int rosesStock = std::stoi(values[2]);
    int tulipsStock = std::stoi(values[3]);

    return ProducerFlowers(producerId, producerName, rosesStock, tulipsStock);

}
