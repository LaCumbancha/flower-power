#include "BouquetRequest.h"

BouquetRequest::BouquetRequest(int rosesAmount, int tulipsAmount) :
        rosesAmount(rosesAmount), tulipsAmount(tulipsAmount) {}

std::string BouquetRequest::serialize() {
    std::string packet;
    std::string body;
    std::string head;

    body = std::to_string(rosesAmount) + "|" + std::to_string(tulipsAmount) + "|";
    head = "|" + std::to_string(body.length()) + "|";
    packet = head + body;

    return packet;
}

BouquetRequest BouquetRequest::deserialize(const std::string& data) {
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

    int rosesAmount = std::stoi(values[0]);
    int tulipsAmount = std::stoi(values[1]);

    return BouquetRequest(rosesAmount, tulipsAmount);

}
