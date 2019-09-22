#include "SellerRequest.h"

std::string SellerRequest::serialize() {
    std::string packet;
    std::string body;
    std::string head;

    body = sellerId + "|" + std::to_string(rosesBoxAmount) + "|" + std::to_string(tulipsBoxAmount) + "|";
    head = "|" + std::to_string(body.length()) + "|";
    packet = head + body;

    return packet;
}

SellerRequest SellerRequest::deserialize(const std::string& data) {
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

    std::string sellerId = values[1];
    int rosesAmount = std::stoi(values[2]);
    int tulipsAmount = std::stoi(values[3]);

    return SellerRequest(sellerId, rosesAmount, tulipsAmount);
}
