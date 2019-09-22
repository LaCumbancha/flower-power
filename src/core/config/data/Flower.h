#ifndef FLOWER_POWER_FLOWER_H
#define FLOWER_POWER_FLOWER_H


#include <string>
#include <utility>

enum FlowerType {ROSE, TULIP};

class Flower {

public:
    explicit Flower(int producerId, std::string producerName);

    int producerId;
    std::string producerName;

};


#endif //FLOWER_POWER_FLOWER_H
