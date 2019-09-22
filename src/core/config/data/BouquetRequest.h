#ifndef FLOWER_POWER_BOUQUETREQUEST_H
#define FLOWER_POWER_BOUQUETREQUEST_H


#include <string>
#include <vector>

class BouquetRequest {
public:
    explicit BouquetRequest(int rosesAmount, int tulipsAmount);
    unsigned int rosesAmount;
    unsigned int tulipsAmount;

    std::string serialize();
    static BouquetRequest deserialize(const std::string& data);
};


#endif //FLOWER_POWER_BOUQUETREQUEST_H
