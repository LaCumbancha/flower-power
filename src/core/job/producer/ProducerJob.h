#ifndef FLOWER_POWER_PRODUCERJOB_H
#define FLOWER_POWER_PRODUCERJOB_H


#include <iostream>
#include "../Job.h"
#include "../../../utils/Pipe.h"
#include "../../../utils/Logger.h"


class ProducerJob : public Job {

public:
    explicit ProducerJob(int centerId, const FlowerBox* producerData, Pipe* distributionPipe);
    int run() override;
    int finish() override;

private:
    int _centerId;
    int _producerId;
    int _rosesStock;
    int _tulipsStock;
    std::string _producerName;
    Pipe* _distributionPipe;

    FlowerBox generateFlowerBox();

};


#endif //FLOWER_POWER_PRODUCERJOB_H
