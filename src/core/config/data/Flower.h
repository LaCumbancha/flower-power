#ifndef FLOWER_POWER_FLOWER_H
#define FLOWER_POWER_FLOWER_H


#include <string>
#include <vector>
#include <utility>

enum FlowerType {ROSE, TULIP};

class Flower {
public:
    explicit Flower(std::string producerId, std::string producerName);
    std::string producerId;
    std::string producerName;

    std::string serialize();
    static Flower deserialize(const std::string& data);

};


#endif //FLOWER_POWER_FLOWER_H
