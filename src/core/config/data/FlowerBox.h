#ifndef FLOWER_POWER_FLOWERBOX_H
#define FLOWER_POWER_FLOWERBOX_H


#include <string>
#include <utility>
#include <vector>

class FlowerBox {
public:
    explicit FlowerBox(int producerId, std::string producerName, int rosesStock, int tulipsStock);
    int producerId;
    std::string producerName;
    int rosesStock;
    int tulipsStock;

    std::string serialize();
    static FlowerBox deserialize(const std::string& data);
};

#endif //FLOWER_POWER_FLOWERBOX_H
