#ifndef FLOWER_POWER_CLASSIFIERJOB_H
#define FLOWER_POWER_CLASSIFIERJOB_H


#include <vector>
#include <iostream>
#include "../Job.h"
#include "../../../utils/Pipe.h"
#include "../../../utils/Logger.h"
#include "../../config/data/Flower.h"

class ClassifierJob : public Job {

public:
    explicit ClassifierJob(int center, Pipe* producersPipe);
    int run() override;
    int finish() override;

private:
    int _center;
    Pipe* _producersPipe;

    // TODO: Delete after implementing shared memory.
    std::vector<Flower*> _roses;
    std::vector<Flower*> _tulips;

};


#endif //FLOWER_POWER_CLASSIFIERJOB_H
