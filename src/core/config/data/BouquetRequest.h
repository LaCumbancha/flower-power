#ifndef FLOWER_POWER_BOUQUETREQUEST_H
#define FLOWER_POWER_BOUQUETREQUEST_H


#include <string>
#include <vector>

class BouquetRequest {
public:
    explicit BouquetRequest(int rosesAmount, int tulipsAmount, bool onlineSale = false);
    unsigned int rosesAmount;
    unsigned int tulipsAmount;
    bool onlineSale = false;

    std::string serialize();
    static BouquetRequest deserialize(const std::string& data);
};


#endif //FLOWER_POWER_BOUQUETREQUEST_H
