#ifndef FLOWER_POWER_PRODUCERJOB_H
#define FLOWER_POWER_PRODUCERJOB_H


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include "../Job.h"
#include "../../../utils/Pipe.h"

class ProducerJob : public Job {

public:
    explicit ProducerJob(const int center, const FlowerBox& producerData, Pipe* distributionPipe);
    int run() override;

private:
    int _center;
    int _producerId;
    int _rosesStock;
    int _tulipsStock;
    std::string _producerName;
    Pipe* _distributionPipe;

    FlowerBox generateFlowerBox();

};


#endif //FLOWER_POWER_PRODUCERJOB_H
