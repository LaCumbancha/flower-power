#ifndef FLOWER_POWER_DTOS_H
#define FLOWER_POWER_DTOS_H


#include <string>

struct ProducerDTO {
    int producerId;
    std::string producerName;
    int rosesStock;
    int tulipsStock;
};

#endif //FLOWER_POWER_DTOS_H
