#include "BouquetRequest.h"

BouquetRequest::BouquetRequest(int rosesAmount, int tulipsAmount, bool onlineSale) :
        rosesAmount(rosesAmount), tulipsAmount(tulipsAmount), onlineSale(onlineSale) {}

std::string BouquetRequest::serialize() {
    std::string packet;
    std::string body;
    std::string head;

    body = std::to_string(onlineSale) + "|" + std::to_string(rosesAmount) + "|" + std::to_string(tulipsAmount) + "|";
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

    bool onlineSale = std::stoi(values[1]);
    int rosesAmount = std::stoi(values[2]);
    int tulipsAmount = std::stoi(values[3]);

    return BouquetRequest(rosesAmount, tulipsAmount, onlineSale);

}
