#ifndef FLOWER_POWER_PRODUCERJOB_H
#define FLOWER_POWER_PRODUCERJOB_H


#include <iostream>
#include "../Job.h"
#include "../../../utils/Pipe.h"

class ProducerJob : public Job {

public:
    explicit ProducerJob(const ProducerDTO& producerData, Pipe* distributionPipe);
    int run() override;

private:
    int _id;
    int _rosesStock;
    int _tulipsStock;
    std::string _name;
    Pipe* _distributionPipe;

};


#endif //FLOWER_POWER_PRODUCERJOB_H
