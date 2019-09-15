//
// Created by darius on 15/9/19.
//

#include <iostream>
#include "ClientSimulator.h"

void ClientSimulator::Run() {

    for (int i = 0; i < 10; i++) {
        BouquetRequest request = SimulateBouquetRequest();
        std::cout << request.to_string() << std::endl;
    }
}

ClientSimulator::BouquetRequest ClientSimulator::SimulateBouquetRequest() {
    int rosesAmount = rand() % 10;
    int tulipsAmount = rand() % 10;
    BouquetRequest request(rosesAmount, tulipsAmount);
    return request;
}
