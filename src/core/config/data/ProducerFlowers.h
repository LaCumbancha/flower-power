#ifndef FLOWER_POWER_PRODUCERFLOWERS_H
#define FLOWER_POWER_PRODUCERFLOWERS_H


#include <string>
#include <utility>
#include <vector>

class ProducerFlowers {
public:
    explicit ProducerFlowers(int producerId, std::string producerName, int rosesStock, int tulipsStock);
    int producerId;
    std::string producerName;
    int rosesStock;
    int tulipsStock;

    std::string serialize();
    static ProducerFlowers deserialize(std::string data);
};

//struct ProducerFlowers {
//    int producerId;
//    std::string producerName;
//    int rosesStock;
//    int tulipsStock;
//};

#endif //FLOWER_POWER_PRODUCERFLOWERS_H
