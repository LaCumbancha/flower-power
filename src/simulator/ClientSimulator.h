#ifndef FLOWER_POWER_CLIENTSIMULATOR_H
#define FLOWER_POWER_CLIENTSIMULATOR_H


#include <cstdlib>
#include <vector>
#include <iostream>
#include "../utils/Pipe.h"

struct BouquetRequest {
    int rosesAmount = 0;
    int tulipsAmount = 0;

    BouquetRequest(int rosesAmount, int tulipsAmount) : rosesAmount(rosesAmount), tulipsAmount(tulipsAmount) {}

    std::string serialize() {
        std::string packet;
        std::string body;
        std::string head;

        body = std::to_string(rosesAmount) + "|" + std::to_string(tulipsAmount) + "|";
        head = "|" + std::to_string(body.length()) + "|";
        packet = head + body;

        return packet;
    }

    explicit BouquetRequest(std::string serializedRequest) {
        std::string delimiter = "|";
        size_t pos = 0;
        std::string field;
        std::vector<std::string> fields;

        serializedRequest.erase(0, pos + delimiter.length());
        while ((pos = serializedRequest.find(delimiter)) != std::string::npos) {
            field = serializedRequest.substr(0, pos);
            fields.push_back(field);
            serializedRequest.erase(0, pos + delimiter.length());
        }

        rosesAmount = std::stoi(fields[1]);
        tulipsAmount = std::stoi(fields[2]);
    }
};

class ClientSimulator {

public:
    explicit ClientSimulator(int distributionCenterId, int sellerId, int sellerProcessId, Pipe *clientPipe);
    void run();

private:
    int _centerId;
    int _sellerId;
    int _sellerPID;
    Pipe * _clientPipe;

    BouquetRequest simulateBouquetRequest();

};


#endif //FLOWER_POWER_CLIENTSIMULATOR_H
